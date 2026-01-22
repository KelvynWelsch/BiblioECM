#include "CadastrarUsuario.h"
#include "../terminal/Terminal.h"
#include "../terminal/InputUtils.h"
#include "../forms/Form.h"

#include "../../core/entities/User.h"

#include <iostream>
#include <expected>


void cadastrarUsuario(LibService& serv) {

    Form form("Cadastro de Usuário");

    form.add(std::make_unique<Field<UserId>>("ID do usuario", UserId::create));

    form.add(std::make_unique<Field<std::string>>(
        "Nome",
        [](const std::string& input) -> ECM::Expected<std::string> {
            if (!input.empty()) {
                return input;
            }
            return std::unexpected(validation("Nome não pode ser vazio!"));
        }));

    form.add(std::make_unique<Field<Email>>("E-mail", Email::create));

    form.add(std::make_unique<Field<std::string>>(
        "Telefone",
        [](const std::string& input) -> ECM::Expected<std::string> {
            if (!input.empty()) {
                return input;
            }
            return std::unexpected(validation("Nome não pode ser vazio!"));
        }
    ));

    if (!form.run()) return;

    auto idTest = UserId::create(form.get(0));
    auto emailTest = Email::create(form.get(2));

    User u(
        idTest.value(),
        form.get(1),
        emailTest.value(),
        form.get(3)
    );

    auto res = serv.salvarUsuario(u);
    if (res) {
        std::cout << "\nUsuário cadastrado com sucesso!\n";
    }
    else {
        std::cout << "\nErro ao cadastrar usuário: " << res.error().message << std::endl;
    }

    system("pause");
}

