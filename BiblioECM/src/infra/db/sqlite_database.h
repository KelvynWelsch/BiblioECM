#pragma once

#include "database.h"
#include "statement.h"

#include "../../external/sqlite/sqlite3.h"



class SqliteDatabase : public Database {
public:
    explicit SqliteDatabase(const std::string& path);
    ~SqliteDatabase();

    ECM::Result exec(std::string_view sql) override;
    ECM::Expected<std::unique_ptr<Statement>> prepare(const std::string& sql) override;

    ECM::Result begin() override;
    ECM::Result commit() override;
    ECM::Result rollback() override;

    sqlite3* raw();

     
private:
    sqlite3* db_;
};


