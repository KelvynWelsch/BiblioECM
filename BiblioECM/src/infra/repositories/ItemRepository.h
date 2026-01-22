#pragma once

#include "../../core/entities/Book.h"
#include "../../core/entities/Item.h"
#include "../../core/errors/errors.h"
#include "../db/database.h"

class ItemRepository {
private:
    Database& db;
public:
    explicit ItemRepository(Database& db);
    ECM::Result salvar(const Book& u, int past, int n); 
    ECM::Expected<int> maxId(const BookId& id);
    ECM::Result buscarId(const ItemId& id);
    ECM::Expected<int> quantosDisponiveis(const BookId& book);
    ECM::Expected<int> quantosLivros(const BookId& book);
};


