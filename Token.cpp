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
#include "Function.h"

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

Token::Token(short tokenType, void (*funcptr)(std::stack<Token>*)) {
    this->tokenType  = tokenType;
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
    if (*inputPtr == ',' ) {
        inputPtr++;
        return new Token(TOKEN_TYPE_SEPERATOR);
    }
    return new Token(TOKEN_TYPE_UNKNOWN);
}

Token* Parser::findToken(TokenVector map) {
    for (auto const& value: map) {
        if ( memcmp (inputPtr, value.text.data(), value.text.length()) == 0 ) {
            inputPtr += value.text.length();
            // std::cout << value.text;
            return value.token;
        }
    }
    return nullptr;
}

void Token::call(std::stack<Token>* tokenStack) {
    funcptr(tokenStack);
}

Parser::Parser() {

    operatorToken.push_back(TokenDefinition("^",  new Token(TOKEN_TYPE_OPERATOR, 12, RIGHT, Operator::pow)));
    operatorToken.push_back(TokenDefinition("*",  new Token(TOKEN_TYPE_OPERATOR, 11, LEFT, Operator::mul)));
    operatorToken.push_back(TokenDefinition("/",  new Token(TOKEN_TYPE_OPERATOR, 11, LEFT, Operator::div)));
    operatorToken.push_back(TokenDefinition("%",  new Token(TOKEN_TYPE_OPERATOR, 11, LEFT, Operator::mod)));
    operatorToken.push_back(TokenDefinition("+",  new Token(TOKEN_TYPE_OPERATOR, 10, LEFT, Operator::add)));
    operatorToken.push_back(TokenDefinition("-",  new Token(TOKEN_TYPE_OPERATOR, 10, LEFT, Operator::sub)));
    operatorToken.push_back(TokenDefinition("<=", new Token(TOKEN_TYPE_OPERATOR, 8, LEFT, Operator::leqt)));
    operatorToken.push_back(TokenDefinition(">=", new Token(TOKEN_TYPE_OPERATOR, 8, LEFT, Operator::geqt)));
    operatorToken.push_back(TokenDefinition("<>", new Token(TOKEN_TYPE_OPERATOR, 6, LEFT, Operator::neq)));
    operatorToken.push_back(TokenDefinition("<",  new Token(TOKEN_TYPE_OPERATOR, 8, LEFT, Operator::lt)));
    operatorToken.push_back(TokenDefinition(">",  new Token(TOKEN_TYPE_OPERATOR, 8, LEFT, Operator::gt)));
    operatorToken.push_back(TokenDefinition("=",  new Token(TOKEN_TYPE_OPERATOR, 6, LEFT, Operator::eq)));
    operatorToken.push_back(TokenDefinition("AND",new Token(TOKEN_TYPE_OPERATOR, 5, LEFT, Operator::bitAnd)));
    operatorToken.push_back(TokenDefinition("XOR",new Token(TOKEN_TYPE_OPERATOR, 4, LEFT, Operator::bitXor)));
    operatorToken.push_back(TokenDefinition("OR", new Token(TOKEN_TYPE_OPERATOR, 3, LEFT, Operator::bitOr)));
    operatorToken.push_back(TokenDefinition("&&", new Token(TOKEN_TYPE_OPERATOR, 2, LEFT, Operator::boolAnd)));
    operatorToken.push_back(TokenDefinition("||", new Token(TOKEN_TYPE_OPERATOR, 1, LEFT, Operator::boolOr)));

    unaryOperatorToken.push_back(TokenDefinition("+", new Token(TOKEN_TYPE_OPERATOR, 13, LEFT, Operator::dummy)));
    unaryOperatorToken.push_back(TokenDefinition("-", new Token(TOKEN_TYPE_OPERATOR, 13, LEFT, Operator::neg)));

    seperatorToken    = new Token(TOKEN_TYPE_SEPERATOR);
    bracketOpenToken  = new Token(TOKEN_TYPE_BRACKETOPEN);
    bracketCloseToken = new Token(TOKEN_TYPE_BRACKETCLOSE);

    functionToken.push_back(TokenDefinition("SIN",  new Token(TOKEN_TYPE_FUNCTION, Function::sin)));
    functionToken.push_back(TokenDefinition("COS",  new Token(TOKEN_TYPE_FUNCTION, Function::cos)));
    functionToken.push_back(TokenDefinition("TAN",  new Token(TOKEN_TYPE_FUNCTION, Function::tan)));
    functionToken.push_back(TokenDefinition("ATN",  new Token(TOKEN_TYPE_FUNCTION, Function::atn)));
    functionToken.push_back(TokenDefinition("SQR",  new Token(TOKEN_TYPE_FUNCTION, Function::sqr)));
    functionToken.push_back(TokenDefinition("RND",  new Token(TOKEN_TYPE_FUNCTION, Function::rnd)));
    functionToken.push_back(TokenDefinition("CHR$",  new Token(TOKEN_TYPE_FUNCTION, Function::chr)));
    functionToken.push_back(TokenDefinition("STR$",  new Token(TOKEN_TYPE_FUNCTION, Function::str)));

    commandToken.push_back(TokenDefinition("PRINT", new Token(TOKEN_TYPE_COMMAND)));

}

TokenDefinition::TokenDefinition(std::string text, Token *token) {
    this->token = token;
    this->text  = text;
}