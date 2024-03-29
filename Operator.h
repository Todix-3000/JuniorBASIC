//
// Created by tdix on 07.02.2021.
//

#include "Token.h"
#include <stack>

using Binary = struct {Value op1; Value op2;};
using Unary = struct {Value op1;};

class Operator {
public:
    static void mul(std::stack<Token>*);
    static void div(std::stack<Token>*);
    static void add(std::stack<Token>*);
    static void sub(std::stack<Token>*);
    static void neg(std::stack<Token>*);
    static void dummy(std::stack<Token>*);
    static void pow(std::stack<Token> *stack);
    static void bitAnd(std::stack<Token> *stack);
    static void bitOr(std::stack<Token> *stack);
    static void bitXor(std::stack<Token> *stack);
    static void boolAnd(std::stack<Token> *stack);
    static void boolOr(std::stack<Token> *stack);
    static void mod(std::stack<Token> *stack);
    static void lt(std::stack<Token> *stack);
    static void leqt(std::stack<Token> *stack);
    static void gt(std::stack<Token> *stack);
    static void geqt(std::stack<Token> *stack);
    static void eq(std::stack<Token> *stack);
    static void neq(std::stack<Token> *stack);

private:
    static Binary getTwoValues(std::stack<Token>*);
    static Unary getOneValue(std::stack<Token>*);


};
