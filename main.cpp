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


int main() {
    std::cout << "JuniorBASIC v1.0" << std::endl;
    std::cout << "(c) 2021 by Torsten Dix" << std::endl << std::endl;

    Program *code = Program::getInstance();
    ShuntingYard *algorithm = new ShuntingYard();
    Parser *parser = Parser::getInstance(nullptr);
    std::string line;

    do {
        std::cout << '>';
        std::getline(std::cin, line);
        Tokenizer tokenizer = Tokenizer(line);
        if (tokenizer.isCodeline()) {
            auto lineNumber = tokenizer.getLineNumber();
            if (tokenizer.getLine().length()) {
                code->setLine(tokenizer.getLineNumber(), tokenizer.getLine());
            } else {
                code->removeLine(tokenizer.getLineNumber());
            }
        } else {
            try {
                auto restOfLine = (unsigned char *) tokenizer.getLine().data();
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
                            }
                        } while (code->nextProgramCounter() && Global::getInstance()->isRunMode());
                        Global::getInstance()->setDirectMode();
                        restOfLine = (unsigned char *) "";
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
        }
    } while (line != "QUIT");
    std::cout << "Bye\n";
/*
    std::cout <<test.size();
    Program *code = Program::getInstance();
    code->setLine(10, "A%=10");
    code->setLine(20, "PRINT A%");
    code->setLine(30, "A% = A% +1");
    code->setLine(40, "IF A%<10 GOTO 20");

    code->resetLinePointer();
    unsigned short lineNumber;
    std::string line;
    do {
        line = code->getNextLine(lineNumber);
        if (line != "") {
            std::cout << lineNumber << ' ' << line << std::endl;
        }
    } while (line != "");


    ShuntingYard *algorithm = new ShuntingYard();
    try {
        Tokenizer tokenizer = Tokenizer("10000 print a%-chr$(9)+ 50");
        tokenizer = Tokenizer("10000 rem Print a%-chr$(9)+ 50");
        tokenizer = Tokenizer("10000 MID$(TEST$, 3,7)\\0\")");
        std::cout << std::boolalpha << tokenizer.isCodeline();
        std::cout << tokenizer.getLineNumber();

        auto v = Variable::getContainer();
        v->setValue("TEST%", Value(42));
        v->setValue("TEST%", Value(42.5));
        v->setValue("TEST$", Value("HALLO I BIMS"));

        v->dim("TEST%", VALUE_TYPE_INT, {5,5});
        v->setValue("TEST%", {2,3}, Value(23));

        tokenizer = Tokenizer("10 1+(2*3)-8+Test%(2,5-2)");
        std::cout << algorithm->run((unsigned char*) tokenizer.getLine().data()) << std::endl;

        tokenizer = Tokenizer("10 1+sin(1+2.14)");
        std::cout << algorithm->run((unsigned char *) tokenizer.getLine().data()) << std::endl;

        tokenizer = Tokenizer("mid$(test$, 3,7)\0");
        std::cout << algorithm->run((unsigned char *) tokenizer.getLine().data()) << std::endl;

        //std::cout << algorithm->run((unsigned char *) "\"Hallo\" = \" \" + \"Hallo\"\0") << std::endl;
       // std::cout << algorithm->run((unsigned char*) "-(3*-26./5+6*-1)\0") << std::endl;
       // std::cout << algorithm->run((unsigned char*) "25*6-100\0") << std::endl;
       // std::cout << algorithm->run((unsigned char *) "\"Hallo\" + \" \" + \"Welt\"\0") << std::endl;
       // std::cout << algorithm->run((unsigned char*) "\"\" && \"45\"\0") << std::endl;
    } catch (Exception exception) {
        std::cout << "EXCEPTION: " << exception.getCode();
    }
    */
    return 0;
}
