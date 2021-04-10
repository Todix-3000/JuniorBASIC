#ifndef TOKEN_H
#define TOKEN_H

#include <cstdlib>
#include <map>
#include <stack>
#include <vector>
#include "Value.h"


class Token {
public:
    Token(short tokenType);
    Token(Value tokenValue);
    Token(short tokenType, short precedence, short assoc, void (*funcptr)(std::stack<Token>*));
    Token(short tokenType, void (*funcptr)(std::stack<Token>*));
    Token(short tokenType, unsigned char* (*cmdptr)(unsigned char*));
    Token(short tokenType, std::string varName, short varType);
    short getType();
    short getPrecedence();
    short getAssoc();
    void call(std::stack<Token>*);
    unsigned char* call(unsigned char*);

    void fetchArrayValue(std::stack<Token>*);
    Value getValue();

protected:

    short tokenType;
    short precedence;
    short assoc;
    void (*funcptr)(std::stack<Token>*);
    unsigned char* (*cmdptr)(unsigned char*);
    Value tokenValue;
};

enum {
    TOKEN_TYPE_VALUE, TOKEN_TYPE_FUNCTION, TOKEN_TYPE_SEPERATOR, TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_BRACKETOPEN, TOKEN_TYPE_BRACKETCLOSE, TOKEN_TYPE_UNKNOWN,
    TOKEN_TYPE_COMMAND, TOKEN_TYPE_ARRAY
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
// using VarDefinition = struct {std::string varName; short varType;};
typedef struct {std::string varName; short varType;} VarDefinition;
class Parser
{
private:
    static Parser* instance;
    int bracketLevel;

    TokenVector tokenList;
    TokenVector unaryOperatorToken;
    Token* seperatorToken;
    Token* bracketOpenToken;
    Token* bracketCloseToken;

protected:
    Parser();

public:
    /* Static access method. */
    unsigned char* inputPtr;
    static Parser* getInstance(unsigned char * input);
    void setInputPtr(unsigned char * input);
    Token* getNextTokenForExpression(bool unaryOperator);
    Token* findToken(TokenVector map);
    std::string findTokenName(unsigned char, short &tokenType);
    Token* findVariable();
    unsigned char* call(unsigned char* restOfLine);

    unsigned char getTokenId(size_t &tokenLength);
    VarDefinition getVariableDefinition();

    Value getLiteralValue();
};

enum {
    OP_POW = '^',
    OP_MUL = '*',
    OP_DIV = '/',
    OP_MOD = '%',
    OP_ADD = '+',
    OP_SUB = '-',
    OP_LT  = '<',
    OP_GT  = '>',
    OP_EQ  = '=',
    OP_BITAND = 128,
    OP_BITXOR,
    OP_BITOR,
    OP_BOOLAND,
    OP_BOOLOR,
    OP_LEQT,
    OP_GEQT,
    OP_NEQ,
    CMD_PRINT,
    CMD_REM,
    CMD_RUN,
    CMD_GOTO,
    CMD_IF,
    CMD_LIST,
    CMD_LET,
    CMD_CLOSE,
    CMD_CLR,
    CMD_CONT,
    CMD_DATA,
    CMD_DIM,
    CMD_END,
    CMD_FOR,
    CMD_GET,
    CMD_GOSUB,
    CMD_INPUT,
    CMD_LOAD,
    CMD_NEW,
    CMD_NEXT,
    CMD_ON,
    CMD_OPEN,
    CMD_READ,
    CMD_RETURN,
    CMD_SAVE,
    CMD_STOP,
    CMD_STEP,
    CMD_THEN,
    CMD_TO,
    CMD_RESTORE,
    CMD_DIR,
    CMD_KILL,
    CMD_CHDIR,
    CMD_MKDIR,
    CMD_RMDIR,
    FUN_SIN,
    FUN_COS,
    FUN_TAN,
    FUN_ATN,
    FUN_SQR,
    FUN_RND,
    FUN_ASC,
    FUN_LEN,
    FUN_VAL,
    FUN_CHR,
    FUN_STR,
    FUN_LEFT,
    FUN_RIGHT,
    FUN_MID,
    FUN_ABS,
    FUN_SGN,
    FUN_EXP,
    FUN_LOG,
    FUN_INT
};
#endif