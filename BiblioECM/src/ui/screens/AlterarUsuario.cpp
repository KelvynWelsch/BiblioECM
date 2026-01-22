#include "AlterarUsuario.h"
#include "../forms/Form.h"
#include <iostream>

void AlterarUsuario(LibService& serv, User u){

    auto nomeField = std::make_unique<Field<std::string>>(
        "Nome",
        [](const std::string& input) -> ECM::Expected<std::string> {
            if (!input.empty()) {
                return input;
            }
            return std::unexpected(validation("Nome não pode ser vazio!"));
        });

    auto emailField = std::make_unique<Field<Email>>("E-mail", Email::create);

    auto telField = std::make_unique<Field<std::string>>(
        "Telefone",
        [](const std::string& input) -> ECM::Expected<std::string> {
            if (!input.empty()) {
                return input;
            }
            return std::unexpected(validation("Nome não pode ser vazio!"));
        }
    );

    
    nomeField->set(u.nome, u.nome);
    emailField->set(u.email.get(), u.email);
    telField->set(u.tel, u.tel);

    auto* nomePtr = nomeField.get();
    auto* emailPtr = emailField.get();
    auto* telPtr = telField.get();

    Form form("Alterar Usuário");

    std::cout << "ID do usuário: " << u.id.get_Id_format() << std::endl;
    form.add(std::move(nomeField));
    form.add(std::move(emailField));
    form.add(std::move(telField));

    if (!form.run()) return;

    User atualizado(
        u.id,
        nomePtr->get(),
        emailPtr->get(),
        telPtr->get()
    );

    auto res_update = serv.alterarUsuario(atualizado);
    if (!res_update) {
        std::cout << res_update.error().message;
    }
    else {
        std::cout << "Usuário atualizado com sucesso!" << std::endl;
    }
    system("pause");
    return;
}
