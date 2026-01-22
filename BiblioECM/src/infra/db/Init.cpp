#include "init.h"


 ECM::Result dbInitializer::criarTabelas(Database& db) {


    ECM::Result status_membros = db.exec(R"(
        CREATE TABLE IF NOT EXISTS membros(
            id TEXT PRIMARY KEY,
            nome TEXT NOT NULL,
            email TEXT,
            tel TEXT,
            role INTEGER DEFAULT 1,
            status INTEGER DEFAULT 1,
            blocked_until TEXT DEFAULT NULL,
            created_at TEXT DEFAULT CURRENT_TIMESTAMP
        );
      )");

    ECM::Result status_livros = db.exec(R"(
        CREATE TABLE IF NOT EXISTS livros(
            id INTEGER PRIMARY KEY, 
            titulo TEXT NOT NULL,
            subtitulo TEXT,
            autor TEXT,
            assunto TEXT,
            colecao TEXT,
            isbn TEXT,
            created_at TEXT DEFAULT CURRENT_TIMESTAMP
         ); 
       )");

    ECM::Result status_itens = db.exec(R"(
        CREATE TABLE IF NOT EXISTS itens(
            id TEXT PRIMARY KEY,
            book_id INTEGER NOT NULL,
            FOREIGN KEY (book_id) REFERENCES livros(id)
        );
    )");

    ECM::Result status_loans = db.exec(R"(
        CREATE TABLE IF NOT EXISTS loans(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            item_id TEXT NOT NULL,
            user_id TEXT NOT NULL,
            loaned_at TEXT DEFAULT CURRENT_TIMESTAMP,
            due_at TEXT NOT NULL,
            n_Renov INTEGER DEFAULT 0,
            returned_at TEXT,
            status INTEGER NOT NULL DEFAULT 1,
            FOREIGN KEY(item_id) REFERENCES itens(id), 
            FOREIGN KEY(user_id) REFERENCES membros(id)
        ); 
    )");

    ECM::Result status_reservas = db.exec(R"(
        CREATE TABLE IF NOT EXISTS reservas (
            id INTEGER PRIMARY KEY,
            user_id INTEGER NOT NULL,
            book_id INTEGER NOT NULL,
            data_reserva TEXT NOT NULL,
            status INTEGER NOT NULL,
            FOREIGN KEY(user_id) REFERENCES membros(id),
            FOREIGN KEY(book_id) REFERENCES livros(id)
       );
    )");

    bool has_error = false;
    Error erro = database("Erro ao criar as tabelas: ");

    if (!status_membros.has_value()) {
        has_error = true;
        erro = addContext(erro, "membros (" + (status_membros.error().message) + ");");
    }
    if (!status_livros.has_value()) {
        has_error = true;
        erro = addContext(erro, "livros (" + (status_membros.error().message) + ");");
    }
    if (!status_itens.has_value()) {
        has_error = true;
        erro = addContext(erro, "itens (" + (status_membros.error().message) + ");");
    }
    if (!status_loans.has_value()) {
        has_error = true;
        erro = addContext(erro, "loans (" + (status_membros.error().message) + ");");
    }
    if (!status_reservas.has_value()) {
        has_error = true;
        erro = addContext(erro, "reservas (" + (status_membros.error().message) + ");");
    }
    if (has_error) {
        return std::unexpected(erro);
    }
    return {};
}

