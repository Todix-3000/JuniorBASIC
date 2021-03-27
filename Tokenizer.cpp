//
// Created by tdix on 27.03.2021.
//

#include <iostream>
#include "Tokenizer.h"
#include "utils.h"

Tokenizer::Tokenizer(std::string rawLineInput) {
    codeline = false;
    rawLine = rawLineInput;
    size_t idx;
    try {
        int line = std::stoi(rawLine, &idx);
        if (idx > 0) {
            if (line < 0 || line > USHRT_MAX) {
                throw std::out_of_range("");
            }
            codeline = true;
            lineNumber = line;
            rawLine = &rawLine[idx];
        }
    } catch (const std::out_of_range& oor) {
        throw Exception(EXCEPTION_RANGE_ERROR);
    } catch (...) {};
    while (rawLine[0] == ' ') {
        rawLine = &rawLine[1];
    }
    std::cout << rawLine;
}
bool Tokenizer::isCodeline() {
    return codeline;
}

unsigned short Tokenizer::getLineNumber() {
    if (codeline) {
        return lineNumber;
    }
    return 0;
}

std::string Tokenizer::getLine() {
    return parsedLine;
}