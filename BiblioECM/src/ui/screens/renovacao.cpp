#include "renovacao.h"

#include "../terminal/Terminal.h"

#include "../input/LoanInput.h"

#include "../../core/utils/TimeUtils.h"

#include <iostream>

void renovacao(LibService& serv) {
    clearScreen();
    std::cout << "===== Renovar um Empréstimo Ativo =====" << std::endl;

    ECM::Expected<LoanInput> input = lerUserEItem();

    if (!input.has_value()) {
        std::cout << "Empréstimo cancelado.\n";
        system("pause");
        return;
    }

    LoanInput loan_input = input.value();

    auto result = serv.renovar(loan_input.user, loan_input.item);

    if (!result) {
        std::cout << result.error().message << std::endl;
    }
    std::cout << "Livro renovado com sucesso!" << std::endl;
    system("pause");
    return;

}
