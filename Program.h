//
// Created by tdix on 27.03.2021.
//

#ifndef JUNIORBASIC_PROGRAM_H
#define JUNIORBASIC_PROGRAM_H
#include <string>
#include <map>

class Program {
private:
    Program();
    static Program *instance;
    std::map<unsigned short, std::string>  code;
    std::map<unsigned short, std::string>::iterator linePointer;
    std::map<unsigned short, std::string>::iterator programCounter;
    unsigned char* programLineCounter;

public:
    bool nextProgramCounter();
    unsigned char *getProgramLineCounter() const;
    unsigned short getProgramCounter();
    void setProgramLineCounter(unsigned char *programLineCounter);
    void resetProgramCounter();
    bool setProgramCounter(unsigned short lineNumber);
    static Program* getInstance();
    void setLine(unsigned short lineNumber, std::string lineCode);
    void removeLine(unsigned short lineNumber);
    std::string getLine(unsigned short lineNumber);
    std::string getNextLine(unsigned short &lineNumber);
    void clear();
    void resetLinePointer();
};
#endif //JUNIORBASIC_PROGRAM_H
