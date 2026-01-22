#include "database_feed.h"
#include "../infra/feed/feed.h"
#include <iostream>


void database_feed(LibService& serv) {
    auto result = db_feed(serv);
    if (!result) {
        std::cout << "Erro ao alimentar o banco de dados: " << result.error().message << std::endl;
    }
    else {
        std::cout << "Banco de dados alimentado com sucesso!" << std::endl;
    }
    system("pause");
}
