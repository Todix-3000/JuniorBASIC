//
// Created by tdix on 06.02.2021.
//
#include <cstdlib>
#include <iostream>
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

short Token::getType() {
    return tokenType;
}

short Token::getPrecedence() {
    return precedence;
}

short Token::getAssoc() {
    return assoc;
}

Parser* Parser::instance = nullptr;

Parser* Parser::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Parser();
    }

    return instance;
}

Token* Parser::getNextToken(unsigned char * input) {
    static int i=0;
    i++;
    switch (i) {
        case 1: return new Token(TOKEN_TYPE_VALUE);
        case 2 : return operatorToken["+"];
        case 3 : return new Token(TOKEN_TYPE_VALUE);
        case 4 : return operatorToken["*"];
        case 5 : return new Token(TOKEN_TYPE_VALUE);
        case 6 : return new Token(TOKEN_TYPE_UNKNOWN);
    }
    return operatorToken["+"];
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

    seperatorToken = new Token(TOKEN_TYPE_SEPERATOR);
    bracketOpenToken = new Token(TOKEN_TYPE_BRACKETOPEN);
    bracketCloseToken = new Token(TOKEN_TYPE_BRACKETCLOSE);

    functionToken["SIN"] = new Token(TOKEN_TYPE_FUNCTION);
    functionToken["COS"] = new Token(TOKEN_TYPE_FUNCTION);
}

