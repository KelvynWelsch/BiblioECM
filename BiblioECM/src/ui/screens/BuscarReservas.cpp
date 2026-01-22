#include "BuscarReservas.h"
#include "../terminal/InputUtils.h"
#include <iostream>
#include "../utils/exibirSelecao.h"

void BuscarReservas(LibService& serv) {
    clearScreen();
    std::string inputUser;
    std::string inputItem;
    std::cout << "Escaneie ou digite o ID do usuário: ";
    inputUser = lerTexto();

    auto userTest = UserId::create(inputUser);

    if (!userTest.has_value()) {
        std::cout << userTest.error().message << std::endl;
        BuscarReservas(serv);
        return;
    }

    auto result = serv.buscarReservas(userTest.value());
    if (!result) {
        std::cout << result.error().message << std::endl;
        system("pause");
        return;
    }
    auto& res = result.value();
    std::cout << "Foram encontrados " << res.size() << " resultados: \n\n";
	const int reserva_size = 1;
	std::vector<std::string> out;
	for (int i = 0; i < res.size(); i++) {
		out.push_back(res[i].nome_livro);
        std::cout << i +1 << ". " << res[i].nome_livro << "\n";
	}
    std::cout << "\n\nO que você deseja fazer?\n" << std::endl;
    std::array<std::string, 2> options = {"Cancelar uma reserva", "Voltar"};
    int select = exibirSelecao(options);
    switch (select) {
    case 0:{
            clearScreen();
            int choice = exibirSelecao("De qual livro é a reserva que você gostaria de cancelar?\n", out, "");
            std::cout << "\n\nTem certeza que gostaria de cancelar a reserva do livro \"" << out[choice] << "\"?\n";
            std::array<std::string, 2> yorn = { "Sim", "Não" };
            int selecionada = exibirSelecao(yorn);
            if (!selecionada) {
                auto cancel = serv.cancelarReserva(res[choice].id_reserva);
                if (!cancel) {
                    std::cout << cancel.error().message << std::endl;

                }
                else {
                    std::cout << "Reserva cancelada com sucesso!" << std::endl;
                }

            }
            system("pause");
            return;
    }
    default:
        return;
    }

}
