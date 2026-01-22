#include "Reservar.h"
#include "../terminal/InputUtils.h"
#include "../../core/utils/TimeUtils.h"
#include <iostream>
#include "windows.h"

void Reservar(LibService& serv, const BookId& book) {
    std::string inputUser;
    std::string inputItem;
    std::cout << "Escaneie ou digite o ID do usuário: ";
    inputUser = lerTexto();
    // atualmente a função de prever disponbilidade apenas é mostrada na tela após a reserva ter sido efetivada
    // (nesse arquivo, ali embaixo), mas como o cálculo da previsão depende de quantas reservas tem e, como 
    // futuramente pode ser interessante  mostrar a previsão sem ter feito reserva, em outra tela, atualmente
    // o cálculo da previsão é implementado tomando como base que a reserva não foi feita ainda, por isso 
    // a função precisa ser chamada aqui em cima
    auto dataTest = serv.preverDisponibilidade(book); 
    if (!dataTest && (dataTest.error().type == Error::Type::Conflict)) {
        std::cout << dataTest.error().message << std::endl;
        std::cout << std::endl;
        system("pause");
        return;
    }
    auto userTest = UserId::create(inputUser);

    if (!userTest.has_value()) {
        std::cout << userTest.error().message << std::endl;
        Reservar(serv, book);
        return;
    }
    std::cout << std::endl;
    auto res = serv.reservar(userTest.value(), book);
    if (!res) {
        std::cout << res.error().message << std::endl;

    }
    else {
        std::cout << "Reserva efetivada com sucesso!" << std::endl;
        
        if (!dataTest) {
            std::cout << dataTest.error().message << std::endl;
        }
        else {
            std::cout << "A data prevista em que o livro se encontrará disponível é: " << timeToStringUser(dataTest.value()) << std::endl;
        }
    }
    std::cout << std::endl;
        system("pause");
        return;
}
