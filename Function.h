//
// Created by tdix on 07.03.2021.
//

#ifndef JUNIORBASIC_FUNCTION_H
#define JUNIORBASIC_FUNCTION_H
//
// Created by tdix on 07.02.2021.
//

#include "Token.h"
#include <stack>

class Function {
public:
    static void sin(std::stack<Token>*);
    static void cos(std::stack<Token>*);
    static void tan(std::stack<Token>*);
    static void atn(std::stack<Token>*);
    static void sqr(std::stack<Token>*);

    static void rnd(std::stack<Token>*);

//  ABS, , , EXP, INT, LOG, SGN, ,  und
// RND
// ASC, LEN, VAL
//  LEFT$, MID$, RIGHT$
    static void chr(std::stack<Token>*);
    static void str(std::stack<Token>*);

private:
    static std::vector<Token> getValuesFromStack(std::stack<Token>*);
};

#endif //JUNIORBASIC_FUNCTION_H
