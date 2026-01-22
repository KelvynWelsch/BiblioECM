#include "AddItem.h"
#include <iostream>
#include "../terminal/InputUtils.h"


void addItem(LibService& serv, BookId& id) {
    std::cout << "Quantos exemplares você gostaria de adicionar? ";
	std::string s = lerTexto();
    bool isValid = true;
    for (int i = 0; i < s.size(); i++) {
        isValid = isValid && std::isdigit(static_cast<unsigned char>(s[i]));
        if (!isValid) {
            std::cout << "Caracteres inválidos inseridos! \n" << std::endl;
            addItem(serv, id);
            break;
        }
    }
    if (isValid) {
        int n = std::stoi(s);
        auto result = serv.adicionarItem(id, n);
        if (!result) {
            std::cout << result.error().message << std::endl;
        }
        else {
            std::cout << "Itens adicionado com sucesso!" << std::endl;
        }
        system("pause");
    }
}
