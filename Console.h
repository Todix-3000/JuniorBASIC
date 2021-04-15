//
// Created by tdix on 13.04.2021.
//

#ifndef JUNIORBASIC_SONSOLE_H
#define JUNIORBASIC_SONSOLE_H

#include <string>

class Console {

public:
    static void setCursor(unsigned int x, unsigned int y);
    static void clear();
    static void foregroundColor(unsigned int c);
    static void backgroundColor(unsigned int c);
    static void init();
    static void sound(unsigned short freq, unsigned short time);
    static int getChar();
    static bool isKeyPressed();
    static std::string input();
    static std::string input(std::string preSelect);

// private:
};

#endif //JUNIORBASIC_SONSOLE_H
