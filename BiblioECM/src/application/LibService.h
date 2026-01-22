#pragma once

/* Essa é a classe que coordena, orquestra, o que cada repositório deve fazer em cada serviço básico da
biblioteca: emprestar, receber devolução, renovar empréstimo e reservar um título. É aqui que devem ser
implementadas as regras de negócio, isto é, por quanto tempo um usuário ter um item emprestado, quantas
renovações são possíveis no máximo, em quais casos alguém pode reservar um título, etc. Além destes 
serviços básicos, esta classe também está dotada de métodos auxiliares. */

#include "../infra/repositories/UserRepository.h"
#include "../infra/repositories/BookRepository.h"
#include "../infra/repositories/ItemRepository.h"
#include "../infra/repositories/LoanRepository.h"
#include "../infra/repositories/SearchRepository.h"
#include "../infra/repositories/ReservaRepository.h"


class LibService {
public:
    LibService(
        Database& db,
        UserRepository& users,
        BookRepository& books,
        ItemRepository& items,
        LoanRepository& loans,
        SearchRepository& searches,
        ReservaRepository& reserva
    );
    //------------- util/Transactions ---------------
    Error handleRollback(Error erroOriginal);
    //------------- checks --------------------------
    ECM::Result checkBlock(const UserId id_user);
    ECM::Result checkBlock(const UserState state);
    ECM::Result checkLimit(const UserId& id_user);
    ECM::Result meetCriteria(const UserId& id_user);
    //------------- register ------------------------
    ECM::Result cadastrarLivro(const Book& b);
    ECM::Result salvarUsuario(const User& u);
    ECM::Result adicionarItem(const BookId& id, int n);
    //-------------- read ---------------------------
    ECM::Expected<std::vector<BookSearchResult>> buscarLivros(std::string_view term, BookSearchField field);
    ECM::Expected<std::vector<UserSearchResult>> buscarUsuario(std::string_view term);
    ECM::Expected<std::vector<UserSearchResult>> buscarUsuarioId(const UserId& id);
    ECM::Expected<User> idToUser(const UserId& id);
    ECM::Expected<std::vector<ReservaInfo>> buscarReservas(const UserId&);
    //-------------- modify -------------------------
    ECM::Result alterarUsuario(const User& u);
    ECM::Result cancelarReserva(int id);
    //-------------- forecast -----------------------
    ECM::Expected<std::chrono::year_month_day> preverDisponibilidade(const BookId& book);
    //-------------- services------------------------
    ECM::Expected<Loan> emprestar(const UserId& id_user, const ItemId& id_item);
    ECM::Result devolver(const UserId& user, const ItemId& item);
    ECM::Result renovar(const UserId& user, const ItemId& item);
    ECM::Result reservar(const UserId&, const BookId&);
private:
    Database& db;
    UserRepository& users;
    BookRepository& books;
    ItemRepository& items;
    LoanRepository& loans;
    SearchRepository& searches;
    ReservaRepository& reserva;
};

inline std::chrono::days prazoPerRole(UserRole role) {
    if (role == UserRole::Normal) {
        return  std::chrono::days(14);
    }
    return std::chrono::days(14);
}


