//
// Created by tdix on 07.02.2021.
//

#include "Token.h"
#include <stack>

class Operator {
public:
    static void mul(std::stack<Token>*);
    static void div(std::stack<Token>*);
    static void add(std::stack<Token>*);
    static void sub(std::stack<Token>*);
    static void dummy(std::stack<Token>*);
};
