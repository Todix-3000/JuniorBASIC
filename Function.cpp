//
// Created by tdix on 07.03.2021.
//
#include "Function.h"
#include "utils.h"

void Function::sin(std::stack<Token>*) {

}

void Function::cos(std::stack<Token>*) {

}

Token Function::getValueFromStack(std::stack<Token>* stack) {
    if (!stack->empty()) {
        auto v1 = stack->top();
        stack->pop();
        if (v1.getType() == TOKEN_TYPE_VALUE) {
            return v1.getValue();
        }
    }
    throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
}