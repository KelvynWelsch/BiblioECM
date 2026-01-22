#pragma once
#include "../../core/entities/User.h"
#include "../../core/utils/TimeUtils.h"
#include <iostream>

namespace ECM {
	inline void Print(User& u) {
		std::cout << "ID: " << u.id.get_Id_format() << "\n";
		std::cout << "Nome: " << u.nome << "\n";
		std::cout << "E-mail: " << u.email.get() << "\n";
		std::cout << "Telefone: " << u.tel << "\n";
		if (u.status == UserStatus::Inativo) {
			std::cout << "Status: " << "Inativo" << "\n";
		}
		else if (u.status == UserStatus::Bloqueado) {
			std::cout << "Status: " << "Bloqueado" << "\n";
			if (u.blocked_until == std::nullopt) {
				std::cout << "Sem data de desbloqueio" << std::endl;
			}
			else {
				std::cout << "Bloqueado até: " << timeToStringUser(u.blocked_until.value()) << std::endl;
			}
		}
		else {
			std::cout << "Status: " << "Ativo" << "\n";
		}
		std::cout << std::endl;
		return;
	}

	inline std::string UserToString(User& u) {
		std::string s = "ID: " + u.id.get_Id_format() + "\n";
		s += "Nome: " + u.nome + "\n";
		s+= "E-mail: " + u.email.get() + "\n";
		s += "Telefone: " + u.tel + "\n";
		if (u.status == UserStatus::Inativo) {
			s += "Status: Inativo\n";
		}
		else if (u.status == UserStatus::Bloqueado) {
			s += "Status: Bloqueado\n";
			if (u.blocked_until == std::nullopt) {
				s += "Sem data de desbloqueio\n";
			}
			else {
				s += "Bloqueado até: " + timeToStringUser(u.blocked_until.value()) + "\n";
			}
		}
		else {
			s += "Status: Ativo\n";
		}
		return s;
	}
}

