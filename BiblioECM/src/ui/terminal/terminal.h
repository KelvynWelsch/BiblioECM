#pragma once

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

void clearFromPosition(HANDLE hOut, CONSOLE_SCREEN_BUFFER_INFO csbi);
void enableANSI();
void clearScreen();
void enableHorizontalScroll(int bufferWidth, int bufferHeight);
