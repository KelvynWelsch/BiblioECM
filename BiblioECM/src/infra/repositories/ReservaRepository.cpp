#include "ReservaRepository.h"
#include "../../core/utils/TimeUtils.h"

ReservaRepository::ReservaRepository(Database& db) : db(db) {}

ECM::Result ReservaRepository::inserir(const Reserva& r) {
    auto prep = db.prepare(R"(
        INSERT INTO reservas (user_id, book_id, data_reserva, status)
        VALUES (?, ?, ?, ?)
    )");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, r.getUser().get_Id_int());
    stmt->bind(2, r.getBook().get_Id_int());
    stmt->bind(3, timeToString(today()));
    stmt->bind(4, static_cast<int>(r.getStatus()));

    auto res = stmt->step();

    if (!res) {
        return std::unexpected(addContext(res.error(), "Erro ao inserir reserva: "));
    }

}

ECM::Result ReservaRepository::usuarioJaReservou(const UserId& user, const BookId& book) {
    auto prep = db.prepare(R"(
        SELECT 1 FROM reservas WHERE user_id = ? AND book_id = ? AND status = 1 LIMIT 1;
    )");

    if (!prep.has_value()) {
        return std::unexpected(addContext(prep.error(), "Não foi possível verificar se o usuário possui reservas ativas no mesmo título: "));
    }

    auto& stmt = prep.value();

    stmt->bind(1, user.get_Id_int());
    stmt->bind(2, book.get_Id_int());

    auto res = stmt->step();

    if (res.has_value()) {
        if (stmt->hasRow()) {
            return std::unexpected(conflict("Usuário já reservou este título!"));
        }
        return {};
    }
    return std::unexpected(addContext(res.error(), "Não foi possível verificar se o usuário possui reservas ativas no mesmo título: "));
}

ECM::Expected<int> ReservaRepository::ativasPorLivro(const BookId& book) {
    auto prep = db.prepare(R"(
        SELECT COUNT(*)
        FROM reservas
        WHERE book_id = ?
        AND status = 1;
    )");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, book.get_Id_int());

    auto res = stmt->step();

    if (res.has_value()) {
        return stmt->columnInt(0);
    }
    return std::unexpected(res.error());
    
}

ECM::Expected<ReservaInfo> ReservaRepository::proximaReserva(const BookId& book) {
    auto prep = db.prepare(R"(SELECT r.id, m.id, m.nome, l.titulo
                                FROM reservas r 
                                JOIN membros m ON r.user_id = m.id
                                JOIN livros l ON r.book_id = l.id
                                WHERE r.book_id = ? AND r.status > 0 
                                ORDER BY r.data_reserva ASC LIMIT 1)");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, book.get_Id_int());

    auto res = stmt->step();

    if (!res) {
        return std::unexpected(res.error());
    }
     

    if (stmt->hasRow()) {
        ReservaInfo info = { stmt->columnInt(0), stmt->columnInt(1), stmt->columnText(2), stmt->columnText(3)};
        return info;
    }
    return std::unexpected(notFound(""));
}

ECM::Result ReservaRepository::updateStatus(int id, ReservaStatus status) {
    int intStatus = static_cast<int>(status);
    auto prep = db.prepare(R"(UPDATE reservas SET status = ? WHERE id = ?)");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, intStatus);
    stmt->bind(2, id);

    auto res = stmt->step();

    if (!res) {
        return std::unexpected(res.error());
    }

    return {};
}

ECM::Expected<std::vector<ReservaInfo>> ReservaRepository::listarAtivasPorUser(const UserId& user_id) {
    auto prep = db.prepare(R"(SELECT r.id, m.id, m.nome, l.titulo
                                FROM reservas r 
                                JOIN membros m ON r.user_id = m.id
                                JOIN livros l ON r.book_id = l.id
                                WHERE r.user_id = ? AND r.status > 0)");


    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, user_id.get_Id_int());

    std::vector<ReservaInfo> results;

    while (true) {
        auto res = stmt->step();
        if (!res) return std::unexpected(res.error());
        if (!stmt->hasRow()) break;

        ReservaInfo dto{
            .id_reserva = stmt->columnInt(0),
            .id_user = stmt->columnInt(1),
            .nome_user = stmt->columnText(2),
            .nome_livro = stmt->columnText(3)
        };

        results.push_back(std::move(dto));
    }

    return results;
}

ECM::Expected<int> ReservaRepository::ativasPorUser(const UserId& user_id) {
    auto prep = db.prepare(R"(SELECT COUNT(*)
                                FROM reservas r 
                                WHERE r.user_id = ? AND r.status > 0)");


    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, user_id.get_Id_int());

    auto res = stmt->step();
    
    if (!res) return std::unexpected(res.error());

    return stmt->columnInt(0);
}


