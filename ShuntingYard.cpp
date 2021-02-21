//
// Created by tdix on 07.02.2021.
//
#include <stack>
#include <iostream>
#include "ShuntingYard.h"
#include "Token.h"
#include "utils.h"

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

Value ShuntingYard::run(unsigned char* input) {

    std::stack<Token> tokenStack;
    OutputBuffer outputBuffer;

    Token *token;

    Parser *parser = Parser::getInstance(input);
    bool unary{true};

    while(true) {
        token = parser->getNextToken(unary);
        if (token->getType()==TOKEN_TYPE_UNKNOWN) {
            break;
        }
        switch(token->getType()) {
            case TOKEN_TYPE_VALUE:
                unary=false;
                outputBuffer.push(*token);
                break;
            case TOKEN_TYPE_FUNCTION:
                unary=true;
                tokenStack.push(*token);
                break;
            case TOKEN_TYPE_SEPERATOR:
                unary=true;
                do {
                    if (tokenStack.empty()) {
                        throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
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
                unary=true;
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
                unary=true;
                tokenStack.push(*token);
                break;
            case TOKEN_TYPE_BRACKETCLOSE:
                unary=false;
                do {
                    if (tokenStack.empty()) {
                        // error = ERROR_MISSING_BRACKET_OPEN;
                        throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
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

    while (!tokenStack.empty()) {
        if (tokenStack.top().getType()==TOKEN_TYPE_BRACKETOPEN) {
            // error = ERROR_TOO_MANY_BRACKETS_OPEN;
            throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
        }
        outputBuffer.push(tokenStack.top());
        tokenStack.pop();
    };
    if (!outputBuffer.empty()) {
        Value v = outputBuffer.top().getValue();
        outputBuffer.pop();
        return v;
    }
    throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
}
