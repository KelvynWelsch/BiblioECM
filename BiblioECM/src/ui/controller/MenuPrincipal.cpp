#include "MenuPrincipal.h"
#include "MenuOutros.h"

#include "../screens/CadastrarUsuario.h"
#include "../screens/RegistrarEmprestimo.h"
#include "../screens/RegistrarDevolucao.h"
#include "../screens/renovacao.h"
#include "../utils/exibirSelecao.h"

#include "../../devtools/main_dev.h"

#include <array>
#include <string>

#include <iostream>

void MenuPrincipal(LibService& serv) {
    std::array<std::string, 6> menuPrincipal = {
        "Registrar empréstimo",
        "Registrar devolução",
        "Renovar empréstimo",
        "Cadastrar novo usuário",
        "Outros",
        "Sair"
    };
 

    while (true) {
        clearScreen();
        std::cout << "Biblioteca Edvaldo Cardoso Mattos\nPrimeira Igreja Batista em Taboão da Serra\n\n";
        int escolha = exibirSelecao(menuPrincipal);

        switch (escolha) {
        case 0:
            registrarEmprestimo(serv);
            break;
        case 1:
            registrarDevolucao(serv);
            break;
        case 2:
            renovacao(serv);
            break;
        case 3:
            cadastrarUsuario(serv);
            break;
        case 4:
            menuOutros(serv);
            break;
        case -1:
            main_dev(serv);
            break;
        case 5:
            return; // encerra o programa
        }
    }
}

