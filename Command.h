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
    static unsigned char* _if(unsigned char* restOfLine);
    static unsigned char* run(unsigned char* restOfLine);
    static unsigned char* then(unsigned char* restOfLine);
    static unsigned char* load(unsigned char* restOfLine);
    static unsigned char* save(unsigned char* restOfLine);
    static unsigned char* _new(unsigned char* restOfLine);
    static unsigned char* end(unsigned char* restOfLine);
    static unsigned char* stop(unsigned char* restOfLine);
    static unsigned char* cont(unsigned char* restOfLine);
    static unsigned char* clr(unsigned char* restOfLine);
    static unsigned char* input(unsigned char* restOfLine);
};
#endif //JUNIORBASIC_COMMAND_H
