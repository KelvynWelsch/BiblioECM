#include "Form.h"
#include "../terminal/Terminal.h"
#include "../terminal/InputUtils.h"
#include "../utils/exibirSelecao.h"
#include <iostream>

void Form::add(std::unique_ptr<IField> field) {
    fields.push_back(std::move(field));
}

const std::string& Form::get(int i) const {
    return fields[i]->value();
}

bool Form::run() {
    while (true) {
        clearScreen();
        draw();

        std::string input = lerTexto();

        if (input == "$cancelar") return false;

        if (input == "$voltar") {
            if (current > 0) current--;
            continue;
        }

        std::string error;
        if (!fields[current]->handleInput(input, error)) {
            showError(error);
            continue;
        }

        if (current == fields.size() - 1) {
            if (confirm()) return true;
        }
        else {
            current++;
        }
    }
}

void Form::draw() {
    std::cout << "===== " << title << " =====\n\n";

    for (int i = 0; i < fields.size(); ++i) {
        std::cout << (i == current ? "\033[47;30m> " : "  ");
        std::cout << fields[i]->label() << ": " << fields[i]->value() << "\033[0m\n";
    }

    std::cout << "\nComandos: $voltar  $cancelar\n\n";
    std::cout << fields[current]->label() << ": ";
}

void Form::showError(const std::string& msg) const {
    std::cout << "\nERRO: " << msg << "\n";
    system("pause");
}

bool Form::confirm() const {
    clearScreen();
    std::cout << "========== Confirmar dados ==========\n\n";

    for (const auto& f : fields) {
        std::cout << f->label() + ": " + f->value() + "\n";
    }

    std::array<std::string, 2> opcoes = { "Sim", "Não" };
    std::cout << "\nConfirmar?\n";
    int sel = exibirSelecao(opcoes);
    return (sel == 0);
}

