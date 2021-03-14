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
    static void abs(std::stack<Token>*);
    static void sgn(std::stack<Token>*);
    static void exp(std::stack<Token>*);
    static void log(std::stack<Token>*);
    static void rnd(std::stack<Token>*);
    static void xint(std::stack<Token>*);

    static void asc(std::stack<Token>*);
    static void len(std::stack<Token>*);
    static void val(std::stack<Token>*);

    static void chr(std::stack<Token>*);
    static void str(std::stack<Token>*);
    static void left(std::stack<Token>*);
    static void right(std::stack<Token>*);
    static void mid(std::stack<Token>*);
private:
    static std::vector<Token> getValuesFromStack(std::stack<Token>*);
};

#endif //JUNIORBASIC_FUNCTION_H
