#include <cstdlib>
#include <map>
#include <stack>
#include <vector>
#include "Value.h"

#ifndef TOKEN_H
#define TOKEN_H

class Token {
public:
    Token(short tokenType);
    Token(Value tokenValue);
    Token(short tokenType, short precedence, short assoc, void (*funcptr)(std::stack<Token>*));
    Token(short tokenType, void (*funcptr)(std::stack<Token>*));
    Token(short tokenType, void (*funcptr)(std::stack<Token>*), std::string varName);
    short getType();
    short getPrecedence();
    short getAssoc();
    void call(std::stack<Token>*);
    Value getValue();
    // ~Token() { std::cout << "Kaputt" << tokenValue << std::endl; }
protected:
    short tokenType;
    short precedence;
    short assoc;
    void (*funcptr)(std::stack<Token>*);
    Value tokenValue;
};

enum {
    TOKEN_TYPE_VALUE, TOKEN_TYPE_FUNCTION, TOKEN_TYPE_SEPERATOR, TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_BRACKETOPEN, TOKEN_TYPE_BRACKETCLOSE, TOKEN_TYPE_UNKNOWN,
    TOKEN_TYPE_COMMAND
};

enum {
    LEFT, RIGHT
};

class TokenDefinition {
public:
    std::string text;
    Token *token;
    TokenDefinition(std::string text, Token *token);
};

using TokenVector = std::vector<TokenDefinition>;

class Parser
{
private:
    static Parser* instance;
    unsigned char* inputPtr;

    TokenVector commandToken;
    TokenVector functionToken;
    TokenVector operatorToken;
    TokenVector unaryOperatorToken;
    Token* seperatorToken;
    Token* bracketOpenToken;
    Token* bracketCloseToken;

protected:
    Parser();

public:
    /* Static access method. */
    static Parser* getInstance(unsigned char * input);
    void setInputPtr(unsigned char * input);
    Token* getNextToken(bool unaryOperator);
    Token* findToken(TokenVector map);
    Token* findVariable();
};

#endif