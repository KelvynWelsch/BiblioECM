#include "../../application/LibService.h"

#include "../../infra/db/sqlite_database.h"
#include "../../infra/db/init.h"
#include "../../infra/repositories/UserRepository.h"
#include "../../infra/repositories/BookRepository.h"
#include "../../infra/repositories/ItemRepository.h"
#include "../../infra/repositories/LoanRepository.h"
#include "../../infra/repositories/SearchRepository.h"


#include "../../ui/utils/exibirSelecao.h"

#include "MenuHomolog.h"


#include <iostream>

void homolog() {
    SqliteDatabase db_homolog("biblioteca_homolog.db");

    auto init = dbInitializer::criarTabelas(db_homolog);

    if (!init) {
        std::cout << "Erro na criação de tabelas: " << init.error().message << std::endl;
        system("pause");
        return;
    }

    UserRepository users_homolog(db_homolog);
    BookRepository books_homolog(db_homolog);
    ItemRepository items_homolog(db_homolog);
    LoanRepository loans_homolog(db_homolog);
    SearchRepository searches_homolog(db_homolog);
    ReservaRepository reserva_homolog(db_homolog);

    LibService serv_homolog(db_homolog, users_homolog, books_homolog, items_homolog, loans_homolog, searches_homolog, reserva_homolog);

    MenuHomolog(serv_homolog);
}
