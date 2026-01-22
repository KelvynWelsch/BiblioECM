#pragma once

#include "../ids/UserId.h"
#include <string>

struct UserSearchResult {
    UserId user_id;
    std::string nome;
};
