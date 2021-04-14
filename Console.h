//
// Created by tdix on 13.04.2021.
//

#ifndef JUNIORBASIC_SONSOLE_H
#define JUNIORBASIC_SONSOLE_H
class Console {

public:
    static void setCursor(unsigned int x, unsigned int y);
    static void clear();
    static void foregroundColor(unsigned int c);
    static void backgroundColor(unsigned int c);
    static void init();

// private:
    static void setColor(unsigned int fg, unsigned int bg);
};

#endif //JUNIORBASIC_SONSOLE_H
