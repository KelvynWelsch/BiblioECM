#pragma once

#include "strings.h"
#include "../../core/dto/BookSearchResult.h"
#include <array>



template <std::size_t N>
std::array<std::string, 2> criarLinha(std::array<std::string, N> strs, std::array<int, N> sizes) {
	std::array<int, N> dif;
	for (int i = 0; i < N; i++) {
		dif[i] = sizes[i] - static_cast<int>(visualLength(strs[i]));
	}
	std::string out;
	int total = 0;
	for (int i = 0; i < N; i++) {
		out += strToPrint(strs[i], dif[i]) + "|";
		total += static_cast<int>(sizes[i]);
	}
	total = total + N-1;
	std::string line = std::string(total, '-');
	return { out,line };
}


