#include <cstdlib>
#include <map>
#include <stack>

#ifndef TOKEN_H
#define TOKEN_H
class Token {
public:
    Token(short tokenType);
    Token(short tokenType, double tokenValue);
    Token(short tokenType, short precedence, short assoc, void (*funcptr)(std::stack<Token>*));
    short getType();
    short getPrecedence();
    short getAssoc();
    void call(std::stack<Token>*);
    double getValue();
    // ~Token() { std::cout << "Kaputt" << tokenValue << std::endl; }
protected:
    short tokenType;
    short precedence;
    short assoc;
    void (*funcptr)(std::stack<Token>*);
    double tokenValue;
};

enum {
    TOKEN_TYPE_VALUE, TOKEN_TYPE_FUNCTION, TOKEN_TYPE_SEPERATOR, TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_BRACKETOPEN, TOKEN_TYPE_BRACKETCLOSE, TOKEN_TYPE_UNKNOWN,
    TOKEN_TYPE_COMMAND
};

enum {
    LEFT, RIGHT
};

typedef std::map<std::string, Token*> TokenMap;

class Parser
{
private:
    static Parser* instance;
    unsigned char* inputPtr;

    TokenMap commandToken;
    TokenMap functionToken;
    TokenMap operatorToken;
    TokenMap unaryOperatorToken;
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
    Token* findToken(TokenMap map);
};

#endif