//
// Created by tdix on 07.03.2021.
//
#include "Function.h"
#include "utils.h"
#include <cmath>
#include <sstream>
#include <ctime>

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

void Function::tan(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value(std::tan(params[0].getValue().getFloat())));
}

void Function::atn(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value(std::atan(params[0].getValue().getFloat())));
}
void Function::sqr(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value(std::sqrt(params[0].getValue().getFloat())));
}

void Function::rnd(std::stack<Token>* stack) {
    static bool init = false;
    if (!init) {
        srand(time(NULL));
        rand();
        srand(rand());
        init = true;
    }
    auto params = getValuesFromStack(stack);
    if (params.size() != 0) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    stack->push(Value((double)rand()/(double)RAND_MAX));
}

void Function::chr(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    auto c = params[0].getValue().getInt();
    if (c<0 || c>255) {
        throw Exception(EXCEPTION_RANGE_ERROR);
    }

    stack->push(Value(std::string(1, c)));
}

void Function::str(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_INT) {
        stack->push(Value(std::to_string(params[0].getValue().getInt())));
        return;
    }
    if (params[0].getValue().getType() == VALUE_TYPE_FLOAT) {
        std::ostringstream ss;
        ss << params[0].getValue().getFloat();
        stack->push(Value(ss.str()));
        return;
    }
    throw Exception(EXCEPTION_TYPE_MISMATCH);
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