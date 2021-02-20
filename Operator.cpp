//
// Created by tdix on 07.02.2021.
//

#include <iostream>
#include "Operator.h"
#include "utils.h"

void Operator::div(std::stack<Token>* stack) {
    if (!stack->empty()) {
        auto v2 = stack->top();
        stack->pop();
        if (!stack->empty()) {
            auto v1 = stack->top();
            stack->pop();
            if (v1.getType()==TOKEN_TYPE_VALUE && v2.getType()==TOKEN_TYPE_VALUE) {
                double result = v1.getValue() / v2.getValue();
                std::cout << result << "=" << v1.getValue() << "/" << v2.getValue() << std::endl;
                stack->push(*new Token(TOKEN_TYPE_VALUE, result));
                return;
            }
        }
    }
    throw Exception{EXCEPTION_ILLEGAL_EXPRESSION};
}

void Operator::mul(std::stack<Token>* stack) {
    if (!stack->empty()) {
        auto v2 = stack->top();
        stack->pop();
        if (!stack->empty()) {
            auto v1 = stack->top();
            stack->pop();
            if (v1.getType()==TOKEN_TYPE_VALUE && v2.getType()==TOKEN_TYPE_VALUE) {
                double result = v1.getValue() * v2.getValue();
                std::cout << result << "=" << v1.getValue() << "*" << v2.getValue() << std::endl;
                stack->push(*new Token(TOKEN_TYPE_VALUE, result));
                return;
            }
        }
    }
    throw Exception{EXCEPTION_ILLEGAL_EXPRESSION};
}

void Operator::add(std::stack<Token>* stack) {
    if (!stack->empty()) {
        auto v2 = stack->top();
        stack->pop();
        if (!stack->empty()) {
            auto v1 = stack->top();
            stack->pop();
            if (v1.getType()==TOKEN_TYPE_VALUE && v2.getType()==TOKEN_TYPE_VALUE) {
                double result = v1.getValue() + v2.getValue();
                std::cout << result << "=" << v1.getValue() << "+" << v2.getValue() << std::endl;
                stack->push(*new Token(TOKEN_TYPE_VALUE, result));
                return;
            }
        }
    }
    throw Exception{EXCEPTION_ILLEGAL_EXPRESSION};
}

void Operator::sub(std::stack<Token>* stack) {
    if (!stack->empty()) {
        auto v2 = stack->top();
        stack->pop();
        if (!stack->empty()) {
            auto v1 = stack->top();
            stack->pop();
            if (v1.getType()==TOKEN_TYPE_VALUE && v2.getType()==TOKEN_TYPE_VALUE) {
                double result = v1.getValue() - v2.getValue();
                std::cout << result << "=" << v1.getValue() << "-" << v2.getValue() << std::endl;
                stack->push(*new Token(TOKEN_TYPE_VALUE, result));
                return;
            }
        }
    }
    throw Exception{EXCEPTION_ILLEGAL_EXPRESSION};
}

void Operator::neg(std::stack<Token>* stack) {
    if (!stack->empty()) {
        auto v1 = stack->top();
        stack->pop();
        if (v1.getType()==TOKEN_TYPE_VALUE) {
            double result = -v1.getValue();
            std::cout << result << "=" << "-" << v1.getValue() << std::endl;
            stack->push(*new Token(TOKEN_TYPE_VALUE, result));
            return;
        }
    }
    throw Exception{EXCEPTION_ILLEGAL_EXPRESSION};
}

void Operator::dummy(std::stack<Token>*) {
    throw Exception{EXCEPTION_NOT_IMPLEMENTED};
}