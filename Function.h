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

    static void chr(std::stack<Token>*);
    static void str(std::stack<Token>*);

private:
    static std::vector<Token> getValuesFromStack(std::stack<Token>*);

};

#endif //JUNIORBASIC_FUNCTION_H
