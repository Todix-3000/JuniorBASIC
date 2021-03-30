//
// Created by tdix on 30.03.2021.
//


#ifndef JUNIORBASIC_COMMAND_H
#define JUNIORBASIC_COMMAND_H

#include <string>

class Command {
public:
    static unsigned char* _goto(unsigned char* restOfLine);
    static unsigned char* list(unsigned char* restOfLine);
    static unsigned char* print(unsigned char* restOfLine);
    static unsigned char* rem(unsigned char* restOfLine);
    static unsigned char* let(unsigned char* restOfLine);

};
#endif //JUNIORBASIC_COMMAND_H
