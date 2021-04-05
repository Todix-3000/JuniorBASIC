//
// Created by tdix on 30.03.2021.
//

#include "Command.h"
#include "ShuntingYard.h"
#include "Program.h"
#include "Token.h"
#include "utils.h"
#include "Variable.h"
#include "Global.h"
#include "Tokenizer.h"
#include <iostream>
#include <fstream>

unsigned char* Command::_goto(unsigned char* restOfLine) {
    int param;
    try {
        size_t len;
        std::string line((char *) restOfLine);
        param = std::stoi(line, &len);
        if (param<0 || param>USHRT_MAX) {
            throw Exception(EXCEPTION_RANGE_ERROR);
        }
        restOfLine += len;
        if (!Program::getInstance()->setProgramCounter(param)) {
            throw Exception(EXCEPTION_UNKNOWN_LINE);
        }
    } catch (std::invalid_argument e) {
        throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
    }
    Global::getInstance()->setRunMode();
    return Program::getInstance()->getProgramLineCounter();
}

void Command::__list(unsigned short start, unsigned short end, std::basic_ostream<char> *stream) {
    Program *code = Program::getInstance();
    code->resetLinePointer();
    unsigned short lineNumber;
    std::string codeLine;
    do {
        codeLine = code->getNextLine(lineNumber);
        if (lineNumber!=0 && lineNumber>=start && lineNumber<=end) {
            std::string outputLine = "";
            bool insertSpace = false;
            short type;
            bool firstChar = true;
            for (auto c : codeLine) {
                std::string tokenName = Parser::getInstance(nullptr)->findTokenName((unsigned char) c, type);
                if (insertSpace) { outputLine += ' '; insertSpace = false; firstChar = true;}
                if (tokenName.length()>0) {
                    if (type == TOKEN_TYPE_COMMAND) {
                        insertSpace = true;
                        if (!firstChar) {
                            outputLine += ' ';
                        }
                    }
                    outputLine += tokenName;
                } else {
                    outputLine += c;
                }
                firstChar = false;
            }
            *stream << lineNumber << ' ' << outputLine << std::endl;
        }
    } while (lineNumber!=0);
}
unsigned char* Command::list(unsigned char* restOfLine) {
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
    __list(start, end, &std::cout);

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

unsigned char *Command::_if(unsigned char *restOfLine) {
    ShuntingYard *algorithm = new ShuntingYard();
    Value result;
    restOfLine = algorithm->run(restOfLine, result);
    bool isTrue = false;
    switch (result.getType()) {
        case VALUE_TYPE_INT:
            isTrue = result.getInt() != 0;
            break;
        case VALUE_TYPE_FLOAT:
            isTrue = result.getFloat() != 0.0;
            break;
        case VALUE_TYPE_STRING:
            isTrue = result.getString().length() > 0;
            break;
    }
    if (isTrue) {
        if (*restOfLine == CMD_GOTO) {
            restOfLine++;
            return _goto(restOfLine);
        } else if (*restOfLine == CMD_THEN) {
            restOfLine++;
            return restOfLine;
        }
    } else {
        while (*restOfLine != 0) { restOfLine++; }
    }
    return restOfLine;
}

unsigned char *Command::run(unsigned char *restOfLine) {
    Variable::getContainer()->clearAll();
    Global::getInstance()->setRunMode();
    Program::getInstance()->resetProgramCounter();
    return restOfLine;
}

unsigned char *Command::illegal(unsigned char *restOfLine) {
    throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
}

unsigned char *Command::_new(unsigned char *restOfLine) {
    Program::getInstance()->clear();
    restOfLine = (unsigned char*) "";
    return restOfLine;
}

unsigned char *Command::end(unsigned char *restOfLine) {
    Global::getInstance()->setDirectMode();
    return restOfLine;
}

unsigned char *Command::stop(unsigned char *restOfLine) {
    Global::getInstance()->setDirectMode();
    std::cout << "BREAK IN " << Program::getInstance()->getProgramCounter() << std::endl;
    return restOfLine;
}

unsigned char *Command::cont(unsigned char *restOfLine) {
    Global::getInstance()->setRunMode();
    return restOfLine;
}

unsigned char *Command::clr(unsigned char *restOfLine) {
    Variable::getContainer()->clearAll();
    return restOfLine;
}

unsigned char *Command::input(unsigned char *restOfLine) {
    if (*restOfLine == '"') {
        ShuntingYard *algorithm = new ShuntingYard();
        Value result;
        restOfLine = ShuntingYard().run(restOfLine, result);
        std::cout << result.getString();
        if (*restOfLine == ';') {
            restOfLine++;
        } else {
            throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
        }
    }
    bool lastVar = false;
    do {
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
        std::cout << '?';
        std::string line;
        std::getline(std::cin, line);

        Value result;

        try {
            switch (varDef.varType) {
                case VALUE_TYPE_STRING:
                    result = Value(line);
                    break;
                case VALUE_TYPE_INT:
                    result = Value(std::stoi(line));
                    break;
                case VALUE_TYPE_FLOAT:
                    result = Value(std::stod(line));
                    break;
            }
        } catch (std::invalid_argument e) {
            throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
        }
        if (index.size()==0) {
            Variable::getContainer()->setValue(varDef.varName, result);
        } else {
            Variable::getContainer()->setValue(varDef.varName, index, result);
        }

        if (*restOfLine==',') {
            restOfLine++;
        } else {
            lastVar = true;
        }
    } while (!lastVar);
    return restOfLine;
}

unsigned char *Command::close(unsigned char *restOfLine) {
    return restOfLine;
}

unsigned char *Command::data(unsigned char *restOfLine) {
    return restOfLine;
}

unsigned char *Command::dim(unsigned char *restOfLine) {
    return restOfLine;
}

unsigned char *Command::_for(unsigned char *restOfLine) {
    return restOfLine;
}

unsigned char *Command::get(unsigned char *restOfLine) {
    return restOfLine;
}

unsigned char *Command::gosub(unsigned char *restOfLine) {
    int param;
    try {
        size_t len;
        std::string line((char *) restOfLine);
        param = std::stoi(line, &len);
        if (param<0 || param>USHRT_MAX) {
            throw Exception(EXCEPTION_RANGE_ERROR);
        }
        restOfLine += len;
        StackEntry stackEntry;
        stackEntry.type = STACK_TYPE_GOSUB;
        stackEntry.runMode = Global::getInstance()->isRunMode();
        stackEntry.programLineCounter = restOfLine;
        stackEntry.programCounter = Program::getInstance()->getProgramCounter();
        Program::getInstance()->stackPush(stackEntry);
        if (!Program::getInstance()->setProgramCounter(param)) {
            throw Exception(EXCEPTION_UNKNOWN_LINE);
        }
    } catch (std::invalid_argument e) {
        throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
    }
    Global::getInstance()->setRunMode();
    return Program::getInstance()->getProgramLineCounter();
}

unsigned char *Command::on(unsigned char *restOfLine) {
    return restOfLine;
}

unsigned char *Command::open(unsigned char *restOfLine) {
    return restOfLine;
}

unsigned char *Command::read(unsigned char *restOfLine) {
    return restOfLine;
}

unsigned char *Command::_return(unsigned char *restOfLine) {
    Program *code = Program::getInstance();
    if (code->stackEmpty() || code->stackTop().type!=STACK_TYPE_GOSUB) {
        throw Exception(EXCEPTION_RETURN_WITHOUT_GOSUB);
    }
    StackEntry stackEntry = code->stackTop();
    code->stackPop();
    code->setProgramCounter(stackEntry.programCounter);
    if (!stackEntry.runMode) {
        Global::getInstance()->setDirectMode();
    }
    return stackEntry.programLineCounter;
}

unsigned char *Command::wait(unsigned char *restOfLine) {
    return restOfLine;
}

unsigned char *Command::next(unsigned char *restOfLine) {
    return restOfLine;
}

unsigned char *Command::load(unsigned char *restOfLine) {
    Value result;

    restOfLine = ShuntingYard().run(restOfLine, result);
    std::ifstream in(result.getString(), std::ios::in);
    if (!in.is_open()) {
        throw Exception(EXCEPTION_FILE_READ);
    }
    Program *code = Program::getInstance();

    code->clear();
    Variable::getContainer()->clearAll();

    std::string line;
    while (std::getline(in, line)) {
        Tokenizer tokenizer = Tokenizer(line);
        if (tokenizer.isCodeline()) {
            auto lineNumber = tokenizer.getLineNumber();
            if (tokenizer.getLine().length()) {
                code->setLine(tokenizer.getLineNumber(), tokenizer.getLine());
            }
        }
    }
    code->resetProgramCounter();
    restOfLine = (unsigned char*) "";
    return restOfLine;
}

unsigned char *Command::save(unsigned char *restOfLine) {
    Value result;
    restOfLine = ShuntingYard().run(restOfLine, result);
    std::ofstream out(result.getString(), std::ios::trunc | std::ios::out);
    if (!out.is_open()) {
        throw Exception(EXCEPTION_FILE_WRITE);
    }
    __list(0, USHRT_MAX, &out);
    return restOfLine;
}



