//
// Created by tdix on 07.02.2021.
//

#include <iostream>
#include "Operator.h"

void Operator::div(std::stack<Token>* stack) {
    auto v2 = stack->top();
    stack->pop();
    auto v1 = stack->top();
    stack->pop();
    double result = v1.getValue() / v2.getValue();
    std::cout << result << "=" << v1.getValue() << "/" << v2.getValue() << "\n";
    stack->push(*new Token(TOKEN_TYPE_VALUE, result));
}

void Operator::mul(std::stack<Token>* stack) {
    auto v2 = stack->top();
    stack->pop();
    auto v1 = stack->top();
    stack->pop();
    double result = v1.getValue() * v2.getValue();
    std::cout << result << "=" << v1.getValue() << "*" << v2.getValue() << "\n";
    stack->push(*new Token(TOKEN_TYPE_VALUE, result));

}

void Operator::add(std::stack<Token>* stack) {
    auto v2 = stack->top();
    stack->pop();
    auto v1 = stack->top();
    stack->pop();
    double result = v1.getValue() + v2.getValue();
    std::cout << result << "=" << v1.getValue() << "+" << v2.getValue() << "\n";
    stack->push(*new Token(TOKEN_TYPE_VALUE, result));
}

void Operator::sub(std::stack<Token>* stack) {
    auto v2 = stack->top();
    stack->pop();
    auto v1 = stack->top();
    stack->pop();
    double result = v1.getValue() - v2.getValue();
    std::cout << result << "=" << v1.getValue() << "-" << v2.getValue() << "\n";
    stack->push(*new Token(TOKEN_TYPE_VALUE, result));
}

void Operator::neg(std::stack<Token>* stack) {
    auto v1 = stack->top();
    stack->pop();
    double result = -v1.getValue();
    std::cout << result << "=" << "-" << v1.getValue() << "\n";
    stack->push(*new Token(TOKEN_TYPE_VALUE, result));
}

void Operator::dummy(std::stack<Token>*) {

}