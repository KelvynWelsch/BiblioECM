#pragma once

#include "statement.h"

#include <memory>


class Database {
public:
    virtual ~Database() = default;

    virtual ECM::Result exec(std::string_view sql) = 0;
    virtual ECM::Expected<std::unique_ptr<Statement>> prepare(const std::string& sql) = 0;

    virtual ECM::Result begin() = 0;
    virtual ECM::Result commit() = 0;
    virtual ECM::Result rollback() = 0;
};


