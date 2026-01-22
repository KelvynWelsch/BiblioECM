#pragma once

#include "../utils/TimeUtils.h"

#include <expected>
#include <string>
#include <variant>
#include <chrono>

    struct Extra {
        std::optional<std::chrono::year_month_day> blocked_until;
    };

    // -------------------------

    // Erro de aplicação

    // -------------------------

    struct Error {

        enum class Type {
            Unknown,
            NotFound,       // entidade não existe
            DatabaseError,  // falha técnica (sqlite, IO, etc)
            Validation,     // dado inválido
            Blocked,        // usuário bloqueado
            Inactive,       // usuário inativo
            Overdue,        // empréstimo atrasado
            Conflict,        // estado inválido (ex: item já emprestado)
            Forbidden,        // por ex., já renovou muitas vezes
            Reserved         // obra reservada
        };


        Type type{ Type::Unknown };

        std::string message{};

        Extra extra{};

    };

    // -------------------------
    // Alias padrão de retorno
    // -------------------------

    namespace ECM {

        template <typename T>
        using Expected = std::expected<T, Error>;

        using Result = Expected<void>;

    }

    // -------------------------
    // Helpers 
    // -------------------------

    inline Error unknown(std::string msg) {
        return { Error::Type::Unknown, std::move(msg)};
    }

    inline Error notFound(std::string msg) {
        return { Error::Type::NotFound, std::move(msg) };
    }

    inline Error database(std::string msg) {
        return { Error::Type::DatabaseError, std::move(msg) };
    }

    inline Error validation(std::string msg) {
        return { Error::Type::Validation, std::move(msg)};
    }

    inline Error blocked(std::string msg = "Usuário bloqueado sem data de desbloqueio.") {
        return Error{
            .type = Error::Type::Blocked,
            .message = std::move(msg)
        };
    }

    inline Error blocked(std::chrono::year_month_day data, std::string msg = "Usuário bloqueado até: ") {
        return Error{
            .type = Error::Type::Blocked,
            .message = std::move(msg) + timeToStringUser(data),
            .extra = Extra{data}
        };
    }

    inline Error inactive(std::string msg = "Usuário inativo.") {
        return { Error::Type::Inactive, std::move(msg)};
    }

    inline Error conflict(std::string msg) {
        return { Error::Type::Conflict, std::move(msg)};
    }

    inline Error overdue(std::chrono::year_month_day data,
        std::string msg = "Empréstimo em atraso!") { 
        return { Error::Type::Overdue, std::move(msg), Extra{data} };
    }

    inline Error forbidden(std::string msg) {
        return { Error::Type::Conflict, std::move(msg), {} };
    }

    inline Error reserved(std::string msg) {
        return { Error::Type::Reserved, "Este livro possui reserva!\nSeparar este exemplar!\nReservado por: " + std::move(msg), {} };
    }

    inline Error addContext(Error e, const std::string& ctx) {
        e.message = ctx + "\n" + e.message;
        return e;
    }
