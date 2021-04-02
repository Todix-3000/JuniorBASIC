//
// Created by tdix on 27.03.2021.
//

#include "Program.h"

Program::Program() {
    resetLinePointer();
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

