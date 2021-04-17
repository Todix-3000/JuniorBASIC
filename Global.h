//
// Created by tdix on 03.04.2021.
//

#ifndef JUNIORBASIC_GLOBAL_H
#define JUNIORBASIC_GLOBAL_H
#include "Program.h"
class Global {
private:
    Global();

    static Global *instance;
    bool runMode;
    std::string outputBuffer;
public:
    void setRunMode() ;
    void setDirectMode();
    bool isRunMode();
    bool isDircetMode();
    static Global *getInstance();
    void setOutputBuffer(std::string buffer);
    std::string getOutputBuffer();
};
#endif //JUNIORBASIC_GLOBAL_H
