#pragma once
#include "database.h"

class dbInitializer {
public:
    static ECM::Result criarTabelas(Database& db);
private:
    dbInitializer() = delete;
};


//int criarTabela(sqlite3* db, const char* sql, const char* nome);
//void inicializarBanco(Database& db);


