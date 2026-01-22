#pragma once

#include "../../core/entities/User.h"
#include "../db/database.h"

#include "../../core/utils/TimeUtils.h"

class UserRepository {
private:
    Database& db;
public:
    explicit UserRepository(Database& db);
    ECM::Result salvar(const User& u);
    ECM::Result update(const User& u);
    ECM::Expected<UserState> getUserState(const UserId& id);
    ECM::Result block(UserId id, std::chrono::days atraso);
    ECM::Expected<User> getUser(const UserId& user);
};


