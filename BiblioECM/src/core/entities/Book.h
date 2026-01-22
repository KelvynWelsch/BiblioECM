#pragma once
#include <string>
#include "../ids/BookId.h"

class Book {
public:
    BookId id;
    std::string titulo;
    std::string subtitulo;
    std::string autor;
    std::string assunto;
    std::string colecao;
    std::string isbn;
    int copias;

    Book(BookId id,
        std::string titulo,
        std::string subtitulo,
        std::string autor,
        std::string assunto,
        std::string colecao,
        int copias)
        : id(id),
        titulo(std::move(titulo)),
        subtitulo(std::move(subtitulo)),
        autor(std::move(autor)),
        assunto(std::move(assunto)),
        colecao(std::move(colecao)),
        copias(copias){
    }

   static std::string get_Item(int i) {
        if (i > 0 && i < 10) {
            return "0" + std::to_string(i);
        }
        else {
            return std::to_string(i);
        }
    }
};

