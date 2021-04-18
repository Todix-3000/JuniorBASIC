//
// Created by tdix on 27.03.2021.
//

#include "Program.h"

Program::Program() {
    resetLinePointer();
    resetDataCounter();
    resetProgramCounter();
}

Program* Program::instance = nullptr;

Program* Program::getInstance() {
    if (instance == nullptr) {
        instance = new Program();
    }
    return instance;
}

void Program::setLine(unsigned short lineNumber, std::string lineCode) {
    code[lineNumber] = lineCode;
}

void Program::removeLine(unsigned short lineNumber) {
    code.erase(lineNumber);
}

std::string Program::getLine(unsigned short lineNumber) {
    try {
        return code.at(lineNumber);
    }
    catch (const std::out_of_range &e) {
        return "";
    }
}

std::string Program::getNextLine(unsigned short &lineNumber) {
    if (linePointer != code.end()) {
        lineNumber = linePointer->first;
        return linePointer++->second;
    }
    lineNumber = 0;
    return "";
}

void Program::clear() {
    code.clear();
}

void Program::resetLinePointer() {
    linePointer = code.begin();
}

unsigned short Program::getProgramCounter() {
    return programCounter->first;
}

bool Program::nextProgramCounter() {
    if (programCounter != code.end()) {
        programCounter++;
    }
    if (programCounter == code.end()) {
        programLineCounter = (unsigned char *) "";
        return false;
    }
    programLineCounter = (unsigned char*) programCounter->second.data();
    return true;
}

bool Program::nextDataCounter() {
    if (dataCounter != code.end()) {
        dataCounter++;
    }
    if (dataCounter == code.end()) {
        dataLineCounter = (unsigned char *) "";
        return false;
    }
    dataLineCounter = (unsigned char*) dataCounter->second.data();
    return true;
}

void Program::resetProgramCounter() {
    programCounter = code.begin();
    programLineCounter = (unsigned char*) programCounter->second.data();
}

bool Program::setProgramCounter(unsigned short lineNumber) {
    auto tmpProgramCounter = code.find(lineNumber);
    if (tmpProgramCounter == code.end()) {
        return false;
    }
    programCounter = tmpProgramCounter;
    programLineCounter = (unsigned char*) programCounter->second.data();

    return true;
}

void Program::setProgramLineCounter(unsigned char *programLineCounter) {
    Program::programLineCounter = programLineCounter;
}

void Program::resetDataCounter() {
    dataCounter = code.begin();
    dataLineCounter = (unsigned char*) dataCounter->second.data();
}

bool Program::setDataCounter(unsigned short lineNumber) {
    dataCounter = code.find(lineNumber);
    if (dataCounter == code.end()) {
        return false;
    }
    dataLineCounter = (unsigned char*) dataCounter->second.data();

    return true;
}

void Program::setDataLineCounter(unsigned char *dataLineCounter) {
    Program::dataLineCounter = dataLineCounter;
}

unsigned char *Program::getDataLineCounter() const {
    return dataLineCounter;
}

unsigned char *Program::getProgramLineCounter() const {
    return programLineCounter;
}

void Program::stackPush(StackEntry entry) {
    stack.push(entry);
}

void Program::stackPop() {
    if (stack.top().forNextDefinition != nullptr) {
        delete stack.top().forNextDefinition;
    }
    stack.pop();
}
StackEntry Program::stackTop() {
    return stack.top();
}

bool Program::stackEmpty() {
    return stack.empty();
}

void Program::stackClear() {
    while (!stackEmpty()) {
        stackPop();
    }
}

void Program::rewriteLineNumbers(std::map<int,int> lineNumbers) {
    std::map<unsigned short, std::string> tmpCode;
    for (auto assoc : lineNumbers) {
        tmpCode[assoc.second] = code[assoc.first];
        code.erase(assoc.first);
    }
    for (auto newCode : tmpCode) {
        code[newCode.first] = newCode.second;
    }
    resetProgramCounter();
}