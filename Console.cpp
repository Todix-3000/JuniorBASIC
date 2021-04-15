//
// Created by tdix on 13.04.2021.
//

#include "Console.h"
#include <windows.h>
#include <utilapiset.h>
#include <conio.h>
#include <iostream>

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

void Console::init() {
//    system("mode con COLS=80");
//ShowWindow(GetConsoleWindow(),SW_MAXIMIZE);
    //  SendMessage(GetConsoleWindow(),WM_SYSKEYDOWN,VK_RETURN,0x20000000);
    keybd_event(VK_MENU, 0x38, 0, 0);
    keybd_event(VK_RETURN, 0x1c, 0, 0);
    keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);

    SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);

    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hstdout, &csbi);

    csbi.dwSize.X = csbi.dwMaximumWindowSize.X;
    csbi.dwSize.Y = csbi.dwMaximumWindowSize.Y;
    SetConsoleScreenBufferSize(hstdout, csbi.dwSize);

    HWND x = GetConsoleWindow();
    ShowScrollBar(x, SB_BOTH, FALSE);
}

void Console::sound(unsigned short freq, unsigned short time) {
    Beep(freq, time);
}

int Console::getChar() {
    return getch();
}

bool Console::isKeyPressed() {
    return isKeyPressed();
}

std::string Console::input() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

std::string Console::input(std::string preSelect) {
    return input();
}


