#include "MostrarUsuario.h"
#include "AlterarUsuario.h"
#include "../utils/Print.h"
#include "../utils/exibirSelecao.h"
#include <windows.h>
#include <iostream>

void MostrarUsuario(LibService& serv, UserId& id) {
	clearScreen();
	std::cout << "=========== Detalhes de Usuário ===========\n\n";
	auto u = serv.idToUser(id);
	if (!u) {
		std::cout << u.error().message << std::endl;
	}
	std::string caput = ECM::UserToString(u.value());
	std::array<std::string, 2> escolha = { "Alterar cadastro", "Voltar" };
	std::cout << caput + "\n\n O que você deseja fazer?" << std::endl;
	int input = exibirSelecao(escolha);
	switch (input) {
	case 0:{
		AlterarUsuario(serv, u.value());
		break;
	}
	default:
		return;
	}
	return;
}
