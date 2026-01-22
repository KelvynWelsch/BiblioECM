#include "BookRepository.h"
#include "../db/database.h"
#include <iostream> // seria interessante tirar isso daqui

BookRepository::BookRepository(Database& db) : db(db) {}

ECM::Result BookRepository::salvar(const Book& livro) {
    auto prep = db.prepare(R"(
        INSERT INTO livros (id, titulo, subtitulo, autor, assunto, isbn)
        VALUES (?, ?, ?, ?, ?, ?)
    )");

    if (!prep.has_value()) {
        return std::unexpected(prep.error()); 
    }

    auto& stmt = prep.value();
    

    stmt->bind(1, livro.id.get_Id_int());
    stmt->bind(2, livro.titulo);
    stmt->bind(3, livro.subtitulo);
    stmt->bind(4, livro.autor);
    stmt->bind(5, livro.assunto);
    stmt->bind(6, livro.isbn);

    return stmt->step();
}

ECM::Expected<Book> BookRepository::getBook(const BookId id) {
    auto prep = db.prepare(R"(
        SELECT titulo, subtitulo, autor, assunto, colecao, isbn FROM livros
        WHERE id = ?;
    )");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, id.get_Id_int());

    auto res = stmt->step();

    if (res.has_value()) {
        if (stmt->hasRow()) {
            std::string titulo = stmt->columnText(0);
            std::string subtitulo = stmt->columnText(1);
            std::string autor = stmt->columnText(2);
            std::string assunto = stmt->columnText(3);
            std::string colecao = stmt->columnText(4);
            Book b = Book(id, titulo, subtitulo, autor, assunto, colecao, 1);
            return b;
        }
        return std::unexpected(notFound("Livro não encontrado!"));
    }

    return std::unexpected(res.error());
}

