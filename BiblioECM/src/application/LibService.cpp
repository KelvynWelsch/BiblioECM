#include "LibService.h"

#include "../core/utils/TimeUtils.h"


LibService::LibService(
    Database& db,
    UserRepository& users,
    BookRepository& books,
    ItemRepository& items,
    LoanRepository& loans,
    SearchRepository& searches,
    ReservaRepository& reserva
    ) : db(db), users(users), books(books), items(items), loans(loans), searches(searches), reserva(reserva) {}


Error LibService::handleRollback ( Error erroOriginal ) {
    auto rb = db.rollback ( );
    if ( !rb ) {
        return addContext ( erroOriginal, "Erro crítico: Falha ao interromper transação. " );
    }
    return  addContext ( erroOriginal, "Transação interrompida: " );
}

ECM::Result LibService::checkBlock(const UserId id_user) {
    // 1. Buscando o status do usuário
    auto check = users.getUserState(id_user);
    if (!check) {
        return std::unexpected(addContext(check.error(),
            "Não foi possível verificar bloqueio: "));
    }
    UserState state = check.value();
    // 2. Verificando se ele se encontra bloqueado ou inativo
    if (state.status == UserStatus::Bloqueado) {
        if (!state.blocked_until)
            return std::unexpected(blocked());
        return std::unexpected(blocked(state.blocked_until.value()));
    }
    if (state.status == UserStatus::Inativo) {
        return std::unexpected(inactive());
    }
    return {};
}

ECM::Result LibService::checkBlock(const UserState state) {
    if (state.status == UserStatus::Bloqueado) {
        if (!state.blocked_until)
            return std::unexpected(blocked());
        return std::unexpected(blocked(state.blocked_until.value()));
    }
    if (state.status == UserStatus::Inativo) {
        return std::unexpected(inactive());
    }
    return {};
}

ECM::Result LibService::checkLimit(const UserId& id_user) {
    auto nLoanTest = loans.ativosPorUser(id_user);
    if (!nLoanTest) {
        return std::unexpected(addContext(nLoanTest.error(), "Não foi possível verificar o número de empréstimos ativos deste usuário"));
    }
    auto nReservTest = reserva.ativasPorUser(id_user);
    if (!nReservTest) {
        return std::unexpected(addContext(nReservTest.error(), "Não foi possível verificar o número de reservas ativas deste usuário"));
    }
    if (nLoanTest.value() + nReservTest.value() > 3) {
        return std::unexpected(forbidden("Usuário excedeu o limite de empréstimos/reservas ativas"));
    }
    return {};
}

ECM::Result LibService::meetCriteria(const UserId& id_user){
    // 1. Buscando o status do usuário
    auto res = users.getUserState(id_user);
    if (!res) {
        return std::unexpected(addContext(res.error(),
            "Não foi possível verificar o status do usuário: "));
    }
    UserState state = res.value();
    // 2. Verificando se ele se encontra bloqueado ou inativo
    auto check = checkBlock(state);
    if (!check) {
        return std::unexpected(check.error());
    }

    //3. Verificando se o usuário excedeu o limite de empréstimos + reservas ativas
    auto check_limit = LibService::checkLimit(id_user);
    if (!check_limit) {
        return std::unexpected(check_limit.error());
    }

    return {};
}

ECM::Result LibService::cadastrarLivro(const Book& livro) {
    
    auto res = db.begin();
    if (!res) return std::unexpected(addContext(res.error(), "Erro ao iniciar a transação: "));

    
    res = books.salvar(livro);
    if (!res) return std::unexpected(handleRollback(addContext(res.error(), "Erro ao salvar livro: ")));

    
    res = items.salvar(livro, 0, livro.copias);
    if (!res) return std::unexpected(handleRollback(addContext(res.error(), "Erro ao salvar item: ")));

    
    res = db.commit();
    if (!res) return std::unexpected(addContext(res.error(), "Erro ao finalizar a transação: "));

    return {};
}

ECM::Result LibService::salvarUsuario(const User& u) {
    return users.salvar(u);
}

ECM::Result LibService::adicionarItem(const BookId& id, int n) {

    auto res = books.getBook(id);
    if (!res) {
        return std::unexpected(addContext(res.error(), "Não foi possível adicionar os itens extras pois não foi possível recuperar o livro desejado pelo Id: "));
    }
    auto nTest = items.maxId(id);
    if (!nTest) {
        return std::unexpected(nTest.error());
    }
    int n_Ex = nTest.value();
    auto bg = db.begin();
    if (!bg) return std::unexpected(addContext(bg.error(), "Erro ao iniciar a transação: "));

    auto result = items.salvar(res.value(), n_Ex, n);
    if (!result) return std::unexpected(handleRollback(addContext(result.error(), "Erro ao salvar item: ")));


    auto Commit = db.commit();
    if (!Commit) return std::unexpected(addContext(Commit.error(), "Erro ao finalizar a transação: "));

    return {};

}

ECM::Expected<std::vector<BookSearchResult>>
LibService::buscarLivros(std::string_view term, BookSearchField field) {

    if (term.empty())
        return std::unexpected(validation("Não foi possível realizar a busca: palavra-chave vazia."));

    auto res = searches.searchBooks(term, field);

    if (!res)
        return std::unexpected(addContext(res.error(), "Erro ao realizar busca: "));

    if (res->empty())
        return std::unexpected(notFound("Nenhum livro encontrado."));


    return res.value();
}

ECM::Expected<std::vector<ReservaInfo>> LibService::buscarReservas(const UserId& id) {
    auto res = reserva.listarAtivasPorUser(id);

    if (!res)
        return std::unexpected(addContext(res.error(), "Erro ao realizar busca: "));

    if (res->empty())
        return std::unexpected(notFound("Nenhuma reserva encontrada."));


    return res.value();
}

ECM::Expected<std::vector<UserSearchResult>> LibService::buscarUsuario(std::string_view term) {
    if (term.empty())
        return std::unexpected(validation("Não foi possível realizar a busca: palavra-chave vazia."));

    auto res = searches.searchUser(term);

    if (!res)
        return std::unexpected(addContext(res.error(), "Erro ao realizar busca: "));

    if (res->empty())
        return std::unexpected(notFound("Nenhum usuário encontrado."));

    return res.value();
}

ECM::Expected<std::vector<UserSearchResult>> LibService::buscarUsuarioId(const UserId& id) {

    auto res = searches.searchUser(id.get_Id_int());

    if (!res)
        return std::unexpected(addContext(res.error(), "Erro ao realizar busca: "));

    if (res->empty())
        return std::unexpected(notFound("Nenhum usuário encontrado."));

    return res.value();
}

ECM::Expected<User> LibService::idToUser(const UserId& id) {
    auto res = users.getUser(id);
    if (!res) {
        return std::unexpected(addContext(res.error(), "Não foi possível encontrar o usuário: "));
    }
    return res.value();
}

ECM::Result LibService::alterarUsuario(const User& u) {
    return users.update(u);
}

ECM::Result LibService::cancelarReserva(int id) {

    auto res = reserva.updateStatus(id, ReservaStatus::Cancelada);

    if (!res) {
        return std::unexpected(addContext(res.error(), "Não foi possível cancelar a reserva: "));
    }

    return {};
}

ECM::Expected<std::chrono::year_month_day> LibService::preverDisponibilidade(const BookId& book){
    //1. buscando o vetor de datas previstas das devoluções
    auto test = loans.datasPrevistasPorLivro(book);
    if (!test) {
        return std::unexpected(addContext(test.error(), "Não foi possível prever data de disponibilidade: "));
    }
    auto datas = test.value();
    if (datas.empty()) {
        return std::unexpected(conflict("Não foi possível efetivar a reserva: há itens disponíveis deste título!"));
    }

    //2. buscando o número de reservas ativas do livro
    auto test2 = reserva.ativasPorLivro(book);
    if (!test2) {
        return std::unexpected(addContext(test.error(), "Não foi possível prever data de disponibilidade: "));
    }
    int ativas = test2.value();
   
    //3. buscando o número total de itens
    auto test3 = items.quantosLivros(book);
    if (!test3) {
        return std::unexpected(addContext(test.error(), "Não foi possível prever data de disponibilidade: "));
    }
    int tot = test3.value();

    //4. fazendo a previsão
    if (ativas >= tot) {
        return std::unexpected(unknown("Não foi possível prever data de disponibilidade (número de reservas maior que o número de itens)"));
    }
    else {
        return std::chrono::sys_days(datas[ativas]);
    }
}

ECM::Expected<Loan> LibService::emprestar(const UserId& id_user, const ItemId& id_item) {

    // 1. Buscando o status do usuário
    auto res = users.getUserState(id_user);
    if (!res) {
        return std::unexpected(addContext(res.error(), 
        "Não foi possível realizar o empréstimo pois não foi possível verificar o status do usuário: "));
    }
    UserState state = res.value();
    // 2. Verificando se ele se encontra bloqueado ou inativo
    auto check = checkBlock(state);
    if (!check) {
        return std::unexpected(addContext(check.error(),
            "Não foi possível realizar o empréstimo: "));
    }

    //3. Verificando se o usuário excedeu o limite de empréstimos + reservas ativas
    auto check_limit = LibService::checkLimit(id_user);
    if (!check_limit) {
        return std::unexpected(check_limit.error());
    }
 
        //4. Verificando se o item existe
        auto check_item = items.buscarId(id_item);
        if (!check_item) {
            return std::unexpected(addContext(check_item.error(), "Não foi possível realizar o empréstimo: "));
        }
        //5. Verificando se o item possui reserva ativa
        auto checkReserva = reserva.proximaReserva(id_item.getBook());

        if (!checkReserva && (checkReserva.error().type != Error::Type::NotFound)) {
            return std::unexpected(checkReserva.error());
        }
        ReservaInfo info;

        if (checkReserva) {
            info = checkReserva.value();
            // 6. checando se a reserva está no nome de quem está realizando o empréstimo
            if (info.id_user != id_user.get_Id_int()) {
                // 7. retornar informação da reserva
                return std::unexpected(reserved(info.nome_user));
            }
        }
        //8. Calculando prazo e construindo objeto loan
        std::chrono::year_month_day loaned = today();
        std::chrono::year_month_day due = std::chrono::sys_days(loaned) + prazoPerRole(state.role);
        Loan l(0, id_item, id_user, loaned, due);

        //9. Iniciando transação
        
        auto res_begin = db.begin ( );
        if ( !res_begin ) return std::unexpected ( addContext ( res.error ( ), "Erro ao iniciar a transação: " ) );

        //9. Salvando empréstimo
        auto res_salvar = loans.salvar(l);
        if (!res_salvar) {
            return std::unexpected(handleRollback ( addContext ( res_salvar.error ( ), "Não foi possível registrar o empréstimo: " ) ));
            return std::unexpected(addContext(res_salvar.error(), "Não foi possível registrar o empréstimo: "));
        }

        //10. Alterando status da reserva
        if (checkReserva) {
            auto update_reserva = reserva.updateStatus(info.id_reserva, ReservaStatus::Atendida);
            if (!update_reserva) {
                return std::unexpected(handleRollback ( addContext ( update_reserva.error ( ), "Não foi possível efetivar a reserva!: " ) ));
                return std::unexpected(addContext(update_reserva.error(), "Não foi possível efetivar a reserva!"));
            }
        }

        auto res_commit = db.commit ( );
        if ( !res ) return std::unexpected ( addContext ( res_commit.error ( ), "Erro ao finalizar a transação: " ) );

        return l;
}

ECM::Result LibService::devolver(const UserId& user, const ItemId& item) {

    // 1. Buscando o status do empréstimo
    auto loanState = loans.getLoanState(user, item);
    if (!loanState) {
        return std::unexpected(addContext(loanState.error(),
            "Não foi possível localizar o empréstimo: "));
    }

    LoanState state = loanState.value();

    // 2. Verificando o status
    if (state.status == LoanStatus::Devolvido) {
        return std::unexpected(conflict("Este item já foi devolvido."));
    }

    // 3. Registrando devolução
    auto resDev = loans.realizarDevolucao(state.loan_id);
    if (!resDev) {
        return std::unexpected(addContext(resDev.error(),
            "Não foi possível registrar a devolução: "));
    }

    // 4. Verificando atraso
    auto hoje = today();
    auto atraso = std::chrono::sys_days(hoje) - std::chrono::sys_days(state.due_at);

    if (atraso > std::chrono::days{ 0 }) {

        // 5. Bloqueia usuário
        auto resBlock = users.block(user, atraso);
        if (!resBlock) {
            return std::unexpected(addContext(resBlock.error(),
                "Devolução realizada com sucesso, mas falha ao bloquear usuário: "));
            // TODO: resolver a questão que, nesse caso, o sistema retorna e não checa se o livro tem reserva ativa
            
        }

        // 6. Retorna dados de bloqueio
        return std::unexpected(checkBlock(user).error());
    }

    // 7. checa se existe reserva ativa com este título
    auto checkReserva = reserva.proximaReserva(item.getBook());

    if (!checkReserva && (checkReserva.error().type != Error::Type::NotFound)) {
        return std::unexpected(checkReserva.error());
    }

    
    if (checkReserva) {
        //std::cout << "Deu bom a reserva" << std::endl;
        ReservaInfo info = checkReserva.value();
        // 8. mudar status da reserva
        auto res_update = reserva.updateStatus(info.id_reserva, ReservaStatus::AguardandoRetirada);
        // 9. retornar informação da reserva
        return std::unexpected(reserved(info.nome_user));
    }

    // 9. Sucesso total
    return {};
}

ECM::Result LibService::renovar(const UserId& user, const ItemId& item) {

    constexpr int LIMITE_RENOVACOES = 3;
    constexpr int PRAZO_DIAS = 14;

    // 1. Buscando o status do empréstimo
    auto loanState = loans.getLoanState(user, item);
    if (!loanState) {
        return std::unexpected(addContext(loanState.error(),
            "Não foi possível localizar o empréstimo: "));
    }

    LoanState state = loanState.value();

    // 2. Validando status do empréstimo
    if (state.status == LoanStatus::Devolvido) {
        return std::unexpected(validation("Este item já foi devolvido."));
    }

    // 3. Verificando atraso 
    auto hoje = today();
    auto atraso = std::chrono::sys_days(hoje) - std::chrono::sys_days(state.due_at);

    if (atraso > std::chrono::days{ 0 }) {

        // 3.1 Bloqueia usuário
        auto resBlock = users.block(user, atraso);
        if (!resBlock) {
            return std::unexpected(addContext(resBlock.error(),
                "Empréstimo em atraso, mas falha ao bloquear usuário: "));
        }

        // 4. Retorna dados de bloqueio
        return std::unexpected(addContext(checkBlock(user).error(), "Devolução realizada com sucesso."));
    }

    // 4. Verifica limite de renovações
    if (state.n_Renov >= LIMITE_RENOVACOES) {
        return std::unexpected(forbidden("Limite de renovações atingido."));
    }

    // 5. Renova empréstimo
    auto res = loans.renovar(state.loan_id, PRAZO_DIAS);
    if (!res) {
        return std::unexpected(addContext(res.error(),
            "Não foi possível renovar o empréstimo: "));
    }

    return {};
}

ECM::Result LibService::reservar(const UserId& user_id, const BookId& book_id){
    // 1. Buscando o status do usuário
    auto res = users.getUserState(user_id);
    if (!res) {
        return std::unexpected(addContext(res.error(),
            "Não foi possível realizar a reserva pois não foi possível verificar o status do usuário: "));
    }
    UserState state = res.value();
    // 2. Verificando se ele se encontra bloqueado ou inativo
    auto check = checkBlock(state);
    if (!check) {
        return std::unexpected(addContext(check.error(),
            "Não foi possível efetivar a reserva: "));
    }
    // 3. verificando se o usuário já possui reserva ativa no mesmo título
    auto check2 = reserva.usuarioJaReservou(user_id, book_id);
    if (!check2) {
        return std::unexpected(check2.error());
    }
    // 4. verificando se o usuário possui empréstimo ativo no mesmo título
    auto res_loans = loans.temEmprestimo(user_id, book_id);
    if (!res_loans) {
        return std::unexpected(addContext(res_loans.error(), "Não foi possível fazer a reserva: "));
    }
    // 5. verificando se não há itens disponíveis no mesmo título
    auto check_disp = items.quantosDisponiveis(book_id);
    if (!items.quantosDisponiveis(book_id)) {
        return std::unexpected(conflict("Não foi possível efetivar a reserva pois não foi possível verificar se há itens disponíveis."));
    }
    if (check_disp.value()) {
        return std::unexpected(conflict("Não foi possível efetivar a reserva: há itens disponíveis deste título!"));
    }
    //6. Verificando se o usuário excedeu o limite de empréstimos + reservas ativas
    auto check_limit = LibService::checkLimit(user_id);
    if (!check_limit) {
        return std::unexpected(check_limit.error());
    }
    
    // 7. efetivando reserva
    auto result = reserva.inserir(Reserva(user_id, book_id, today()));

    if (!result) {
        return std::unexpected(addContext(result.error(), "Não foi possível fazer a reserva: "));
    }

    return{};
}

