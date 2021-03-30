//
// Created by tdix on 30.03.2021.
//

#include "Command.h"
#include "ShuntingYard.h"
#include "Program.h"
#include "Token.h"
#include <iostream>

unsigned char* Command::_goto(unsigned char* restOfLine) {
    std::cout << "#GOTO\n";
    return restOfLine;
}

unsigned char* Command::list(unsigned char* restOfLine) {
    Program *code = Program::getInstance();
    code->resetLinePointer();
    unsigned short lineNumber;
    std::string codeLine;
    do {
        codeLine = code->getNextLine(lineNumber);
        if (lineNumber!=0) {
            std::string outputLine = "";
            bool insertSpace = false;
            short type;
            for (auto c : codeLine) {
                if (insertSpace) { outputLine += ' '; insertSpace = false; }
                std::string tokenName = Parser::getInstance(nullptr)->findTokenName((unsigned char) c, type);
                if (tokenName.length()>0) {
                    outputLine += tokenName;
                    if (type == TOKEN_TYPE_COMMAND) { insertSpace = true; }
                } else {
                    outputLine += c;
                }
            }
            std::cout << lineNumber << ' ' << outputLine << std::endl;
        }
    } while (lineNumber!=0);

    return restOfLine;
}

unsigned char* Command::print(unsigned char* restOfLine) {
    ShuntingYard *algorithm = new ShuntingYard();
    Value result;
    restOfLine = algorithm->run(restOfLine, result);
    switch (result.getType()) {
        case VALUE_TYPE_INT:
            std::cout << result.getInt();
            break;
        case VALUE_TYPE_FLOAT:
            std::cout << result.getFloat();
            break;
        case VALUE_TYPE_STRING:
            std::cout << result.getString();
            break;
    }
    if (*restOfLine == ';') {
        restOfLine++;
    } else {
        std::cout << std::endl;
    }
    return restOfLine;
}

unsigned char* Command::rem(unsigned char* restOfLine) {
    std::cout << "#REM\n";
    return restOfLine;
}

unsigned char* Command::let(unsigned char* restOfLine) {
    std::cout << "#LET\n";
    return restOfLine;
}


