//
// Created by tdix on 27.03.2021.
//

#include <iostream>
#include "Tokenizer.h"
#include "utils.h"

Tokenizer::Tokenizer(std::string rawLineInput) {
    codeline = false;
    bool quoteMode = false;
    parsedLine = "";
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

    std::string upperRawLine = rawLine;
    for (auto & c: upperRawLine) { c = toupper(c); }
    while (rawLine.length()>0) {
        while (rawLine[0] == ' ') {
            rawLine = &rawLine[1];
            upperRawLine = &upperRawLine[1];
        }
        size_t tokenLength;
        unsigned char tokenId = Parser::getInstance((unsigned char *) upperRawLine.data())->getTokenId(tokenLength);
        if (tokenLength > 0) {
            parsedLine += tokenId;
            rawLine.replace(0, tokenLength, "");
            upperRawLine.replace(0, tokenLength, "");
            if (tokenId==CMD_REM) {
                if (rawLine[0] == ' ') { rawLine = &rawLine[1]; }
                parsedLine += rawLine;
                rawLine = "";
            }
        } else {
            if (rawLine[0]=='"') {
                quoteMode = !quoteMode;
            }
            parsedLine += quoteMode ? rawLine[0] : upperRawLine[0];
            upperRawLine = &upperRawLine[1];
            rawLine = &rawLine[1];
        }
    }
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