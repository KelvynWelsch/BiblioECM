#pragma once
#include <string>
#include "../ids/UserId.h"
#include "../states/UserRole.h"
#include "../states/UserState.h"
#include "../value_objects/email.h"

class User {
public:
    UserId id;
    std::string nome;
    Email email;
    std::string tel;
    UserRole role;
    UserStatus status;
    std::optional<std::chrono::year_month_day> blocked_until;

    User() = default;
    User(UserId id, std::string nome, Email email, std::string tel)
        :id(id), nome(std::move(nome)), email(email), tel(std::move(tel)) {
        role = UserRole::Normal;
        status = UserStatus::Ativo;
    }
    User(UserId id, 
        std::string nome,
        Email email,
        std::string tel, 
        UserStatus status, 
        std::optional<std::chrono::year_month_day> blocked_until)
        :id(id), nome(nome), email(email), tel(tel), status(status), blocked_until(blocked_until) {}

    static std::string roletostr(UserRole r) {
        switch (r) {
        case UserRole::Normal: return "Normal";
        case UserRole::Professor: return "Professor";
        }
    }

    static std::string statustostr(UserStatus s) {
        switch (s) {
        case UserStatus::Ativo: return "Ativo";
        case UserStatus::Bloqueado: return "Bloqueado";
        case UserStatus::Inativo: return "Inativo";
        }
    }

    static UserRole parseUserRole(int s) {
        switch (s) {
        case 1: return UserRole::Normal;
        default: return UserRole::Normal;
        }
    }

    static UserStatus parseUserStatus(int s) {
        switch (s) {
        case 0: return UserStatus::Inativo;
        case 2: return UserStatus::Bloqueado;
        default: return UserStatus::Ativo;
        }
    }
};
