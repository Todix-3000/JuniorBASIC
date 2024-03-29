#include <iostream>
#include <stack>
#include "Token.h"
#include "Value.h"
#include "ShuntingYard.h"
#include "utils.h"
#include "Variable.h"
#include "Program.h"
#include "Tokenizer.h"
#include "Global.h"
#include "Console.h"
#include "Command.h"

bool isBreak = false;

void setBreak(int signalId) {
    isBreak = true;
    signal(SIGINT, setBreak);
}


int main(int argc, char* argv[]) {
    Console::init();
    Console::foregroundColor(2);
    Console::backgroundColor(7);
    Console::clear();

    std::string startParam = "";

    if (argc>1) {
        startParam = argv[1];
    }

    std::cout << "JuniorBASIC v1.0" << std::endl;
    std::cout << "(c) 2021 by Torsten Dix" << std::endl << std::endl;

    Console::foregroundColor(0);
    signal(SIGINT, setBreak);
    Program *code = Program::getInstance();
    Parser *parser = Parser::getInstance(nullptr);
    std::string line;
    try {
        do {
            try {
                std::cin.clear();
                if (startParam.length()) {
                    line = "RUN \""+startParam+"\"";
                    startParam = "";
                } else {
                    std::cout << '>';
                    line = Console::input(Global::getInstance()->getOutputBuffer());
                }
                if (isBreak) {
                    isBreak = false;
                    throw Break();
                }
                Tokenizer tokenizer = Tokenizer(line);
                if (tokenizer.isCodeline()) {
                    auto lineNumber = tokenizer.getLineNumber();
                    if (tokenizer.getLine().length()) {
                        code->setLine(tokenizer.getLineNumber(), tokenizer.getLine());
                    } else {
                        code->removeLine(tokenizer.getLineNumber());
                    }
                } else {
                    auto tokenLine = tokenizer.getLine();
                    auto tokenLineChar = tokenLine.data();
                    unsigned char *restOfLine = (unsigned char *) tokenLineChar;
                    while (*restOfLine != 0) {
                        while (*restOfLine == ':') {
                            restOfLine++;
                        }
                        restOfLine = parser->call(restOfLine);

                        if (Global::getInstance()->isRunMode()) {
                            do {
                                while (*code->getProgramLineCounter() != 0) {
                                    while (*code->getProgramLineCounter() == ':') {
                                        code->setProgramLineCounter(code->getProgramLineCounter() + 1);
                                    }
                                    code->setProgramLineCounter(parser->call(code->getProgramLineCounter()));
                                    if (isBreak) {
                                        isBreak = false;
                                        throw Break();
                                    }
                                }
                            } while (code->nextProgramCounter() && Global::getInstance()->isRunMode());
                            Global::getInstance()->setDirectMode();
                            restOfLine = (unsigned char *) "";
                        }
                    }

                }
            } catch (Break e) {
                std::cout << "?BREAK";
                if (Global::getInstance()->isRunMode()) {
                    std::cout << " IN LINE " << Program::getInstance()->getProgramCounter();
                }
                std::cout << std::endl;
                Global::getInstance()->setDirectMode();
            } catch (Exception e) {
                std::cout << "?" << e.getMessage() << " ERROR";
                if (Global::getInstance()->isRunMode()) {
                    std::cout << " IN LINE " << Program::getInstance()->getProgramCounter();
                }
                std::cout << std::endl;
                Global::getInstance()->setDirectMode();
            }
        } while (true);
    } catch (Quit e) {
        std::cout << std::endl;
    }

    return 0;
}
