//
// Created by tdix on 07.02.2021.
//
#include <stack>
#include "ShuntingYard.h"
#include "Token.h"

ShuntingYard::ShuntingYard() {
}

void ShuntingYard::run(unsigned char* input, int &error) {

    std::stack<Token> tokenStack;
    std::stack<Token> outputBuffer;

    Token *token;

    error = ERROR_OK;

    while(1) {
        token = Parser::getInstance()->getNextToken(input);
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
                /*
                BIS Stack-Spitze IST öffnende-Klammer:
                Stack-Spitze ZU Ausgabe.
                FEHLER-BEI Stack IST-LEER:
                GRUND (1) Ein falsch platziertes Argumenttrennzeichen.
                GRUND (2) Der schließenden Klammer geht keine öffnende voraus.
                ENDEFEHLER
                ENDEBIS
                */
                break;
            case TOKEN_TYPE_OPERATOR:
                while (!tokenStack.empty() &&
                  tokenStack.top().getType()==TOKEN_TYPE_OPERATOR &&
                  tokenStack.top().getAssoc()==LEFT &&
                  token->getPrecedence()<=tokenStack.top().getPrecedence()) {
                    outputBuffer.push(tokenStack.top());
                    tokenStack.pop();
                }
                tokenStack.push(*token);
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
