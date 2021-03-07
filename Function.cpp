//
// Created by tdix on 07.03.2021.
//
#include "Function.h"
#include "utils.h"
#include <cmath>

void Function::sin(std::stack<Token>* stack) {

    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value(std::sin(params[0].getValue().getFloat())));
}

void Function::cos(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value(std::cos(params[0].getValue().getFloat())));
}

std::vector<Token> Function::getValuesFromStack(std::stack<Token>* stack) {
    std::vector<Token> result;
    while(!stack->empty()) {
        auto value = stack->top();
        if (value.getType() == TOKEN_TYPE_VALUE) {
            stack->pop();
            result.insert(result.begin(), value);
        } else {
            return result;
        }
    }
    return result;
}