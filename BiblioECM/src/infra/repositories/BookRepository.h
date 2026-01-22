#pragma once

#include "../../core/entities/Book.h"
#include "../db/database.h"
#include "../../core/errors/errors.h"

class BookRepository {
private:
    Database& db;
public:
    explicit BookRepository(Database& db);
    ECM::Result salvar(const Book& u); 
    ECM::Expected<Book> getBook(const BookId id);
};
 
