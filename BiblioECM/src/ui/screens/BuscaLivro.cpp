#include "BuscaLivro.h"

#include "../utils/exibirSelecao.h"
#include "../terminal/terminal.h"
#include "../terminal/InputUtils.h"
#include "../utils/criarLinha.h"
#include "../../core/BookSearchField.h"
#include "Reservar.h"
#include "AddItem.h"

void BuscaLivro(LibService& serv){
	clearScreen();
	std::cout << "========== Busca por um livro ==========" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Digite uma palavra ou expressão que deseja buscar: ";
	std::string s = lerTexto();
		clearScreen();
		std::cout << "Expressão a ser buscada: " + s + "\n\n\n" +
			"Você deseja procurar esta expressão entre os títulos ou nomes dos autores?\n";
		
		std::array<std::string, 3> opcoes = {
			"Ambos",
			"Título",
			"Autor"
		};
		
		int selecionada = exibirSelecao(opcoes);

	ECM::Expected<std::vector<BookSearchResult>> Resultado;
	
	switch (selecionada) {
	case 1:
		Resultado = serv.buscarLivros(s, BookSearchField::Title);
		break;
	case 2: 
		Resultado = serv.buscarLivros(s, BookSearchField::Author);
		break;
	default:
		Resultado = serv.buscarLivros(s, BookSearchField::TitleOrAuthor);
		break;
	}
	if (!Resultado) {
		std::cout << Resultado.error().message << std::endl;
		system("pause");
		return;
	}
	clearScreen();
	auto& res = Resultado.value();
	std::cout << "Foram encontrados " << res.size() << " resultados:" << std::endl;
	std::cout << std::endl;
	const int bsr_size = 6;
	std::array<int, bsr_size> sizes = { 6, 30, 25, 8, 10, 10 };
	std::array<std::string, bsr_size> line = {
		"ID",
		"Título",
		"Autor",
		"n. itens",
		"disponíveis",
		"emprestados"
	};
	auto labels = criarLinha(line, sizes);
	std::cout << "Resultados da busca: \n\n\n" + labels[0] + "\n" + labels[1] + "\n";
	std::string caput2;
	std::vector<std::string> out;
	for (int i = 0; i < res.size(); i++) {
		auto& bsr = res[i];
		std::array<std::string, bsr_size> strs = {
			bsr.book_id.get_Id_format(),
			bsr.title,
			bsr.author,
			std::to_string(bsr.total_items),
			std::to_string(bsr.available),
			std::to_string(bsr.loaned)
		};
		auto linha = criarLinha(strs, sizes);
		out.push_back(linha[0]);
		caput2 = caput2 + linha[0] + "\n" + linha[1] + "\n";
	}
	std::array<std::string, 3> escolha = { "Reservar um título", "Adicionar itens", "Voltar"};
	std::cout << caput2 + "\n\n O que você deseja fazer?\n" << std::endl;
	int input = exibirSelecao(escolha);
	int choice;
	switch (input) {
	case 0:
		clearScreen(); 
		choice = exibirSelecao("Qual livro você gostaria de reservar?\n\n\n  " + labels[0] + "\n" + labels[1], out, labels[1]);
		Reservar(serv, res[choice].book_id);
		break;
	case 1:
		clearScreen();
		choice = exibirSelecao("A qual livro você gostaria de adicionar itens?\n\n\n  " + labels[0] + "\n" + labels[1], out, labels[1]);
		addItem(serv, res[choice].book_id);
		break;
	default:
		break;
	}
}

