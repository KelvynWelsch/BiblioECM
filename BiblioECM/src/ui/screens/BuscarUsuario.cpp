#include "BuscarUsuario.h"

#include "../terminal/terminal.h"
#include "../utils/exibirSelecao.h"
#include "../terminal/InputUtils.h"
#include "../../core/UserSearchField.h"
#include "../../core/dto/UserSearchResult.h"
#include "../utils/criarLinha.h"
#include "MostrarUsuario.h"
#include <iostream>
#include <array>

void BuscarUsuario(LibService& serv) {
	clearScreen();
	std::cout << "========== Buscar Usuário ==========" << std::endl;
	std::cout << "Você deseja procurar pelo nome ou ID?\n\n";

	std::array<std::string, 2> opcoes = {
		"ID",
		"Nome"
	};
	int selecionada = exibirSelecao(opcoes);
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Digite a expressão que deseja buscar: ";
	std::string s = lerTexto();
	clearScreen();
	

	ECM::Expected<std::vector<UserSearchResult>> Resultado;
	ECM::Expected<UserId> idTest = std::unexpected(validation("Id não inicializado"));
	switch (selecionada) {
	case 0:
		idTest = UserId::create(s);
		if (!idTest) {
			std::cout << idTest.error().message << std::endl;
			system("pause");
			clearScreen();
			BuscarUsuario(serv);
		}
		else {
			Resultado = serv.buscarUsuarioId(idTest.value());
			break;
		}
	case 1:
		
		Resultado = serv.buscarUsuario(s);
		break;
	}
	if (!Resultado) {
		std::cout << Resultado.error().message << std::endl;
		system("pause");
		return;
	}
	clearScreen();
	auto res = Resultado.value();
	std::cout << "Foram encontrados " << res.size() << " resultados:" << std::endl;
	std::cout << std::endl;
	const int usr_size = 2;
	std::array<int, usr_size> sizes = { 6, 30 };
	std::array<std::string, usr_size> line = {
		"ID",
		"Nome"
	};
	auto labels = criarLinha(line, sizes);
	std::cout << "Resultados da busca: \n\n\n" + labels[0] + "\n" + labels[1] + "\n";
	std::string caput2;
	std::vector<std::string> out;
	for (int i = 0; i < res.size(); i++) {
		auto& usr = res[i];
		std::array<std::string, usr_size> strs = {
			usr.user_id.get_Id_format(),
			usr.nome
		};
		auto linha = criarLinha(strs, sizes);
		out.push_back(linha[0]);
		caput2 = caput2 + linha[0] + "\n" + linha[1] + "\n";
	}
	std::array<std::string, 2> escolha = { "Mostrar detalhes", "Voltar" };
	std::cout << caput2 + "\n\n O que você deseja fazer?\n\n";
	int input = exibirSelecao(escolha);
	int choice;
	switch (input) {
	case 0:
		clearScreen();
		choice = exibirSelecao("Selecione o usuário para mostrar os detalhes?\n\n\n  " + labels[0] + "\n" + labels[1], out, labels[1]);
		MostrarUsuario(serv, res[choice].user_id);
		break;
	default:
		break;
	}
}
