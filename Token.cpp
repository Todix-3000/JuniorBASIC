//
// Created by tdix on 06.02.2021.
//
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <utility>
#include <math.h>
#include "Token.h"
#include "Operator.h"
#include "utils.h"
#include "Function.h"
#include "Variable.h"


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

Token::Token(short tokenType, std::string varName, short varType) {
    this->tokenType  = tokenType;
    tokenValue       = Value(varName);
    assoc            = varType;
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
    instance->bracketLevel = 0;
    return instance;
}

void Parser::setInputPtr(unsigned char * input) {
    inputPtr = input;
}

VarDefinition Parser::getVariableDefinition() {
    VarDefinition definition;
    definition.varName = "";
    definition.varType = VALUE_TYPE_FLOAT;

    if (*inputPtr >= 'A' && *inputPtr<='Z') {
        while ((*inputPtr >= 'A' && *inputPtr <= 'Z') ||
               (*inputPtr >= '0' && *inputPtr <= '9') ||
               *inputPtr == '$' || *inputPtr == '%') {
            definition.varName += *inputPtr;
            if (*inputPtr == '$') {
                definition.varType = VALUE_TYPE_STRING;
                inputPtr++;
                break;
            }
            if (*inputPtr == '%') {
                definition.varType = VALUE_TYPE_INT;
                inputPtr++;
                break;
            }
            inputPtr++;
        }
        while (*inputPtr == ' ') {
            *inputPtr++;
        }
        return definition;
    }
    throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
}

Token* Parser::getNextToken(bool unaryOperator) {
    Token *myToken;
    while (' ' == *inputPtr) {
        inputPtr++;
    }
    if (*inputPtr >= '0' && *inputPtr <= '9') {
        size_t intLength;
        size_t doubleLength;
        int intValue = std::stoi((char*)inputPtr, &intLength);
        double doubleValue = std::stod((char*)inputPtr, &doubleLength);
        if (doubleLength>intLength) {
            inputPtr = &inputPtr[doubleLength];
            return new Token(Value(doubleValue));
        }
        inputPtr = &inputPtr[intLength];
        return new Token(Value(intValue));
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
    if ((myToken = findVariable()) != nullptr) {
        return myToken;
    }
    if (*inputPtr == '(' ) {
        inputPtr++;
        bracketLevel++;
        return new Token(TOKEN_TYPE_BRACKETOPEN);
    }
    if (*inputPtr == ')' && bracketLevel>0) {
        inputPtr++;
        bracketLevel--;
        return new Token(TOKEN_TYPE_BRACKETCLOSE);
    }
    if (*inputPtr == ','  && bracketLevel>0) {
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

Token* Parser::findVariable() {
    try {
        VarDefinition definition = getVariableDefinition();
        if (*inputPtr=='(') {
            return new Token(TOKEN_TYPE_ARRAY, definition.varName, definition.varType);
        }
        return new Token(Variable::getContainer()->getValue(definition.varName, definition.varType));
    }
    catch (Exception e) {}
    return nullptr;
}

void Token::call(std::stack<Token>* tokenStack) {
    funcptr(tokenStack);
}

void Token::fetchArrayValue(std::stack<Token>* tokenStack) {
    Function::getArrayVariableValue(tokenStack, tokenValue.getString(), assoc);
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

    functionToken.assign(FUN_SIN, TokenDefinition("SIN",    new Token(TOKEN_TYPE_FUNCTION, Function::sin)));
    functionToken.assign(FUN_COS, TokenDefinition("COS",    new Token(TOKEN_TYPE_FUNCTION, Function::cos)));
    functionToken.assign(FUN_TAN, TokenDefinition("TAN",    new Token(TOKEN_TYPE_FUNCTION, Function::tan)));
    functionToken.assign(FUN_ATN, TokenDefinition("ATN",    new Token(TOKEN_TYPE_FUNCTION, Function::atn)));
    functionToken.assign(FUN_SQR, TokenDefinition("SQR",    new Token(TOKEN_TYPE_FUNCTION, Function::sqr)));
    functionToken.assign(FUN_RND, TokenDefinition("RND",    new Token(TOKEN_TYPE_FUNCTION, Function::rnd)));
    functionToken.assign(FUN_ASC, TokenDefinition("ASC",    new Token(TOKEN_TYPE_FUNCTION, Function::asc)));
    functionToken.assign(FUN_LEN, TokenDefinition("LEN",    new Token(TOKEN_TYPE_FUNCTION, Function::len)));
    functionToken.assign(FUN_VAL, TokenDefinition("VAL",    new Token(TOKEN_TYPE_FUNCTION, Function::val)));
    functionToken.assign(FUN_CHR, TokenDefinition("CHR$",   new Token(TOKEN_TYPE_FUNCTION, Function::chr)));
    functionToken.assign(FUN_STR, TokenDefinition("STR$",   new Token(TOKEN_TYPE_FUNCTION, Function::str)));
    functionToken.assign(FUN_LEFT, TokenDefinition("LEFT$",  new Token(TOKEN_TYPE_FUNCTION, Function::left)));
    functionToken.assign(FUN_RIGHT, TokenDefinition("RIGHT$", new Token(TOKEN_TYPE_FUNCTION, Function::right)));
    functionToken.assign(FUN_MID, TokenDefinition("MID$",   new Token(TOKEN_TYPE_FUNCTION, Function::mid)));
    functionToken.assign(FUN_ABS, TokenDefinition("ABS",    new Token(TOKEN_TYPE_FUNCTION, Function::abs)));
    functionToken.assign(FUN_SGN, TokenDefinition("SGN",    new Token(TOKEN_TYPE_FUNCTION, Function::sgn)));
    functionToken.assign(FUN_EXP, TokenDefinition("EXP",    new Token(TOKEN_TYPE_FUNCTION, Function::exp)));
    functionToken.assign(FUN_LOG, TokenDefinition("LOG",    new Token(TOKEN_TYPE_FUNCTION, Function::log)));
    functionToken.assign(FUN_INT, TokenDefinition("INT",    new Token(TOKEN_TYPE_FUNCTION, Function::xint)));

    commandToken.assign(CMD_PRINT, TokenDefinition("PRINT", new Token(TOKEN_TYPE_COMMAND)));

}

TokenDefinition::TokenDefinition(std::string text, Token *token) {
    this->token = token;
    this->text  = text;
}