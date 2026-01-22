#include "LoanInput.h"
#include "../terminal/InputUtils.h"

#include <iostream>


ECM::Expected<LoanInput> lerUserEItem() {
    std::string inputUser;
    std::string inputItem;
    std::cout << "Escaneie ou digite o ID do usuário: ";
    inputUser = lerTexto();

    // o comportamente esperado é que o usuário do programa insira primeiro o ID do usuário da biblioteca
    // e somente depois o ID do item, mas atualmente essa função tenta ser inteligente e captar quando a 
    // ordem foi trocada
    // TODO: acrescentar um método na LibService que checa os requisitos para um usuário pegar um livro 
    // emprestado e colocar aqui, de forma que o sistem nem peça o ID do item
    auto userTest = UserId::create(inputUser);
    auto itemTest = ItemId::create(inputUser);

    if (!userTest.has_value()) {
        if (!itemTest.has_value()) {
            std::cout << userTest.error().message << std::endl;
            return std::unexpected(userTest.error());
        }
        else {
            std::cout << "Você digitou o ID do livro!\n";
            std::cout << "Agora, escaneie ou digite o ID do usuário: ";
            inputUser = lerTexto();
            userTest = UserId::create(inputUser);
            if (!userTest.has_value()) {
                std::cout << userTest.error().message << std::endl;
                return std::unexpected(userTest.error());
            }
        }
    }
    else {
        std::cout << "Agora, escaneie ou digite o ID do livro: ";
        inputItem = lerTexto();
        itemTest = ItemId::create(inputItem);
        if (!itemTest.has_value()) {
            std::cout << itemTest.error().message << std::endl;
            return std::unexpected(itemTest.error());
        }
    }

    return LoanInput{ userTest.value(), itemTest.value() };
}
