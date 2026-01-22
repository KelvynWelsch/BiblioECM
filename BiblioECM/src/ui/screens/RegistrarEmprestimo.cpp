#include "RegistrarEmprestimo.h"

#include "../terminal/Terminal.h"

#include <string>
#include <expected>
#include <chrono>

#include <iostream>

#include "../../core/ids/UserId.h"
#include "../../core/ids/ItemId.h"

#include "../../core/entities/Loan.h"


#include "../../core/utils/TimeUtils.h"

#include "../input/LoanInput.h"

void registrarEmprestimo(LibService& serv) {
    clearScreen();
    std::cout << "========== Registrar Novo Empréstimo ==========\n\n";

    ECM::Expected<LoanInput> input = lerUserEItem();

    if (!input.has_value()) {
        std::cout << "ERRO! Empréstimo cancelado.\n";
        system("pause");
        return;
    }

    LoanInput loan_input = input.value();

    auto result = serv.emprestar(loan_input.user, loan_input.item);

    if (!result.has_value()) {
        std::cout << std::endl;
        std::cout << result.error().message << std::endl;
        system("pause");
        return;
    }
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "Empréstimo registrado com sucesso!" << std::endl;
        std::cout << "A data de devolução é " << timeToStringUser(result.value().due_at) << "." << std::endl; 
        system("pause");
}
