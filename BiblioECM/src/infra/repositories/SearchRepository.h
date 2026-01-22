#pragma once

#include "../../core/errors/errors.h"
#include "../../core/dto/BookSearchResult.h"
#include "../../core/dto/UserSearchResult.h"
#include "../../core/BookSearchField.h"
#include "../../core/UserSearchField.h"
#include "../db/database.h"

class SearchRepository {
public:
	SearchRepository(Database& db);
	ECM::Expected<std::vector<BookSearchResult>>
		searchBooks(std::string_view term, BookSearchField field);
	ECM::Expected<std::vector<UserSearchResult>>
		searchUser(std::string_view term);
	ECM::Expected<std::vector<UserSearchResult>>
		searchUser(int id);
private:
	Database& db;
};
