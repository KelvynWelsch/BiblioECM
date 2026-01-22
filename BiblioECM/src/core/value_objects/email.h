#pragma once

#include "../errors/errors.h"

#include <string>
#include <expected>
#include <regex>

class Email {
public:
	static ECM::Expected<Email> create(std::string s) {
		if (s.empty()) {
			return std::unexpected(validation("E-mail vazio!"));
		}
		const std::regex pattern(R"(^\S+@\S+\.\S+$)");
		if (!std::regex_match(s, pattern)) return std::unexpected(validation("E-mail inválido!"));

		return Email(s);
	}

	const std::string& get() const {
		return address;
	}

	static Email fromDatabase(std::string s) {
		return Email(s);
	}
private:
	std::string address;
	Email(const std::string& s) : address(s) {}
};

