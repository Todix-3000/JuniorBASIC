//
// Created by tdix on 06.02.2021.
//
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <utility>
#include <math.h>
#include "Token.h"
#include "Operator.h"
#include "utils.h"

Token::Token(short tokenType) {
    this->tokenType = tokenType;
}

Token::Token(short tokenType, short precedence, short assoc, void (*funcptr)(std::stack<Token>*)) {
    this->tokenType  = tokenType;
    this->precedence = precedence;
    this->assoc      = assoc;
    this->funcptr    = funcptr;
    tokenValue       = Value();
}

Token::Token(Value tokenValue) {
    this->tokenType = TOKEN_TYPE_VALUE;
    this->tokenValue = tokenValue;
}

short Token::getType() {
    return tokenType;
}

short Token::getPrecedence() {
    return precedence;
}

short Token::getAssoc() {
    return assoc;
}

Value Token::getValue() {
    return tokenValue;
}

Parser* Parser::instance = nullptr;

Parser* Parser::getInstance(unsigned char * input)
{
    if (instance == nullptr)
    {
        instance = new Parser();
    }
    instance->setInputPtr(input);
    return instance;
}

void Parser::setInputPtr(unsigned char * input) {
    inputPtr = input;
}

Token* Parser::getNextToken(bool unaryOperator) {
    Token *myToken;
    while (' ' == *inputPtr) {
        inputPtr++;
    }
    if (*inputPtr >= '0' && *inputPtr <= '9') {
        int value = 0;
        while (*inputPtr >= '0' && *inputPtr <= '9') {
            value *= 10;
            value += *inputPtr - '0';
            inputPtr++;
        }
        if (*inputPtr == '.') {
            double dValue = value;
            int comma = 0;
            inputPtr++;
            while (*inputPtr >= '0' && *inputPtr <= '9') {
                comma++;
                dValue += (*inputPtr - '0')/pow(10, comma);
                inputPtr++;
            }
            return new Token(Value(dValue));
        } else {
            return new Token(Value(value));
        }
    }
    if (*inputPtr == '"') {
        unsigned char* start = ++inputPtr;
        while (*inputPtr != '"') {
            if (*inputPtr == '\0') {
                throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
            }
            inputPtr++;
        }
        char subbuff[inputPtr-start+1];
        memcpy( subbuff, start, inputPtr-start );
        subbuff[inputPtr-start] = '\0';
        inputPtr++;
        return new Token(std::string(subbuff));

    }
    if ((myToken = findToken(commandToken)) != nullptr) {
        return myToken;
    }
    if (unaryOperator) {
        if ((myToken = findToken(unaryOperatorToken)) != nullptr) {
            return myToken;
        }
    }
    if ((myToken = findToken(operatorToken)) != nullptr) {
        return myToken;
    }
    if ((myToken = findToken(functionToken)) != nullptr) {
        return myToken;
    }
    if (*inputPtr == '(' ) {
        inputPtr++;
        return new Token(TOKEN_TYPE_BRACKETOPEN);
    }
    if (*inputPtr == ')' ) {
        inputPtr++;
        return new Token(TOKEN_TYPE_BRACKETCLOSE);
    }
    return new Token(TOKEN_TYPE_UNKNOWN);
}

Token* Parser::findToken(TokenMap map) {
    for (TokenMap::iterator it=map.begin(); it!=map.end(); ++it) {
        if ( memcmp (inputPtr, it->first.data(), it->first.length() ) == 0 ) {
            inputPtr += it->first.length();
             std::cout << it->first;
            return it->second;
        }
    }
    return nullptr;
}

void Token::call(std::stack<Token>* tokenStack) {
    funcptr(tokenStack);
}

Parser::Parser() {
    operatorToken["^"]   = new Token(TOKEN_TYPE_OPERATOR, 12, RIGHT, Operator::pow);
    operatorToken["*"]   = new Token(TOKEN_TYPE_OPERATOR, 11, LEFT, Operator::mul);
    operatorToken["/"]   = new Token(TOKEN_TYPE_OPERATOR, 11, LEFT, Operator::div);
    operatorToken["%"]   = new Token(TOKEN_TYPE_OPERATOR, 11, LEFT, Operator::mod);
    operatorToken["+"]   = new Token(TOKEN_TYPE_OPERATOR, 10, LEFT, Operator::add);
    operatorToken["-"]   = new Token(TOKEN_TYPE_OPERATOR, 10, LEFT, Operator::sub);
    operatorToken["<="]  = new Token(TOKEN_TYPE_OPERATOR, 8, LEFT, Operator::leqt);
    operatorToken[">="]  = new Token(TOKEN_TYPE_OPERATOR, 8, LEFT, Operator::geqt);
    operatorToken["<>"]  = new Token(TOKEN_TYPE_OPERATOR, 6, LEFT, Operator::neq);
    operatorToken["<"]   = new Token(TOKEN_TYPE_OPERATOR, 8, LEFT, Operator::lt);
    operatorToken[">"]   = new Token(TOKEN_TYPE_OPERATOR, 8, LEFT, Operator::gt);
    operatorToken["="]   = new Token(TOKEN_TYPE_OPERATOR, 6, LEFT, Operator::eq);
    operatorToken["AND"] = new Token(TOKEN_TYPE_OPERATOR, 5, LEFT, Operator::bitAnd);
    operatorToken["XOR"] = new Token(TOKEN_TYPE_OPERATOR, 4, LEFT, Operator::bitXor);
    operatorToken["OR"]  = new Token(TOKEN_TYPE_OPERATOR, 3, LEFT, Operator::bitOr);
    operatorToken["&&"]  = new Token(TOKEN_TYPE_OPERATOR, 2, LEFT, Operator::boolAnd);
    operatorToken["||"]  = new Token(TOKEN_TYPE_OPERATOR, 1, LEFT, Operator::boolOr);

    unaryOperatorToken["+"]   = new Token(TOKEN_TYPE_OPERATOR, 13, LEFT, Operator::dummy);
    unaryOperatorToken["-"]   = new Token(TOKEN_TYPE_OPERATOR, 13, LEFT, Operator::neg);

    seperatorToken    = new Token(TOKEN_TYPE_SEPERATOR);
    bracketOpenToken  = new Token(TOKEN_TYPE_BRACKETOPEN);
    bracketCloseToken = new Token(TOKEN_TYPE_BRACKETCLOSE);

    functionToken["SIN"] = new Token(TOKEN_TYPE_FUNCTION);
    functionToken["COS"] = new Token(TOKEN_TYPE_FUNCTION);

    commandToken["PRINT"] = new Token(TOKEN_TYPE_COMMAND);

}

