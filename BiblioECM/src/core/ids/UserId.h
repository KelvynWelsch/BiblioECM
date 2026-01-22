#pragma once
#include "../errors/errors.h"
#include <string>
#include <string_view>

#define USER_ID_SIZE 5
#define USER_PREFIX 'M'

class UserId {
private:
    int id;
    UserId(int id) : id(id) {}
    UserId(const std::string& id_) {
        id = std::stoi(id_); //está protegido por validação. TODO: talvez melhorar isso depois
    }
public:
    static UserId fromDatabase(int raw) {
        return UserId(raw);
    }
    static ECM::Expected<UserId> create(std::string_view s) {
        if (s.empty()) {
            return std::unexpected(validation("Id vazio!"));
        }
        int startpoint = 0;
        size_t size = USER_ID_SIZE;
        if (s[0] == USER_PREFIX) {
            if (s.size() == 1) {
                return std::unexpected(validation("Id vazio!"));
            }
            startpoint = 1;
            size++;
        }
        if (s.size() > size) {
            return std::unexpected(validation("Id muito grande!"));
        }
        for (int i = startpoint; i < s.size(); i++) {
            if (!std::isdigit(static_cast<unsigned char>(s[i]))) {
                return std::unexpected(validation("Foram inseridos caracteres inválidos!"));
            }
        }
        std::string id_(s.substr(startpoint));
        return UserId(id_);
    }
    int get_Id_int() const { return id; };
    std::string get_Id_raw() const {
        std::string s = std::to_string(id);
        return s;
    }
    std::string get_Id_format() const {
        std::string s = std::to_string(id);
        size_t lack = USER_ID_SIZE - 1 - s.size();
        if (lack > 0) {
            for (size_t i = 0; i < lack; i++) {
                s = "0" + s;
            }
        }
        return "M" + s;
    };
};


