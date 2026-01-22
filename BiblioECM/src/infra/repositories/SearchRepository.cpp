#include "SearchRepository.h"

SearchRepository::SearchRepository(Database& db) : db(db) {}

ECM::Expected<std::vector<BookSearchResult>> 
	SearchRepository::searchBooks(std::string_view s, BookSearchField option) {
	std::string query = R"(SELECT 
  b.id,
  b.titulo,
  b.subtitulo,
  b.autor,

  COUNT(i.id) AS total_items,

  SUM(CASE 
        WHEN l.id IS NULL THEN 1 
        ELSE 0 
      END) AS available,

  SUM(CASE 
        WHEN l.id IS NOT NULL THEN 1 
        ELSE 0 
      END) AS loaned

FROM livros b
JOIN itens i 
  ON i.book_id = b.id

LEFT JOIN loans l 
  ON l.item_id = i.id 
 AND l.returned_at IS NULL

WHERE )";

    switch (option) {
    case BookSearchField::Title:
        query += " b.titulo LIKE ? ";
        break;
    case BookSearchField::Author:
        query += " b.autor LIKE ? COLLATE NOCASE";
        break;
    default:
        query += " (b.titulo LIKE ? COLLATE NOCASE OR b.autor LIKE ? COLLATE NOCASE) ";
        break;
    }

    query += "GROUP BY b.id, b.titulo, b.autor;";

    auto prep = db.prepare(query);

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    std::string pattern = "%" + std::string(s) + "%";

    if ((option == BookSearchField::Author)||(option == BookSearchField::Title)) {
        stmt->bind(1, pattern);
    }
    else {
        stmt->bind(1, pattern);
        stmt->bind(2, pattern);
    }

    std::vector<BookSearchResult> results;

    while (true) {
        auto res = stmt->step();
        if (!res) return std::unexpected(res.error());
        if (!stmt->hasRow()) break;

        BookSearchResult dto{
            .book_id = BookId::fromDatabase(stmt->columnInt(0)),
            .title = stmt->columnText(1),
            .subtitle = stmt->columnText(2),
            .author = stmt->columnText(3),
            .total_items = stmt->columnInt(4),
            .available = stmt->columnInt(5),
            .loaned = stmt->columnInt(6)
        };

        results.push_back(std::move(dto));
    }

    return results;

}

ECM::Expected<std::vector<UserSearchResult>>
SearchRepository::searchUser(std::string_view term) {
    std::string query = R"(SELECT 
  id,
  nome

FROM membros 

WHERE nome LIKE ? COLLATE NOCASE)";


    auto prep = db.prepare(query);

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    std::string pattern = "%" + std::string(term) + "%";

    stmt->bind(1, pattern);

    std::vector<UserSearchResult> results;

    while (true) {
        auto res = stmt->step();
        if (!res) return std::unexpected(res.error());
        if (!stmt->hasRow()) break;

        UserSearchResult dto{
            .user_id = UserId::fromDatabase(stmt->columnInt(0)),
            .nome = stmt->columnText(1)
        };

        results.push_back(std::move(dto));
    }

    return results;

}

ECM::Expected<std::vector<UserSearchResult>>
SearchRepository::searchUser(int term) {
    std::string query = R"(SELECT 
  id,
  nome

FROM membros 

WHERE id LIKE ?)";

    auto prep = db.prepare(query);

    if (!prep.has_value()) {
        return std::unexpected(prep.error());
    }

    auto& stmt = prep.value();

    stmt->bind(1, term);

    std::vector<UserSearchResult> results;

    while (true) {
        auto res = stmt->step();
        if (!res) return std::unexpected(res.error());
        if (!stmt->hasRow()) break;

        UserSearchResult dto{
            .user_id = UserId::fromDatabase(stmt->columnInt(0)),
            .nome = stmt->columnText(1)
        };

        results.push_back(std::move(dto));
    }

    return results;

}
