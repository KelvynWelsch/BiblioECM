#include "terminal.h"
#include <iostream>



#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(hOut, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, mode);
    }
}

void clearScreen() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);

        DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
        DWORD written;

        // Preenche tudo com espaço
        FillConsoleOutputCharacter(hOut, ' ', cellCount, { 0,0 }, &written);

        // Reseta atributos (cores)
        FillConsoleOutputAttribute(hOut, csbi.wAttributes, cellCount, { 0,0 }, &written);

        // Volta o cursor pro topo
        SetConsoleCursorPosition(hOut, { 0,0 });
}


void clearFromPosition(HANDLE hOut, CONSOLE_SCREEN_BUFFER_INFO csbi) {
    
    COORD start = csbi.dwCursorPosition;

    DWORD cellCount =
        (csbi.dwSize.X * csbi.dwSize.Y)
        - (start.Y * csbi.dwSize.X + start.X);

    DWORD written;

    FillConsoleOutputCharacter(hOut, ' ', cellCount, start, &written);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, cellCount, start, &written);

    SetConsoleCursorPosition(hOut, start);
}

void enableHorizontalScroll(int bufferWidth, int bufferHeight)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hOut, &info);

    COORD newSize;
    newSize.X = bufferWidth;
    newSize.Y = bufferHeight;

    SetConsoleScreenBufferSize(hOut, newSize);

    SMALL_RECT windowSize;
    windowSize.Left = 0;
    windowSize.Top = 0;
    windowSize.Right = min((SHORT)(bufferWidth - 1), info.srWindow.Right);
    windowSize.Bottom = info.srWindow.Bottom;

    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
}
