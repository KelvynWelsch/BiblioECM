#include <windows.h>
#include "terminal.h"

void configurarTerminal() {
    // ----- lida com caracteres especiais no input e output
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    // ----- evita quebra de linhas
    enableHorizontalScroll(150, 2000);
    // ----- ativa expressões de controle
    enableANSI();
}
