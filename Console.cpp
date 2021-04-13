//
// Created by tdix on 13.04.2021.
//

#include "Console.h"
#include <windows.h>

void Console::setCursor(unsigned int x, unsigned int y) {
    COORD c;
    HANDLE hStdOut;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        return;
    }

    c.X = (SHORT) x;
    c.Y = (SHORT) y;

    SetConsoleCursorPosition(hStdOut, c);
}

void Console::clear() {
    HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        return;
    }
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        return;
    }
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    if (!FillConsoleOutputCharacter(
            hStdOut,
            (TCHAR) ' ',
            cellCount,
            homeCoords,
            &count
    )) {
        return;
    }

    if (!FillConsoleOutputAttribute(
            hStdOut,
            csbi.wAttributes,
            cellCount,
            homeCoords,
            &count
    )) {
        return;
    }
    SetConsoleCursorPosition(hStdOut, homeCoords);
}

void Console::foregroundColor(unsigned int c) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        return;
    }
    if (!GetConsoleScreenBufferInfo(hStdOut, &info)) {
        return;
    }
    SetConsoleTextAttribute(hStdOut, (info.wAttributes & 0x00f0) | c);
}

void Console::backgroundColor(unsigned int c) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        return;
    }
    if (!GetConsoleScreenBufferInfo(hStdOut, &info)) {
        return;
    }
    SetConsoleTextAttribute(hStdOut, (info.wAttributes & 0x000f) | (c << 4));
}


