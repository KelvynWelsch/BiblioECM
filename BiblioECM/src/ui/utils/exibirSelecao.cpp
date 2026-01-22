#include "exibirSelecao.h"



int exibirSelecao(std::string_view caput, const std::vector<std::string>& opcoes, std::string_view bar) {
    int selecionada = 0;
    while (true) {
        clearScreen();
        std::cout << caput << std::endl;

        for (int i = 0; i < opcoes.size(); i++) {
            if (i == selecionada)
                std::cout << "\033[47;30m> " << opcoes[i] << " \033[0m\n" << bar << "\n";
            else
                std::cout << "  " << opcoes[i] << "\n" << bar << "\n";
        }

        int ch = _getch();
        if (ch == 224) {
            ch = _getch();
            if (ch == 72 && selecionada > 0) selecionada--;
            if (ch == 80 && selecionada < opcoes.size() - 1) selecionada++;
        }
        else if (ch == 13) {
            return selecionada;
        }
    }
}
