//
// Created by tdix on 03.04.2021.
//
#include "Global.h"



Global* Global::instance = nullptr;

Global* Global::getInstance() {
    if (instance == nullptr) {
        instance = new Global();
        instance->setDirectMode();
    }
    return instance;
}

Global::Global() {
    outputBuffer = "";
    setDirectMode();
}

void Global::setRunMode() {
    runMode = true;
}

void Global::setDirectMode() {
    runMode = false;
}

bool Global::isRunMode() {
    return runMode;
}

bool Global::isDircetMode() {
    return !runMode;
}

void Global::setOutputBuffer(std::string buffer) {
    outputBuffer = buffer;
}

std::string Global::getOutputBuffer() {
    std::string tmp = outputBuffer;
    outputBuffer = "";
    return tmp;
}


