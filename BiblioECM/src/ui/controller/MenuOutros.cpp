#include "MenuOutros.h"
#include "../utils/exibirSelecao.h"
#include "../screens/CadastrarLivro.h"
#include "../screens/BuscarUsuario.h"
#include "../screens/BuscaLivro.h"
#include "../screens/BuscarReservas.h"

void menuOutros(LibService& service){
    
    std::array<std::string, 5> opcoes = {
        "Cadastrar novo livro",
        "Buscar um livro",
        "Buscar usuário",
        "Buscar reservas",
        "Voltar"
    };


    while (true) {
        clearScreen();
        std::cout << "Biblioteca Edvaldo Cardoso Mattos\nPrimeira Igreja Batista em Taboão da Serra\n\n";
        int escolha = exibirSelecao(opcoes);

        switch (escolha) {
        case 0:
            cadastrarLivro(service);
            break;
        case 1:
            BuscaLivro(service);
            break;
        case 2:
            BuscarUsuario(service);
            break;
        case 3:
            BuscarReservas(service);
            break;
        case 4:
            return;
        }
    }
}
