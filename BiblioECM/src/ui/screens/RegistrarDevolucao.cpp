#include "RegistrarDevolucao.h"

#include "../terminal/Terminal.h"

#include "../input/LoanInput.h"


#include <iostream>

void registrarDevolucao(LibService& serv) {
    clearScreen();
    std::cout << "===== Registrar Devolução =====" << std::endl;

    ECM::Expected<LoanInput> input = lerUserEItem();

    if (!input.has_value()) {
        std::cout << "Devolução cancelada.\n";
        system("pause");
        return;
    }

    LoanInput loan_input = input.value();

    auto result = serv.devolver(loan_input.user, loan_input.item);


    if (!result && (result.error().type != Error::Type::Reserved)) {
        std::cout << result.error().message << std::endl;
    }
    std::cout << "Livro devolvido com sucesso!" << std::endl;
    std::cout << std::endl;
    if (!result && result.error().type == Error::Type::Reserved) {
        std::cout << result.error().message << std::endl;
    }
    system("pause");
    return;
}



