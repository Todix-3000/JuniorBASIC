//
// Created by tdix on 07.02.2021.
//

#include <iostream>
#include <cmath>
#include <regex>
#include "Operator.h"
#include "utils.h"

void Operator::div(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 / params.op2);
}

void Operator::mul(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 * params.op2);
}

void Operator::add(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 + params.op2);
}

void Operator::sub(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 - params.op2);
}

void Operator::mod(std::stack<Token> *stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 % params.op2);
}

void Operator::lt(std::stack<Token> *stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 < params.op2);
}

void Operator::leqt(std::stack<Token> *stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 <= params.op2);
}

void Operator::gt(std::stack<Token> *stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 > params.op2);
}

void Operator::geqt(std::stack<Token> *stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 >= params.op2);
}

void Operator::eq(std::stack<Token> *stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 == params.op2);
}

void Operator::neq(std::stack<Token> *stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 != params.op2);
}


static int intPow(int x, int p) {
    if (p == 0) { return 1; }
    if (p == 1) { return x; }
    return x * intPow(x, p-1);
}
void Operator::pow(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    if (params.op1.getType()==VALUE_TYPE_INT && params.op2.getType()==VALUE_TYPE_INT && params.op2.getInt()>=0) {
        stack->push(Value(intPow(params.op1.getInt(), params.op2.getInt())));
    } else {
        stack->push(Value(std::pow(params.op1.getFloat(), params.op2.getFloat())));
    }
}

void Operator::bitAnd(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(Value(params.op1.getInt() & params.op2.getInt()));
}

void Operator::bitOr(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(Value(params.op1.getInt() | params.op2.getInt()));
}

void Operator::bitXor(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(Value(params.op1.getInt() ^ params.op2.getInt()));
}

void Operator::boolAnd(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    if (params.op1.getType() == VALUE_TYPE_STRING && params.op2.getType() == VALUE_TYPE_STRING) {
        if (params.op1.getString().length()>0 && params.op2.getString().length() >0) {
            stack->push(Value(1));
        } else {
            stack->push(Value(0));
        }
    } else if ((params.op1.getInt() != 0 && params.op2.getInt())) {
        stack->push(Value(1));
    } else {
        stack->push(Value(0));
    }
}

void Operator::boolOr(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(Value(params.op1.getInt() | params.op2.getInt()));
}

void Operator::neg(std::stack<Token>* stack) {
    Unary params = Operator::getOneValue(stack);
    stack->push(-params.op1);
}

void Operator::dummy(std::stack<Token>* stack) {
    Unary params = Operator::getOneValue(stack);
    stack->push(params.op1);
}

Binary Operator::getTwoValues(std::stack<Token> * stack) {
    Binary result;
    if (!stack->empty()) {
        auto v2 = stack->top();
        while (v2.getType()==TOKEN_TYPE_BRACKETOPEN) {
            stack->pop();
            if (stack->empty()) {
                throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
            }
            v2 = stack->top();
        }
        stack->pop();
        if (!stack->empty()) {
            auto v1 = stack->top();
            while (v1.getType()==TOKEN_TYPE_BRACKETOPEN) {
                stack->pop();
                if (stack->empty()) {
                    throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
                }
                v1 = stack->top();
            }
            stack->pop();
            if (v1.getType() == TOKEN_TYPE_VALUE && v2.getType() == TOKEN_TYPE_VALUE) {
                result.op1 = v1.getValue();
                result.op2 = v2.getValue();
                return result;
            }
        }
    }
    throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
}

Unary Operator::getOneValue(std::stack<Token> * stack) {
    Unary result;
    if (!stack->empty()) {
        auto v1 = stack->top();
        while (v1.getType()==TOKEN_TYPE_BRACKETOPEN) {
            stack->pop();
            if (stack->empty()) {
                throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
            }
            v1 = stack->top();
        }
        stack->pop();
        if (v1.getType() == TOKEN_TYPE_VALUE) {
            result.op1 = v1.getValue();
            if (!stack->empty() && stack->top().getType() == TOKEN_TYPE_BRACKETOPEN) {
                stack->pop();
            }
            return result;
        }
    }
    throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
}