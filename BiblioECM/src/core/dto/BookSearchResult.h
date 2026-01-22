#pragma once

#include "../ids/BookId.h"
#include <string>

struct BookSearchResult {
    BookId book_id;
    std::string title;
    std::string subtitle;
    std::string author;

    int total_items;
    int available;
    int loaned;
};

