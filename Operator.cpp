//
// Created by tdix on 07.02.2021.
//

#include <iostream>
#include "Operator.h"
#include "utils.h"

void Operator::div(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 / params.op2);
    return;
    if (!stack->empty()) {
        auto v2 = stack->top();
        stack->pop();
        if (!stack->empty()) {
            auto v1 = stack->top();
            stack->pop();
            if (v1.getType()==TOKEN_TYPE_VALUE && v2.getType()==TOKEN_TYPE_VALUE) {
                Value result = v1.getValue() / v2.getValue();
                std::cout << result << "=" << v1.getValue() << "/" << v2.getValue() << std::endl;
                stack->push(*new Token(result));
                return;
            }
        }
    }
    throw Exception{EXCEPTION_ILLEGAL_EXPRESSION};
}

void Operator::mul(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 * params.op2);
    return;
    if (!stack->empty()) {
        auto v2 = stack->top();
        stack->pop();
        if (!stack->empty()) {
            auto v1 = stack->top();
            stack->pop();
            if (v1.getType()==TOKEN_TYPE_VALUE && v2.getType()==TOKEN_TYPE_VALUE) {
                Value result = v1.getValue() * v2.getValue();
                std::cout << result << "=" << v1.getValue() << "*" << v2.getValue() << std::endl;
                stack->push(*new Token(result));
                return;
            }
        }
    }
    throw Exception{EXCEPTION_ILLEGAL_EXPRESSION};
}

void Operator::add(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 + params.op2);
    return;
    if (!stack->empty()) {
        auto v2 = stack->top();
        stack->pop();
        if (!stack->empty()) {
            auto v1 = stack->top();
            stack->pop();
            if (v1.getType()==TOKEN_TYPE_VALUE && v2.getType()==TOKEN_TYPE_VALUE) {
                Value result = v1.getValue() + v2.getValue();
                std::cout << result << "=" << v1.getValue() << "+" << v2.getValue() << std::endl;
                stack->push(*new Token(result));
                return;
            }
        }
    }
    throw Exception{EXCEPTION_ILLEGAL_EXPRESSION};
}

void Operator::sub(std::stack<Token>* stack) {
    Binary params = Operator::getTwoValues(stack);
    stack->push(params.op1 - params.op2);
    return;
    if (!stack->empty()) {
        auto v2 = stack->top();
        stack->pop();
        if (!stack->empty()) {
            auto v1 = stack->top();
            stack->pop();
            if (v1.getType()==TOKEN_TYPE_VALUE && v2.getType()==TOKEN_TYPE_VALUE) {
                Value result = v1.getValue() - v2.getValue();
                std::cout << result << "=" << v1.getValue() << "-" << v2.getValue() << std::endl;
                stack->push(*new Token(result));
                return;
            }
        }
    }
    throw Exception{EXCEPTION_ILLEGAL_EXPRESSION};
}

void Operator::neg(std::stack<Token>* stack) {
    Unary params = Operator::getOneValue(stack);
    stack->push(params.op1);
    return;
    if (!stack->empty()) {
        auto v1 = stack->top();
        stack->pop();
        if (v1.getType()==TOKEN_TYPE_VALUE) {
            Value result = -v1.getValue();
            std::cout << result << "=" << "-" << v1.getValue() << std::endl;
            stack->push(*new Token(result));
            return;
        }
    }
    throw Exception{EXCEPTION_ILLEGAL_EXPRESSION};
}

void Operator::dummy(std::stack<Token>*) {
    throw Exception{EXCEPTION_NOT_IMPLEMENTED};
}

Binary Operator::getTwoValues(std::stack<Token> * stack) {
    Binary result;
    if (!stack->empty()) {
        auto v2 = stack->top();
        stack->pop();
        if (!stack->empty()) {
            auto v1 = stack->top();
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
        stack->pop();
        if (v1.getType() == TOKEN_TYPE_VALUE) {
            result.op1 = v1.getValue();
            return result;
        }
    }
    throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
}