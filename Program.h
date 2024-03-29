//
// Created by tdix on 27.03.2021.
//

#ifndef JUNIORBASIC_PROGRAM_H
#define JUNIORBASIC_PROGRAM_H
#include <string>
#include <map>
#include <stack>
#include "Token.h"
#include "ForNextDefinition.h"

typedef struct {
    int type;
    unsigned short programCounter;
    unsigned char* programLineCounter;
    bool runMode;
    ForNextDefinition* forNextDefinition;
} StackEntry;

enum {STACK_TYPE_GOSUB, STACK_TYPE_FOR};

class Program {
private:
    Program();
    static Program *instance;
    std::map<unsigned short, std::string>  code;
    std::map<unsigned short, std::string>::iterator linePointer;
    std::map<unsigned short, std::string>::iterator programCounter;
    unsigned char* programLineCounter;
    std::map<unsigned short, std::string>::iterator dataCounter;
    unsigned char* dataLineCounter;
    std::stack<StackEntry>  stack;

public:
    void stackPush(StackEntry entry);
    StackEntry stackTop();
    void stackPop();
    bool stackEmpty();
    void stackClear();

    bool nextProgramCounter();
    unsigned char *getProgramLineCounter() const;
    unsigned short getProgramCounter();
    void setProgramLineCounter(unsigned char *programLineCounter);
    void resetProgramCounter();
    bool setProgramCounter(unsigned short lineNumber);

    bool nextDataCounter();
    void resetDataCounter();
    bool setDataCounter(unsigned short lineNumber);
    void setDataLineCounter(unsigned char *dataLineCounter);
    unsigned char *getDataLineCounter() const;

    static Program* getInstance();
    void setLine(unsigned short lineNumber, std::string lineCode);
    void removeLine(unsigned short lineNumber);
    std::string getLine(unsigned short lineNumber);
    std::string getNextLine(unsigned short &lineNumber);
    void clear();
    void resetLinePointer();
    void rewriteLineNumbers(std::map<int,int> lineNumbers);
};
#endif //JUNIORBASIC_PROGRAM_H
