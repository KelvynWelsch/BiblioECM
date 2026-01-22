#include "sqlite_statement.h"



SqliteStatement::SqliteStatement(sqlite3_stmt* stmt)
    : stmt_(stmt), has_row_(false)
{
  
}

SqliteStatement::~SqliteStatement() {
    if (stmt_) sqlite3_finalize(stmt_);
}

void SqliteStatement::bind(int i, const std::string& v) {
    sqlite3_bind_text(stmt_, i, v.c_str(), -1, SQLITE_TRANSIENT);
}

void SqliteStatement::bind(int i, int v) {
    sqlite3_bind_int(stmt_, i, v);
}

ECM::Result SqliteStatement::step() {
    int status = sqlite3_step(stmt_);

    if (status == SQLITE_ROW) { has_row_ = true; return {}; }
    if (status == SQLITE_DONE) { has_row_ = false; return {}; }

    int extended_status = sqlite3_errcode(sqlite3_db_handle(stmt_));
    std::string msg = sqlite3_errmsg(sqlite3_db_handle(stmt_));

    if (extended_status == SQLITE_CONSTRAINT) {
        return std::unexpected(Error{ Error::Type::Conflict, "Conflict: " + msg });
    }

    return std::unexpected(database(msg));
}

int SqliteStatement::columnInt(int col) {
    return sqlite3_column_int(stmt_, col);
}


std::string SqliteStatement::columnText(int col) {
    const auto* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt_, col)); 
    return text ? std::string(text) : std::string("");
}

void SqliteStatement::reset() {
    sqlite3_reset(stmt_);
    sqlite3_clear_bindings(stmt_);
    has_row_ = false;
}

bool SqliteStatement::hasRow() {
    return has_row_;
}
