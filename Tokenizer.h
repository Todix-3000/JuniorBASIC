//
// Created by tdix on 27.03.2021.
//

#ifndef JUNIORBASIC_TOKENIZER_H
#define JUNIORBASIC_TOKENIZER_H
#include "Token.h"

class Tokenizer {
public:
    Tokenizer(std::string rawLine);
    bool isCodeline();
    unsigned short getLineNumber();
    std::string getLine();

private:
    std::string rawLine;
    std::string parsedLine;
    unsigned short lineNumber;
    bool codeline;
};
#endif //JUNIORBASIC_TOKENIZER_H
