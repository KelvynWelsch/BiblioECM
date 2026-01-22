#pragma once

#include <string>
#include <string_view>
#include "BookId.h"

#define Item_ID_SIZE 8


class ItemId {
private:
    BookId book;
    int copyNumber;
    ItemId(BookId book, int copyNumber) : book(book), copyNumber(copyNumber) {};
public:
    static ECM::Expected<ItemId> create(std::string s) {
        int copyNumber;
        int size_max = Item_ID_SIZE + 3;
        if (s.empty()) {
            return std::unexpected(validation("Id vazio!"));
        }
        if (s.size() > size_max) {
            return std::unexpected(validation("Id muito grande!"));
        }
        std::size_t pos_hifen;
        bool hasHifen = false;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == '-') {
                hasHifen = true;
                pos_hifen = i;
                break;
            }
        }
        if (!hasHifen) {
            return std::unexpected(validation("Falta um hífen!"));
        }

        auto test = BookId::create(s.substr(0, pos_hifen));
        if (!test.has_value()) {
            return std::unexpected(test.error());
        }
        BookId book = test.value();
        for (int i = pos_hifen + 1; i < s.size(); i++) {
            if (!std::isdigit(static_cast<unsigned char>(s[i]))) {
                return std::unexpected(validation("Foram inserido caracteres inválidos!"));
            }
        }
        copyNumber = std::stoi(std::string(s.substr(pos_hifen + 1)));


        return ItemId(book, copyNumber);
    }

    std::string get_Id() const {
        if (copyNumber < 10) {
            return book.get_Id_format() + "-0" + std::to_string(copyNumber);
        }
        return book.get_Id_format() + "-" + std::to_string(copyNumber);
    };

    BookId getBook() const {
        return book;
    }

    int getExemp() const {
        return copyNumber;
    }

};

