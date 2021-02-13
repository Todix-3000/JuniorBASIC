#include <cstdlib>
#include <map>
#include <stack>

#ifndef TOKEN_H
#define TOKEN_H
class Token {
public:
    Token(short tokenType);
    Token(short tokenType, short precedence, short assoc, void (*funcptr)(std::stack<Token>));
    short getType();
    short getPrecedence();
    short getAssoc();

private:
    short tokenType;
    short precedence;
    short assoc;
    void (*funcptr)(std::stack<Token>);
    int tokenValue;
};

enum {
    TOKEN_TYPE_VALUE, TOKEN_TYPE_FUNCTION, TOKEN_TYPE_SEPERATOR, TOKEN_TYPE_OPERATOR, TOKEN_TYPE_BRACKETOPEN, TOKEN_TYPE_BRACKETCLOSE, TOKEN_TYPE_UNKNOWN
};

enum {
    LEFT, RIGHT
};

typedef std::map<std::string, Token*> TokenMap;

class Parser
{
private:
    static Parser* instance;

    TokenMap commandToken;
    TokenMap functionToken;
    TokenMap operatorToken;

    Token* seperatorToken;
    Token* bracketOpenToken;
    Token* bracketCloseToken;

protected:
    Parser();

public:
    /* Static access method. */
    static Parser* getInstance();
    Token* getNextToken(unsigned char * input);
};

#endif