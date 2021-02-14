//
// Created by tdix on 06.02.2021.
//
#include <cstdlib>
#include <iostream>
#include <cstring>
#include "Token.h"
#include "Operator.h"

Token::Token(short tokenType) {
    this->tokenType = tokenType;
}

Token::Token(short tokenType, short precedence, short assoc, void (*funcptr)(std::stack<Token>)) {
    this->tokenType  = tokenType;
    this->precedence = precedence;
    this->assoc      = assoc;
    this->funcptr    = funcptr;
}

Token::Token(short tokenType, double tokenValue) {
    this->tokenType  = tokenType;
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

double Token::getValue() {
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

Token* Parser::getNextToken() {
    Token *myToken;
    while (' ' == *inputPtr) {
        inputPtr++;
    }
    if (*inputPtr >= '0' && *inputPtr <= '9') {
        myToken = new Token(TOKEN_TYPE_VALUE, (double) *inputPtr - '0');
        inputPtr++;
        return myToken;
    }
    if ((myToken = findToken(commandToken)) != nullptr) {
        return myToken;
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

Parser::Parser() {
    operatorToken["^"]   = new Token(TOKEN_TYPE_OPERATOR, 2, RIGHT, Operator::dummy);
    operatorToken["*"]   = new Token(TOKEN_TYPE_OPERATOR, 3, LEFT, Operator::mul);
    operatorToken["/"]   = new Token(TOKEN_TYPE_OPERATOR, 3, LEFT, Operator::div);
    operatorToken["%"]   = new Token(TOKEN_TYPE_OPERATOR, 3, LEFT, Operator::dummy);
    operatorToken["+"]   = new Token(TOKEN_TYPE_OPERATOR, 4, LEFT, Operator::add);
    operatorToken["-"]   = new Token(TOKEN_TYPE_OPERATOR, 4, LEFT, Operator::sub);
    operatorToken["<="]  = new Token(TOKEN_TYPE_OPERATOR, 6, LEFT, Operator::dummy);
    operatorToken[">="]  = new Token(TOKEN_TYPE_OPERATOR, 6, LEFT, Operator::dummy);
    operatorToken["<"]   = new Token(TOKEN_TYPE_OPERATOR, 6, LEFT, Operator::dummy);
    operatorToken[">"]   = new Token(TOKEN_TYPE_OPERATOR, 6, LEFT, Operator::dummy);
    operatorToken["="]   = new Token(TOKEN_TYPE_OPERATOR, 7, LEFT, Operator::dummy);
    operatorToken["AND"] = new Token(TOKEN_TYPE_OPERATOR, 8, LEFT, Operator::dummy);
    operatorToken["XOR"] = new Token(TOKEN_TYPE_OPERATOR, 9, LEFT, Operator::dummy);
    operatorToken["OR"]  = new Token(TOKEN_TYPE_OPERATOR, 10, LEFT, Operator::dummy);
    operatorToken["&&"]  = new Token(TOKEN_TYPE_OPERATOR, 11, LEFT, Operator::dummy);
    operatorToken["||"]  = new Token(TOKEN_TYPE_OPERATOR, 12, LEFT, Operator::dummy);

    seperatorToken    = new Token(TOKEN_TYPE_SEPERATOR);
    bracketOpenToken  = new Token(TOKEN_TYPE_BRACKETOPEN);
    bracketCloseToken = new Token(TOKEN_TYPE_BRACKETCLOSE);

    functionToken["SIN"] = new Token(TOKEN_TYPE_FUNCTION);
    functionToken["COS"] = new Token(TOKEN_TYPE_FUNCTION);

    commandToken["PRINT"] = new Token(TOKEN_TYPE_COMMAND);

}

