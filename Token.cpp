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
#include "Command.h"
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

Token::Token(short tokenType, unsigned char* (*cmdptr)(unsigned char*)) {
    this->tokenType  = tokenType;
    this->cmdptr     = cmdptr;
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

Value Parser::getLiteralValue() {
    if (*inputPtr >= '0' && *inputPtr <= '9') {
        size_t intLength;
        size_t doubleLength;
        int intValue = std::stoi((char*)inputPtr, &intLength);
        double doubleValue = std::stod((char*)inputPtr, &doubleLength);
        if (doubleLength>intLength) {
            inputPtr = &inputPtr[doubleLength];
            return Value(doubleValue);
        }
        inputPtr = &inputPtr[intLength];
        return Value(intValue);
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
        return Value(std::string(subbuff));
    }
    throw NotFount();
}

Token* Parser::getNextTokenForExpression(bool unaryOperator) {
    Token *myToken;
    while (' ' == *inputPtr) {
        inputPtr++;
    }
    try {
        return new Token(getLiteralValue());
    } catch (NotFount e) {}

    if (unaryOperator) {
        if ((myToken = findToken(unaryOperatorToken)) != nullptr) {
            return myToken;
        }
    }
    if ((myToken = findToken(tokenList)) != nullptr) {
        if (myToken->getType()!=TOKEN_TYPE_COMMAND) {
            return myToken;
        }
        inputPtr--;
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

std::string Parser::findTokenName(unsigned char tokenId, short &tokenType) {
    std::string result = tokenList[tokenId].text;
    tokenType = (result=="") ? -1 : tokenList[tokenId].token->getType();
    return result;
}

Token* Parser::findToken(TokenVector map) {
    auto value = inputPtr[0];
    if (value<map.size()) {
        if (map[value].text!="") {
            inputPtr++;
            return map[value].token;
        }
    }

    return nullptr;
}

unsigned char Parser::getTokenId(size_t &tokenLength) {
    TokenVector maps[] = {tokenList};
    tokenLength = 0;
    for (auto const& map : maps ) {
        // for (int i=0; i<map.size(); i++) {
        for (int i=map.size()-1; i>=0; i--) {
            auto tok = map.at(i);
            if (tok.token != nullptr) {
                if ( memcmp (inputPtr, tok.text.data(), tok.text.length()) == 0 ) {
                    tokenLength = tok.text.length();
                    return i;
                }
            }
        }
    }
    return 0;
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

unsigned char* Token::call(unsigned char* restOfLine) {
    return cmdptr(restOfLine);
}

unsigned char* Parser::call(unsigned char* restOfLine) {
    Token* myToken;
    if (tokenList[*restOfLine].text!="") {
        myToken =  tokenList[*restOfLine].token;
        restOfLine++;
    } else {
        myToken =  tokenList[CMD_LET].token;
    }
    return myToken->call(restOfLine);
}

void Token::fetchArrayValue(std::stack<Token>* tokenStack) {
    Function::getArrayVariableValue(tokenStack, tokenValue.getString(), assoc);
}

Parser::Parser() {
    unaryOperatorToken.assign(127, TokenDefinition("", nullptr));

    unaryOperatorToken[OP_ADD] = TokenDefinition("+", new Token(TOKEN_TYPE_OPERATOR, 13, LEFT, Operator::dummy));
    unaryOperatorToken[OP_SUB] = TokenDefinition("-", new Token(TOKEN_TYPE_OPERATOR, 13, LEFT, Operator::neg));

    seperatorToken    = new Token(TOKEN_TYPE_SEPERATOR);
    bracketOpenToken  = new Token(TOKEN_TYPE_BRACKETOPEN);
    bracketCloseToken = new Token(TOKEN_TYPE_BRACKETCLOSE);

    tokenList.assign(255, TokenDefinition("", nullptr));

    tokenList[OP_POW]     = TokenDefinition("^",  new Token(TOKEN_TYPE_OPERATOR, 12, RIGHT, Operator::pow));
    tokenList[OP_MUL]     = TokenDefinition("*",  new Token(TOKEN_TYPE_OPERATOR, 11, LEFT, Operator::mul));
    tokenList[OP_DIV]     = TokenDefinition("/",  new Token(TOKEN_TYPE_OPERATOR, 11, LEFT, Operator::div));
    tokenList[OP_MOD]     = TokenDefinition("%",  new Token(TOKEN_TYPE_OPERATOR, 11, LEFT, Operator::mod));
    tokenList[OP_ADD]     = TokenDefinition("+",  new Token(TOKEN_TYPE_OPERATOR, 10, LEFT, Operator::add));
    tokenList[OP_SUB]     = TokenDefinition("-",  new Token(TOKEN_TYPE_OPERATOR, 10, LEFT, Operator::sub));
    tokenList[OP_LEQT]    = TokenDefinition("<=", new Token(TOKEN_TYPE_OPERATOR, 8, LEFT, Operator::leqt));
    tokenList[OP_GEQT]    = TokenDefinition(">=", new Token(TOKEN_TYPE_OPERATOR, 8, LEFT, Operator::geqt));
    tokenList[OP_NEQ]     = TokenDefinition("<>", new Token(TOKEN_TYPE_OPERATOR, 6, LEFT, Operator::neq));
    tokenList[OP_LT]      = TokenDefinition("<",  new Token(TOKEN_TYPE_OPERATOR, 8, LEFT, Operator::lt));
    tokenList[OP_GT]      = TokenDefinition(">",  new Token(TOKEN_TYPE_OPERATOR, 8, LEFT, Operator::gt));
    tokenList[OP_EQ]      = TokenDefinition("=",  new Token(TOKEN_TYPE_OPERATOR, 6, LEFT, Operator::eq));
    tokenList[OP_BITAND]  = TokenDefinition("AND",new Token(TOKEN_TYPE_OPERATOR, 5, LEFT, Operator::bitAnd));
    tokenList[OP_BITXOR]  = TokenDefinition("XOR",new Token(TOKEN_TYPE_OPERATOR, 4, LEFT, Operator::bitXor));
    tokenList[OP_BITOR]   = TokenDefinition("OR", new Token(TOKEN_TYPE_OPERATOR, 3, LEFT, Operator::bitOr));
    tokenList[OP_BOOLAND] = TokenDefinition("&&", new Token(TOKEN_TYPE_OPERATOR, 2, LEFT, Operator::boolAnd));
    tokenList[OP_BOOLOR]  = TokenDefinition("||", new Token(TOKEN_TYPE_OPERATOR, 1, LEFT, Operator::boolOr));

    tokenList[FUN_SIN]   = TokenDefinition("SIN",    new Token(TOKEN_TYPE_FUNCTION, Function::sin));
    tokenList[FUN_COS]   = TokenDefinition("COS",    new Token(TOKEN_TYPE_FUNCTION, Function::cos));
    tokenList[FUN_TAN]   = TokenDefinition("TAN",    new Token(TOKEN_TYPE_FUNCTION, Function::tan));
    tokenList[FUN_ATN]   = TokenDefinition("ATN",    new Token(TOKEN_TYPE_FUNCTION, Function::atn));
    tokenList[FUN_SQR]   = TokenDefinition("SQR",    new Token(TOKEN_TYPE_FUNCTION, Function::sqr));
    tokenList[FUN_RND]   = TokenDefinition("RND",    new Token(TOKEN_TYPE_FUNCTION, Function::rnd));
    tokenList[FUN_ASC]   = TokenDefinition("ASC",    new Token(TOKEN_TYPE_FUNCTION, Function::asc));
    tokenList[FUN_LEN]   = TokenDefinition("LEN",    new Token(TOKEN_TYPE_FUNCTION, Function::len));
    tokenList[FUN_VAL]   = TokenDefinition("VAL",    new Token(TOKEN_TYPE_FUNCTION, Function::val));
    tokenList[FUN_CHR]   = TokenDefinition("CHR$",   new Token(TOKEN_TYPE_FUNCTION, Function::chr));
    tokenList[FUN_STR]   = TokenDefinition("STR$",   new Token(TOKEN_TYPE_FUNCTION, Function::str));
    tokenList[FUN_LEFT]  = TokenDefinition("LEFT$",  new Token(TOKEN_TYPE_FUNCTION, Function::left));
    tokenList[FUN_RIGHT] = TokenDefinition("RIGHT$", new Token(TOKEN_TYPE_FUNCTION, Function::right));
    tokenList[FUN_MID]   = TokenDefinition("MID$",   new Token(TOKEN_TYPE_FUNCTION, Function::mid));
    tokenList[FUN_ABS]   = TokenDefinition("ABS",    new Token(TOKEN_TYPE_FUNCTION, Function::abs));
    tokenList[FUN_SGN]   = TokenDefinition("SGN",    new Token(TOKEN_TYPE_FUNCTION, Function::sgn));
    tokenList[FUN_EXP]   = TokenDefinition("EXP",    new Token(TOKEN_TYPE_FUNCTION, Function::exp));
    tokenList[FUN_LOG]   = TokenDefinition("LOG",    new Token(TOKEN_TYPE_FUNCTION, Function::log));
    tokenList[FUN_INT]   = TokenDefinition("INT",    new Token(TOKEN_TYPE_FUNCTION, Function::xint));

    tokenList[CMD_PRINT] = TokenDefinition("PRINT", new Token(TOKEN_TYPE_COMMAND, Command::print));
    tokenList[CMD_REM]   = TokenDefinition("REM",   new Token(TOKEN_TYPE_COMMAND, Command::rem));
    tokenList[CMD_LIST]  = TokenDefinition("LIST",  new Token(TOKEN_TYPE_COMMAND, Command::list));
    tokenList[CMD_GOTO]  = TokenDefinition("GOTO",  new Token(TOKEN_TYPE_COMMAND, Command::_goto));
    tokenList[CMD_LET]   = TokenDefinition("LET",   new Token(TOKEN_TYPE_COMMAND, Command::let));
    tokenList[CMD_RUN]   = TokenDefinition("RUN",   new Token(TOKEN_TYPE_COMMAND, Command::run));
    tokenList[CMD_IF]    = TokenDefinition("IF",    new Token(TOKEN_TYPE_COMMAND, Command::_if));
    tokenList[CMD_THEN]  = TokenDefinition("THEN",  new Token(TOKEN_TYPE_COMMAND, Command::illegal));
    tokenList[CMD_LOAD]  = TokenDefinition("LOAD",  new Token(TOKEN_TYPE_COMMAND, Command::load));
    tokenList[CMD_SAVE]  = TokenDefinition("SAVE",  new Token(TOKEN_TYPE_COMMAND, Command::save));
    tokenList[CMD_NEW]   = TokenDefinition("NEW",   new Token(TOKEN_TYPE_COMMAND, Command::_new));
    tokenList[CMD_END]   = TokenDefinition("END",   new Token(TOKEN_TYPE_COMMAND, Command::end));
    tokenList[CMD_STOP]  = TokenDefinition("STOP",  new Token(TOKEN_TYPE_COMMAND, Command::stop));
    tokenList[CMD_CONT]  = TokenDefinition("CONT",  new Token(TOKEN_TYPE_COMMAND, Command::cont));
    tokenList[CMD_CLR]   = TokenDefinition("CLR",   new Token(TOKEN_TYPE_COMMAND, Command::clr));
    tokenList[CMD_INPUT] = TokenDefinition("INPUT", new Token(TOKEN_TYPE_COMMAND, Command::input));
    tokenList[CMD_CLOSE] = TokenDefinition("CLOSE", new Token(TOKEN_TYPE_COMMAND, Command::close));
    tokenList[CMD_DATA]  = TokenDefinition("DATA",  new Token(TOKEN_TYPE_COMMAND, Command::data));
    tokenList[CMD_DIM]   = TokenDefinition("DIM",   new Token(TOKEN_TYPE_COMMAND, Command::dim));
    tokenList[CMD_FOR]   = TokenDefinition("FOR",   new Token(TOKEN_TYPE_COMMAND, Command::_for));
    tokenList[CMD_GET]   = TokenDefinition("GET",   new Token(TOKEN_TYPE_COMMAND, Command::get));
    tokenList[CMD_GOSUB] = TokenDefinition("GOSUB", new Token(TOKEN_TYPE_COMMAND, Command::gosub));
    tokenList[CMD_NEXT]  = TokenDefinition("NEXT",  new Token(TOKEN_TYPE_COMMAND, Command::next));
    tokenList[CMD_ON]    = TokenDefinition("ON",    new Token(TOKEN_TYPE_COMMAND, Command::on));
    tokenList[CMD_OPEN]  = TokenDefinition("OPEN",  new Token(TOKEN_TYPE_COMMAND, Command::open));
    tokenList[CMD_READ]  = TokenDefinition("READ",  new Token(TOKEN_TYPE_COMMAND, Command::read));
    tokenList[CMD_RETURN]= TokenDefinition("RETURN",new Token(TOKEN_TYPE_COMMAND, Command::_return));
    tokenList[CMD_STEP]  = TokenDefinition("STEP",  new Token(TOKEN_TYPE_COMMAND, Command::illegal));
    tokenList[CMD_TO]    = TokenDefinition("TO",    new Token(TOKEN_TYPE_COMMAND, Command::illegal));
    tokenList[CMD_RESTORE]= TokenDefinition("RESTORE",new Token(TOKEN_TYPE_COMMAND, Command::restore));
}

TokenDefinition::TokenDefinition(std::string text, Token *token) {
    this->token = token;
    this->text  = text;
}