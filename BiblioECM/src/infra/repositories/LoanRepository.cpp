#include "LoanRepository.h"
#include "../db/database.h"
#include "../../core/utils/TimeUtils.h"

LoanRepository::LoanRepository(Database& db) : db(db) {}

ECM::Result LoanRepository::salvar(const Loan& l) {
    auto prep = db.prepare(R"(
        INSERT INTO loans (item_id, user_id, loaned_at, due_at, status)
        VALUES (?, ?, ?, ?, ?);
    )");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, l.item_id.get_Id());
    stmt->bind(2, l.user_id.get_Id_raw());
    stmt->bind(3, timeToString(l.loaned_at));
    stmt->bind(4, timeToString(l.due_at));
    stmt->bind(5, static_cast<int>(LoanStatus::Ativo)); 

    return stmt->step();
}
    
ECM::Expected<LoanState> LoanRepository::getLoanState(const UserId& user, const ItemId& item) {
    auto prep = db.prepare(R"(
        SELECT id, status, due_at, n_Renov, returned_at FROM loans 
        WHERE user_id = ? AND item_id = ? AND status != 0;
    )");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, user.get_Id_raw());
    stmt->bind(2, item.get_Id());

    auto res = stmt->step();

    if (res.has_value()) {
        if (stmt->hasRow()) {
            int id = stmt->columnInt(0);
            LoanStatus status = Loan::parseLoanStatus(stmt->columnInt(1));
            auto due_at = parseDate(stmt->columnText(2));
            int n_Renov = stmt->columnInt(3);
            std::string returned_at = stmt->columnText(4);
            if (returned_at.empty() && status == LoanStatus::Devolvido) {
                return std::unexpected(unknown("Dados Inconsistentes!"));
            }
            if (returned_at.empty()) {
                return LoanState{ id, status, due_at, n_Renov, std::nullopt };
            }
            return LoanState{ id, status, due_at, n_Renov, parseDate(returned_at)};
        }
        return std::unexpected(notFound("Empréstimo não encontrado!"));
    }

    return std::unexpected(res.error());
}



ECM::Result LoanRepository::realizarDevolucao(int loanId) {
    auto prep = db.prepare("UPDATE loans SET returned_at = ?, status = 0 WHERE id = ?;");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();
    
    auto agora = today();
    stmt->bind(1, timeToString(agora));
    stmt->bind(2, loanId);

    return stmt->step();
}

ECM::Result LoanRepository::renovar(int loanId, int prazoDias) {

    // 1. Busca número atual de renovações
    auto prepSelect = db.prepare(
        "SELECT n_Renov FROM loans WHERE id = ?;"
    );

    if (!prepSelect) {
        return std::unexpected(prepSelect.error());
    }

    auto& stmtSelect = prepSelect.value();
    stmtSelect->bind(1, loanId);

    auto res = stmtSelect->step();
    if (!res) {
        return std::unexpected(res.error());
    }

    if (!stmtSelect->hasRow()) {
        return std::unexpected(notFound("Empréstimo não encontrado."));
    }

    int n_Renov = stmtSelect->columnInt(0);

    // 2. Calcula nova data de vencimento
    auto hoje = today();
    std::chrono::year_month_day novoVencimento =
        std::chrono::sys_days(hoje) + std::chrono::days(prazoDias);

    // 3. Atualiza empréstimo
    auto prepUpdate = db.prepare(
        "UPDATE loans SET due_at = ?, n_Renov = ? WHERE id = ?;"
    );

    if (!prepUpdate) {
        return std::unexpected(prepUpdate.error());
    }

    auto& stmtUpdate = prepUpdate.value();
    stmtUpdate->bind(1, timeToString(novoVencimento));
    stmtUpdate->bind(2, n_Renov + 1);
    stmtUpdate->bind(3, loanId);

    return stmtUpdate->step();
}

ECM::Result LoanRepository::temEmprestimo(const UserId& user, const BookId& book) {
    auto prep = db.prepare(
        "SELECT 1 FROM loans l JOIN itens i ON l.item_id = i.id WHERE l.user_id = ? AND l.status !=0 AND i.book_id = ? LIMIT 1;"
    );

    if (!prep) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();
    stmt->bind(1, user.get_Id_int());
    stmt->bind(2, book.get_Id_int());


    auto res = stmt->step();
    if (!res) {
        return std::unexpected(res.error());
    }

    if (stmt->hasRow()) {
        return std::unexpected(conflict("O usuário já possui empréstimo ativo neste título"));
    }

    return {};
}

ECM::Expected<std::vector<std::chrono::year_month_day>> LoanRepository::datasPrevistasPorLivro(const BookId& book) {
    auto prep = db.prepare(R"(
        SELECT l.due_at
            FROM loans l
            JOIN itens i ON i.id = l.item_id
            WHERE i.book_id = ?
            AND l.status != 0
            ORDER BY l.due_at ASC;
    )");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, book.get_Id_int());

    std::vector<std::chrono::year_month_day> results;

    while (true) {
        auto res = stmt->step();
        if (!res) return std::unexpected(res.error());
        if (!stmt->hasRow()) break;

        results.push_back(parseDate(stmt->columnText(0)));
    }

    return results;

    
}

ECM::Expected<int> LoanRepository::ativosPorUser(const UserId& user_id) {
    auto prep = db.prepare(R"(SELECT COUNT(*)
                                FROM loans l 
                                WHERE l.user_id = ? AND l.status > 0)");


    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, user_id.get_Id_int());

    auto res = stmt->step();

    if (!res) return std::unexpected(res.error());

    return stmt->columnInt(0);
}
