#pragma once

#include <optional>
#include <chrono>

enum class UserStatus {
    Ativo = 1,
    Bloqueado = 2,
    Inativo = 0
};

struct UserState {
    UserStatus status;
    UserRole role;
    std::optional<std::chrono::year_month_day> blocked_until ;
};

