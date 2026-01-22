#pragma once
#include "../errors/errors.h"
#include <string>
#include <string_view>

#define BOOK_ID_SIZE 5
#define BOOK_PREFIX 'L'

class BookId {
private:
    int id;
    BookId(int id) : id(id) {};
    BookId(const std::string& id_) {
        id = std::stoi(id_); //está protegido por validação, TODO: talvez melhorar isso depois
    }
public:
    static BookId fromDatabase(int raw) {
        return BookId(raw);
    }
    static ECM::Expected<BookId> create(std::string s) {
        if (s.empty()) {
            return std::unexpected(validation("Id vazio!"));
        }
        int startpoint = 0;
        int size_max = BOOK_ID_SIZE + 3;
        if (s[0] == BOOK_PREFIX) {
            if (s.size() == 1) {
                return std::unexpected(validation("Id vazio!"));
            }
            startpoint = 1;
            size_max++;
        }
        if (s.size() > size_max) {
            return std::unexpected(validation("Id muito grande!"));
        }
        auto pos = s.find('-');
        if (pos != std::string::npos) {
            s.erase(pos); // apaga de pos até o fim
        }
        for (int i = startpoint; i < s.size(); i++) {
            if (!std::isdigit(static_cast<unsigned char>(s[i]))) {
                return std::unexpected(validation("Foram inseridos caracteres inválidos!")); 
            }
        }
        std::string id_(s.substr(startpoint));
        return BookId(id_);
    }
    int get_Id_int() const { return id; };
    std::string get_Id_raw() const {
        std::string s = std::to_string(id);
        return s;
    }
    std::string get_Id_format() const {
        std::string s = std::to_string(id);
        int lack = BOOK_ID_SIZE - 1 - s.size();
        if (lack > 0) {
            for (int i = 0; i < lack; i++) {
                s = "0" + s;
            }
        }
        return "L" + s;
    };
};
