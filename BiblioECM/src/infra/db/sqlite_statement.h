#pragma once
#include "statement.h"
#include <sqlite3.h>

class SqliteStatement : public Statement {
public:
    SqliteStatement(sqlite3_stmt* stmt);
    ~SqliteStatement() override;

    void bind(int index, const std::string& value) override;
    void bind(int index, int value) override;
    ECM::Result step() override;
    bool hasRow() override;
    int columnInt(int col) override;
    std::string columnText(int col) override;
    void reset() override;

private:
    sqlite3_stmt* stmt_;
    bool has_row_;
};


