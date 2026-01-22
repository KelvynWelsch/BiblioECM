#include "UserRepository.h"
#include "../db/database.h"
#include <chrono>


UserRepository::UserRepository(Database& db) : db(db) {}

ECM::Result UserRepository::salvar(const User& u) {
    auto prep = db.prepare(R"(
        INSERT INTO membros (id, nome, email, tel, role)
        VALUES (?, ?, ?, ?, ?)
    )");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, u.id.get_Id_int());
    stmt->bind(2, u.nome);
    stmt->bind(3, u.email.get());
    stmt->bind(4, u.tel);
    stmt->bind(5, 1); //TODO: consertar essa parte de roles 

    auto res = stmt->step();

    if (!res) {
        return std::unexpected(addContext(res.error(), "Erro ao salvar usuário: "));
    }

    return {};
}

ECM::Result UserRepository::update(const User& u) {
    auto prep = db.prepare(R"(
        UPDATE users SET nome = ?, email = ?, tel = ? WHERE id = ?
    )");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    
    stmt->bind(1, u.nome);
    stmt->bind(2, u.email.get());
    stmt->bind(3, u.tel); 
    // TODO: colocar possibilidade de alterar role
    stmt->bind(4, u.id.get_Id_int());
    

    auto res = stmt->step();

    if (!res) {
        return std::unexpected(addContext(res.error(), "Erro ao atualizar usuário: "));
    }

    return {};
}

ECM::Expected<UserState> UserRepository::getUserState(const UserId& id) {
    auto prep = db.prepare(R"(SELECT status, role, blocked_until FROM membros WHERE id = ?)");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, id.get_Id_int());

    ECM::Result res = stmt->step();

    if (!res.has_value()) {
        return std::unexpected(res.error());
    }

    if (!stmt->hasRow()) {
        return std::unexpected(notFound("Usuário não encontrado!"));
    }
    int status = stmt->columnInt(0);

    if (status == 2) {
        std::string blocked_until = stmt->columnText(2);
        if (blocked_until.empty()) {
            return UserState{ UserStatus::Bloqueado};
        }
        return UserState{ UserStatus::Bloqueado, User::parseUserRole(stmt->columnInt(1)) , parseDate(blocked_until)};
    }
    if (status == 0) {
        return UserState{ UserStatus::Inativo,  User::parseUserRole(stmt->columnInt(1)), std::nullopt };
    }
    if (status == 1) {
        return UserState{ UserStatus::Ativo,  User::parseUserRole(stmt->columnInt(1)), std::nullopt };
    }
    return std::unexpected(unknown("Erro desconhecido."));
}


 ECM::Result UserRepository::block(UserId id, std::chrono::days atraso) { 
     auto check = db.prepare(R"(SELECT blocked_until FROM membros WHERE id = ?)");

     if (!check.has_value()) {
         return std::unexpected(check.error()); 
     }

     auto& stmt = check.value();

     stmt->bind(1, id.get_Id_int());

     auto res = stmt->step();

     if (!res) {
         return std::unexpected(addContext(res.error(), "Não foi possível buscar até quando usuário encontra-se bloqueado: "));
     }

     std::string blockString = stmt->columnText(0);

     std::chrono::year_month_day block = today();

     if (!blockString.empty()) {
         block = parseDate(blockString);
     }

     auto prep = db.prepare(R"(UPDATE membros SET status = 2, blocked_until = ? WHERE id = ?)");

     if (!prep.has_value()) {
         return std::unexpected(prep.error()); 
     }

     auto& stmt2 = prep.value();

     block = std::chrono::sys_days(block) + atraso;

     stmt2->bind(1, timeToString(block));
     stmt2->bind(2, id.get_Id_int());

     return (stmt2->step());
 }

 ECM::Expected<User> UserRepository::getUser(const UserId& user) {
     auto prep = db.prepare(R"(
        SELECT id, nome, email, tel, status, blocked_until FROM membros
        WHERE id = ?;
    )");

     if (!prep.has_value()) {
         return std::unexpected(prep.error());
     }
     

     auto& stmt = prep.value();

     stmt->bind(1, user.get_Id_int());
     
     auto res = stmt->step();

     if (res.has_value()) {
         if (stmt->hasRow()) {
             UserId id = UserId::fromDatabase(stmt->columnInt(0));
             std::string nome = stmt->columnText(1);
             Email email = Email::fromDatabase(stmt->columnText(2));
             std::string tel = stmt->columnText(3);
             UserStatus status = User::parseUserStatus(stmt->columnInt(4));

             std::optional<std::chrono::year_month_day> blocked_until;
             if (stmt->columnText(5).empty()) {
                 blocked_until = std::nullopt;
             }
             else {
                 blocked_until = parseDate(stmt->columnText(5));
             }
             User u = User(id, nome, email, tel, status, blocked_until);
             return u;
         }
         return std::unexpected(notFound("Usuário não encontrado!"));
     }

     return std::unexpected(res.error());
 }
