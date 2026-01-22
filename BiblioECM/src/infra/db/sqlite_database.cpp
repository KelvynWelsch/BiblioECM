#include "sqlite_database.h"

#include "sqlite_statement.h"

#include <stdexcept>

SqliteDatabase::SqliteDatabase(const std::string& filename)
    : db_(nullptr)
{
    if (sqlite3_open(filename.c_str(), &db_) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
}

SqliteDatabase::~SqliteDatabase() {
    if (db_) sqlite3_close(db_);
}

ECM::Result SqliteDatabase::exec(std::string_view sql) { 
    if (sqlite3_exec(db_, sql.data(), nullptr, nullptr, nullptr)!= SQLITE_OK) {
        return std::unexpected(database(sqlite3_errmsg(db_)));
    }
    return {};
}

ECM::Expected<std::unique_ptr<Statement>> SqliteDatabase::prepare(const std::string& sql) {
    sqlite3_stmt* stmt_ptr = nullptr;

    // O SQLite tenta compilar a query SQL
    int rc = sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt_ptr, nullptr);

    if (rc != SQLITE_OK) {
        // Se deu erro (tabela não existe, erro de sintaxe, etc.)
        // Pegamos a mensagem de erro direto do handle do banco
        std::string erroMsg = sqlite3_errmsg(db_);
        return std::unexpected(database(erroMsg));
    }

    // Se deu certo, encapsulamos o ponteiro bruto em um unique_ptr e retornamos sucesso
    return std::make_unique<SqliteStatement>(stmt_ptr);
}

ECM::Result SqliteDatabase::begin() {
    if (sqlite3_exec(db_, "BEGIN;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        return std::unexpected(database(sqlite3_errmsg(db_)));
    }
    return {};
}

ECM::Result SqliteDatabase::commit() {
    //last_error_.clear();
    if (sqlite3_exec(db_, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        return std::unexpected(database(sqlite3_errmsg(db_)));
    }
    return {};
}

ECM::Result SqliteDatabase::rollback() {
    //last_error_.clear();
    if (sqlite3_exec(db_, "ROLLBACK;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        return std::unexpected(database(sqlite3_errmsg(db_)));
    }
    return {};
}


sqlite3* SqliteDatabase::raw() { return db_; }

