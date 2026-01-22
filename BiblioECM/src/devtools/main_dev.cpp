#include "../ui/utils/exibirSelecao.h"
#include "homolog/homolog.h"
#include "database_feed.h"
#include <string>
#include <iostream>

void main_dev(LibService& serv) {
    std::array<std::string, 3> menuDev = {
        "Ambiente de Homologação",
        "Resetar banco de dados",
        "Voltar"
    };
    

    while (true) {
        clearScreen();
        std::cout << "=================== Ambiente de Dev ====================\n\n";
        int escolha = exibirSelecao(menuDev);

        switch (escolha) {
        case 0:
            homolog();
            break;
        case 1:
            database_feed(serv);
            break;
        case 2:
            clearScreen();
            std::cout << "Biblioteca Edvaldo Cardoso Mattos\nPrimeira Igreja Batista em Taboão da Serra\n\n";
            return;
        }
    }
}
