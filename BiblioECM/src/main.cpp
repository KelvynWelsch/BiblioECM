#include "application/LibService.h"

#include "infra/db/sqlite_database.h"
#include "infra/db/init.h"
#include "infra/db/update.h"
#include "infra/repositories/UserRepository.h"
#include "infra/repositories/BookRepository.h"
#include "infra/repositories/ItemRepository.h"
#include "infra/repositories/LoanRepository.h"
#include "infra/repositories/SearchRepository.h"

#include "ui/terminal/configurarTerminal.h"
#include "ui/utils/exibirSelecao.h"
#include "ui/controller/MenuPrincipal.h"

#include <iostream>



int main() {
    configurarTerminal();

    SqliteDatabase db("biblioteca.db");

    auto init = dbInitializer::criarTabelas(db);
   
    if (!init) {
        std::cout << "Erro na criação de tabelas: " << init.error().message << std::endl;
        system("pause");
        return 1;
    }
     
    auto up = updateDb(db);

    if (!up) {
        std::cout << "Erro na atualização de tabelas: " << up.error().message << std::endl;
        system("pause");
        return 1;
    }

    UserRepository users(db);
    BookRepository books(db);
    ItemRepository items(db);
    LoanRepository loans(db);
    SearchRepository searches(db);
    ReservaRepository reserva(db);

    LibService serv(db, users, books, items, loans, searches, reserva);

    MenuPrincipal(serv);

}

