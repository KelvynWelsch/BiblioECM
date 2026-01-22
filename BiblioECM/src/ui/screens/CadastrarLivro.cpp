#include "CadastrarLivro.h"

#include <iostream>
#include <string>

#include "../terminal/terminal.h"
#include "../terminal/InputUtils.h"
#include "../forms/Form.h"


void cadastrarLivro(LibService& service) {

    Form form("Cadastro de Livro");

    form.add(std::make_unique<Field<BookId>>("ID do livro", BookId::create));

    form.add(std::make_unique<Field<std::string>>(
        "Título",
        [](const std::string& input) -> ECM::Expected<std::string> {
            if (!input.empty()) {
                return input;
            }
            return std::unexpected(validation("Nome não pode ser vazio!"));
        }));

    form.add(std::make_unique<Field<std::string>>(
        "Subtítulo",accept_any));

    form.add(std::make_unique<Field<std::string>>(
        "Autor(es)", accept_any));

    form.add(std::make_unique<Field<std::string>>(
        "Assunto", accept_any));

    form.add(std::make_unique<Field<std::string>>(
        "Coleção", accept_any));

    form.add(std::make_unique<Field<int>>(
        "Número de cópias", int_validator));

    if (!form.run()) return;

    auto idTest = BookId::create(form.get(0));

      Book b(
        idTest.value(),
        form.get(1),
        form.get(2),
        form.get(3),
        form.get(4),
        form.get(5),
        std::stoi(form.get(6))
    );


    auto res = service.cadastrarLivro(b);
    if (res) {
        std::cout << "\nLivro cadastrado com sucesso!\n";
    }
    else {
        std::cout << "\nErro ao cadastrar livro: " << res.error().message << std::endl;
    }

    system("pause");
   
    
}
