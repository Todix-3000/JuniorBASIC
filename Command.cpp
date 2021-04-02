//
// Created by tdix on 30.03.2021.
//

#include "Command.h"
#include "ShuntingYard.h"
#include "Program.h"
#include "Token.h"
#include "utils.h"
#include "Variable.h"
#include <iostream>

unsigned char* Command::_goto(unsigned char* restOfLine) {
    std::cout << "#GOTO\n";
    return restOfLine;
}

unsigned char* Command::list(unsigned char* restOfLine) {
    Program *code = Program::getInstance();
    unsigned short start = 0;
    unsigned short end = USHRT_MAX;
    short paramCount = 0;
    while (*restOfLine != 0 && *restOfLine != ':') {
        bool ok = false;
        if (*restOfLine == '-') {
            paramCount++;
            if (paramCount>1) {
                throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
            }
            ok = true;
            restOfLine++;
        }
        int param=0;
        try {
            size_t len;
            std::string line((char *) restOfLine);
            param = std::stoi(line, &len);
            if (param<0 || param>USHRT_MAX) {
                throw Exception(EXCEPTION_RANGE_ERROR);
            }
            restOfLine += len;
            if (paramCount==0) {
                start = param;
            } else {
                end = param;
            }
            ok = true;
        } catch (std::invalid_argument e) {
        }
        if (!ok) {
            throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
        }

    }
    code->resetLinePointer();
    unsigned short lineNumber;
    std::string codeLine;
    do {
        codeLine = code->getNextLine(lineNumber);
        if (lineNumber!=0 && lineNumber>=start && lineNumber<=end) {
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
    bool lineFeed = true;
    while (*restOfLine != 0 && *restOfLine != ':') {
        if (*restOfLine == ';') {
            restOfLine++;
            lineFeed = false;
        } else if (*restOfLine == ',') {
            restOfLine++;
            lineFeed = false;
            std::cout << ' ';
        } else {
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
            lineFeed = true;
        }
    }
    if (lineFeed) {
        std::cout << std::endl;
    }
    return restOfLine;
}

unsigned char* Command::rem(unsigned char* restOfLine) {
    while (*restOfLine != 0) { restOfLine++; }
    return restOfLine;
}

unsigned char* Command::let(unsigned char* restOfLine) {
    Parser* parser = Parser::getInstance(restOfLine);
    auto varDef = parser->getVariableDefinition();
    restOfLine = parser->inputPtr;
    std::vector<int> index;
    if (*restOfLine=='(') {
        restOfLine++;
        bool finished = false;
        do {
            Value result;
            restOfLine = (new ShuntingYard())->run(restOfLine, result);
            index.push_back(result.getInt());
            if (*restOfLine == ',') {
                restOfLine++;
            } else if(*restOfLine==')') {
                restOfLine++;
                finished=true;
            } else {
                throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
            }
        } while (!finished);
    }
    if (*restOfLine!='=') {
        throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
    }
    restOfLine++;
    Value result;
    restOfLine = (new ShuntingYard())->run(restOfLine, result);
    if (result.getType()!= varDef.varType) {
        if (varDef.varType==VALUE_TYPE_FLOAT) {
            result = Value(result.getFloat());
        } else if (varDef.varType==VALUE_TYPE_INT) {
            result = Value(result.getInt());
        } else {
            result = Value(result.getString());
        }

    }
    if (index.size()==0) {
        Variable::getContainer()->setValue(varDef.varName, result);
    } else {
        Variable::getContainer()->setValue(varDef.varName, index, result);
    }

    return restOfLine;
}


