#pragma once
#include "../ids/ItemId.h"

/* Uma possibilidade de adapatação é acrescentar um enum class com o estado (disponível, emprestado) e alterar
a tabela/repositório de itens de acordo. Isso é uma ótima escolha pensando em performance, mas sacrifica 
a consistência: é importante tomar cuidado para atualizar o status sempre que chamar um método da LibService
como emprestar, devolver, etc, sob o risco de criar dados inconsistentes. É necessário fazer isso se o 
sistema crescer muito */

class Item {
public:
    ItemId id;
    BookId book_id;
};

