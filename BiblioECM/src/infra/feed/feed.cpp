#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "feed.h"

std::vector<std::string> splitTSV(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;

    while (std::getline(ss, field, '\t')) {
        fields.push_back(field);
    }

    return fields;
}

ECM::Result db_feed(LibService& serv) {
    std::ifstream file("livros.tsv"); // trate o arquivo como um fluxo de dados de entrada
    file.imbue(std::locale("")); // std::locale funciona como um conjunto de regras locais de caracters,
                                 //         separação decimal, etc

    std::string line;

    std::getline(file, line); // pula cabeçalho

    while (std::getline(file, line)) {
        auto cols = splitTSV(line);

        auto id = BookId::create(cols[0]);
        std::string titulo = cols[1];
        std::string subtitulo = cols[2];
        std::string autores = cols[3];
        std::string assunto = cols[4];
        std::string colecao = cols[5];
        int copias;
        try {
            copias = std::stoi(cols[6]);
        }
        catch (...) {
            copias = 1;
        }
            

        if (!id) {
            return std::unexpected(id.error());
        }

        Book b(id.value(), titulo, subtitulo, autores, assunto, colecao, copias);
        auto result = serv.cadastrarLivro(b);

        if (!result) {
            return std::unexpected(result.error());
        }
    }

    return {};
}


