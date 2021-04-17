//
// Created by tdix on 30.03.2021.
//


#ifndef JUNIORBASIC_COMMAND_H
#define JUNIORBASIC_COMMAND_H

#include <string>
#include <vector>
#include "Tokenizer.h"

class Command {
public:
    static unsigned char* _goto(unsigned char* restOfLine);
    static unsigned char* list(unsigned char* restOfLine);
    static unsigned char* print(unsigned char* restOfLine);
    static unsigned char* rem(unsigned char* restOfLine);
    static unsigned char* let(unsigned char* restOfLine);
    static unsigned char* _if(unsigned char* restOfLine);
    static unsigned char* run(unsigned char* restOfLine);
    static unsigned char* illegal(unsigned char* restOfLine);
    static unsigned char* load(unsigned char* restOfLine);
    static unsigned char* save(unsigned char* restOfLine);
    static unsigned char* _new(unsigned char* restOfLine);
    static unsigned char* end(unsigned char* restOfLine);
    static unsigned char* stop(unsigned char* restOfLine);
    static unsigned char* cont(unsigned char* restOfLine);
    static unsigned char* clr(unsigned char* restOfLine);
    static unsigned char* input(unsigned char* restOfLine);
    static unsigned char* close(unsigned char* restOfLine);
    static unsigned char* data(unsigned char* restOfLine);
    static unsigned char* dim(unsigned char* restOfLine);
    static unsigned char* _for(unsigned char* restOfLine);
    static unsigned char* next(unsigned char* restOfLine);
    static unsigned char* get(unsigned char* restOfLine);
    static unsigned char* gosub(unsigned char* restOfLine);
    static unsigned char* on(unsigned char* restOfLine);
    static unsigned char* open(unsigned char* restOfLine);
    static unsigned char* read(unsigned char* restOfLine);
    static unsigned char* _return(unsigned char* restOfLine);
    static unsigned char* restore(unsigned char* restOfLine);
    static unsigned char* dir(unsigned char* restOfLine);
    static unsigned char* kill(unsigned char* restOfLine);
    static unsigned char* chdir(unsigned char* restOfLine);
    static unsigned char* mkdir(unsigned char* restOfLine);
    static unsigned char* rmdir(unsigned char* restOfLine);
    static unsigned char* sleep(unsigned char* restOfLine);
    static unsigned char* quit(unsigned char* restOfLine);
    static unsigned char* color(unsigned char* restOfLine);
    static unsigned char* cls(unsigned char* restOfLine);
    static unsigned char* sound(unsigned char* restOfLine);
    static unsigned char* edit(unsigned char* restOfLine);
    static unsigned char* renumber(unsigned char* restOfLine);
private:
    static void __list(unsigned short start, unsigned short end, std::basic_ostream<char> *stream);
    static std::string __listLine(std::string codeLine);
    static unsigned char* __getVarIndex(unsigned char* restOfLine, VarDefinition &varDef, std::vector<int> &index);
    static unsigned char* __getFileHandle(unsigned char *restOfLine, std::fstream* &stream);
    static void __print(std::string, std::fstream*);
};
#endif //JUNIORBASIC_COMMAND_H
