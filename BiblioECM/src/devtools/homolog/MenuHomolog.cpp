#include "MenuHomolog.h"

#include "../../ui/controller/MenuOutros.h"

#include "../../ui/screens/CadastrarUsuario.h"
#include "../../ui/screens/RegistrarEmprestimo.h"
#include "../../ui/screens/RegistrarDevolucao.h"
#include "../../ui/screens/renovacao.h"
#include "../../ui/utils/exibirSelecao.h"

#include "../database_feed.h"

#include <string>

#include <iostream>

void MenuHomolog(LibService& serv) {
    std::array<std::string, 7> menuPrincipal = {
        "Registrar empréstimo",
        "Registrar devolução",
        "Renovar empréstimo",
        "Cadastrar novo usuário",
        "Alimentar banco de dados",
        "Outros",
        "Sair"
    };

    
    while (true) {
        clearScreen();
        std::cout << "=================== Ambiente de Homologação ====================\n\n";

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
            database_feed(serv);
            break;
        case 5:
            clearScreen();
            std::cout << "=================== Ambiente de Homologação ====================\n\n";
            menuOutros(serv);
            break;
        case 6:
            clearScreen();
            std::cout << "=================== Ambiente de Dev ====================\n\n";
            return;
        }
    }
}


