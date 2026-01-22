#include "ItemRepository.h"
#define NOMINMAX
#include<iostream>
#include <windows.h>
#include <string>

ItemRepository::ItemRepository(Database& db) : db(db) {}


ECM::Result ItemRepository::buscarId(const ItemId& id) {

    auto prep = db.prepare(R"(
        SELECT id FROM itens 
        WHERE id = ?
    )");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, id.get_Id());

    
        auto res = stmt->step();
        if (!res) return std::unexpected(res.error());
        if (!stmt->hasRow()) return std::unexpected(notFound("Item não encontrado"));
        return {};
}

ECM::Result ItemRepository::salvar(const Book& b, int past, int n) {
    auto prep = db.prepare(R"(
        INSERT INTO itens (id, book_id)
        VALUES (?, ?)
    )");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    past = past + 1;

    for (int i = past; i < past + n; i++) {

        std::string itemId = b.id.get_Id_format() + "-" + Book::get_Item(i);

        stmt->bind(1, itemId);
        stmt->bind(2, b.id.get_Id_int());

        auto res = stmt->step();
        if (!res.has_value()) return res;
        stmt->reset();
    }

    return {};
}


ECM::Expected<int> ItemRepository::maxId(const BookId& id) {
    auto prep = db.prepare(R"(
        SELECT id FROM itens 
        WHERE book_id = ?
    )");

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, id.get_Id_int());

    int max_ex = 0;

    while (true) {
        auto res = stmt->step();
        if (!res) return std::unexpected(res.error());
        if (!stmt->hasRow()) break;

        auto idTest = ItemId::create(stmt->columnText(0));
        if (!idTest) {
            return std::unexpected(addContext(idTest.error(), "Não foi possível identificar o número de exemplares, um dos ids está inválido."));
        }
        max_ex = std::max(max_ex, idTest.value().getExemp());
    }

    return max_ex;
}

ECM::Expected<int> ItemRepository::quantosDisponiveis(const BookId& book) {
    std::cout << "Entra aqui" << std::endl;
    system("pause");
    auto prep = db.prepare(
        R"(SELECT COUNT(*)
        FROM itens i
        WHERE i.book_id = ?
        AND NOT EXISTS(
            SELECT 1 FROM loans l
            WHERE l.item_id = i.id
            AND l.status != 0
        );)"
     );

    if (!prep) {
        return std::unexpected(prep.error());
    }
    
    auto& stmt = prep.value();
    stmt->bind(1, book.get_Id_int());

    auto res = stmt->step();
    if (!res) {
        return std::unexpected(res.error());
    }
   
    return stmt->columnInt(0);
}


ECM::Expected<int> ItemRepository::quantosLivros(const BookId& book) {

    auto prep = db.prepare(
        R"(SELECT COUNT(*)
        FROM itens i
        WHERE i.book_id = ?;)"
    );

    if (!prep) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();
    stmt->bind(1, book.get_Id_int());


    auto res = stmt->step();
    if (!res) {
        return std::unexpected(res.error());
    }

    return stmt->columnInt(0);
}


