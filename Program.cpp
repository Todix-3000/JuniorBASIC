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

void Program::resetProgramCounter() {
    programCounter = code.begin();
    programLineCounter = (unsigned char*) programCounter->second.data();
}

bool Program::setProgramCounter(unsigned short lineNumber) {
    programCounter = code.find(lineNumber);
    if (programCounter == code.end()) {
        return false;
    }
    programLineCounter = (unsigned char*) programCounter->second.data();

    return true;
}

void Program::setProgramLineCounter(unsigned char *programLineCounter) {
    Program::programLineCounter = programLineCounter;
}

unsigned char *Program::getProgramLineCounter() const {
    return programLineCounter;
}

