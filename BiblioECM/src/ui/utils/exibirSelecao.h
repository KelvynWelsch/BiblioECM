#pragma once

#include <vector>
#include <array>
#include <string>

#include "../terminal/terminal.h"
#include <iostream>



int exibirSelecao(std::string_view caput, const std::vector<std::string>& opcoes, std::string_view bar);

template <std::size_t N>
int exibirSelecao(std::array<std::string, N>& opcoes) {
    int selecionada = 0;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    while (true) {
        clearFromPosition(hOut, csbi);

        for (int i = 0; i < opcoes.size(); i++) {
            if (i == selecionada)
                std::cout << "\033[47;30m> " << opcoes[i] << " \033[0m\n";
            else
                std::cout << "  " << opcoes[i] << "\n";
        }

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            if (ch == 72 && selecionada > 0) selecionada--;
            if (ch == 80 && selecionada < opcoes.size() - 1) selecionada++;
            if (ch == 59) {
                return -1;
            }
        }
        else if (ch == 13) {
            return selecionada;
        }
    }
}

template <std::size_t N>
int exibirSelecao(const std::string_view& caput, std::array<std::string, N>& opcoes) {
    int selecionada = 0;

    while (true) {
        clearScreen();
        std::cout << caput << '\n' << std::endl;

        for (int i = 0; i < opcoes.size(); i++) {
            if (i == selecionada)
                std::cout << "\033[47;30m> " << opcoes[i] << " \033[0m\n";
            else
                std::cout << "  " << opcoes[i] << "\n";
        }

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            if (ch == 72 && selecionada > 0) selecionada--;
            if (ch == 80 && selecionada < opcoes.size() - 1) selecionada++;
            if (ch == 59) {
                return -1;
            }
        }
        else if (ch == 13) {
            return selecionada;
        }
    }
}

