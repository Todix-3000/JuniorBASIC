//
// Created by tdix on 07.02.2021.
//
#include <stack>
#include <iostream>
#include "ShuntingYard.h"
#include "Token.h"

class OutputBuffer : public std::stack<Token>
{
public:
    void push(Token token) {
        if (token.getType()==TOKEN_TYPE_OPERATOR) {
            token.call(this);
            return;
        }
        std::stack<Token>::push(token);
    }
};

ShuntingYard::ShuntingYard() {
}

void ShuntingYard::run(unsigned char* input, int &error) {

    std::stack<Token> tokenStack;
    OutputBuffer outputBuffer;

    Token *token;

    error = ERROR_OK;

    Parser *parser = Parser::getInstance(input);

    while(true) {
        token = parser->getNextToken();
        if (token->getType()==TOKEN_TYPE_UNKNOWN) {
            break;
        }
        switch(token->getType()) {
            case TOKEN_TYPE_VALUE:
                outputBuffer.push(*token);
                break;
            case TOKEN_TYPE_FUNCTION:
                tokenStack.push(*token);
                break;
            case TOKEN_TYPE_SEPERATOR:
                do {
                    if (tokenStack.empty()) {
                        error = ERROR_MISSING_BRACKET_OPEN;
                        return;
                    }
                    if (tokenStack.top().getType() != TOKEN_TYPE_BRACKETOPEN) {
                        outputBuffer.push(tokenStack.top());
                        tokenStack.pop();
                    } else {
                        break;
                    }
                } while (true);
                break;
            case TOKEN_TYPE_OPERATOR:
                while (!tokenStack.empty() &&
                  tokenStack.top().getType()==TOKEN_TYPE_OPERATOR &&
                  token->getAssoc()==LEFT &&
                  token->getPrecedence()<=tokenStack.top().getPrecedence()) {
                    outputBuffer.push(tokenStack.top());
                    tokenStack.pop();
                }
                tokenStack.push(*token);

                // std::cout << "#" << outputBuffer.top().getValue() << "#\n";

                break;
            case TOKEN_TYPE_BRACKETOPEN:
                tokenStack.push(*token);
                break;
            case TOKEN_TYPE_BRACKETCLOSE:
                do {
                    if (tokenStack.empty()) {
                        error = ERROR_MISSING_BRACKET_OPEN;
                        return;
                    }
                    if (tokenStack.top().getType() != TOKEN_TYPE_BRACKETOPEN) {
                        outputBuffer.push(tokenStack.top());
                        tokenStack.pop();
                    }
                } while (tokenStack.top().getType() != TOKEN_TYPE_BRACKETOPEN);
                tokenStack.pop();
                if (tokenStack.top().getType() == TOKEN_TYPE_FUNCTION) {
                    outputBuffer.push(tokenStack.top());
                    tokenStack.pop();
                }
                break;
        }
    }

    do {
        if (tokenStack.top().getType()==TOKEN_TYPE_BRACKETOPEN) {
            error = ERROR_TOO_MANY_BRACKETS_OPEN;
            return;
        }
        outputBuffer.push(tokenStack.top());
        tokenStack.pop();
    } while (!tokenStack.empty());
}
