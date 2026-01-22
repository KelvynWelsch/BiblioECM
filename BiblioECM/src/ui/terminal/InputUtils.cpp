#include "InputUtils.h"
#include <iostream>


#ifdef _WIN32
#include <windows.h>
#endif

std::string lerTexto() {
#ifdef _WIN32
    // 1. Prepara um buffer para ler caracteres largos (UTF-16)
    const int BUFFER_SIZE = 2048;
    wchar_t wBuffer[BUFFER_SIZE];
    DWORD charsRead = 0;
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

    // 2. Lê diretamente do console do Windows (evita o bug do std::cin)
    if (!ReadConsoleW(hInput, wBuffer, BUFFER_SIZE - 1, &charsRead, NULL)) {
        return "";
    }

    // 3. Remove o "Enter" (\r\n) que vem junto
    // Verifica se leu algo e remove os ultimos caracteres se forem quebra de linha
    if (charsRead >= 2 && wBuffer[charsRead - 2] == L'\r') {
        wBuffer[charsRead - 2] = L'\0';
    }
    else if (charsRead > 0 && (wBuffer[charsRead - 1] == L'\n' || wBuffer[charsRead - 1] == L'\r')) {
        wBuffer[charsRead - 1] = L'\0';
    }
    else {
        wBuffer[charsRead] = L'\0';
    }

    // 4. Converte de UTF-16 (Windows) para UTF-8 (SQLite/C++)
    // Primeiro descobre o tamanho necessário
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wBuffer, -1, NULL, 0, NULL, NULL);

    // Cria a string do tamanho certo
    std::string strTo(size_needed - 1, 0); // -1 por causa do terminador nulo

    // Faz a conversão real
    WideCharToMultiByte(CP_UTF8, 0, wBuffer, -1, &strTo[0], size_needed, NULL, NULL);

    return strTo;
#else
    // Versão simples para Linux/Mac
    std::string line;
    std::getline(std::cin, line);
    return line;
#endif
}
