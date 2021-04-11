//
// Created by tdix on 30.03.2021.
//

#include "Command.h"
#include "ShuntingYard.h"
#include "Program.h"
#include "Token.h"
#include "utils.h"
#include "Variable.h"
#include "Global.h"
#include "Tokenizer.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <sstream>
#include <dirent.h>
#include <direct.h>
#include <regex>
#include <unistd.h>
#include <sys/stat.h>
#include <iomanip>

unsigned char* Command::_goto(unsigned char* restOfLine) {
    int param;
    try {
        size_t len;
        std::string line((char *) restOfLine);
        param = std::stoi(line, &len);
        if (param<0 || param>USHRT_MAX) {
            throw Exception(EXCEPTION_RANGE_ERROR);
        }
        restOfLine += len;
        if (!Program::getInstance()->setProgramCounter(param)) {
            throw Exception(EXCEPTION_UNKNOWN_LINE);
        }
    } catch (std::invalid_argument e) {
        throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
    }
    Global::getInstance()->setRunMode();
    return Program::getInstance()->getProgramLineCounter();
}

void Command::__list(unsigned short start, unsigned short end, std::basic_ostream<char> *stream) {
    Program *code = Program::getInstance();
    code->resetLinePointer();
    unsigned short lineNumber;
    std::string codeLine;
    do {
        codeLine = code->getNextLine(lineNumber);
        if (lineNumber!=0 && lineNumber>=start && lineNumber<=end) {
            std::string outputLine = "";
            bool insertSpace = false;
            short type;
            bool firstChar = true;
            for (auto c : codeLine) {
                std::string tokenName = Parser::getInstance(nullptr)->findTokenName((unsigned char) c, type);
                if (insertSpace) { outputLine += ' '; insertSpace = false; firstChar = true;}
                if (tokenName.length()>0) {
                    if (type == TOKEN_TYPE_COMMAND) {
                        insertSpace = true;
                        if (!firstChar) {
                            outputLine += ' ';
                        }
                    }
                    outputLine += tokenName;
                } else {
                    outputLine += c;
                }
                firstChar = false;
            }
            *stream << lineNumber << ' ' << outputLine << std::endl;
        }
    } while (lineNumber!=0);
}
unsigned char* Command::list(unsigned char* restOfLine) {
    unsigned short start = 0;
    unsigned short end = USHRT_MAX;
    short paramCount = 0;
    while (*restOfLine != 0 && *restOfLine != ':') {
        bool ok = false;
        if (*restOfLine == '-') {
            paramCount++;
            if (paramCount>1) {
                throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
            }
            ok = true;
            restOfLine++;
        }
        int param=0;
        try {
            size_t len;
            std::string line((char *) restOfLine);
            param = std::stoi(line, &len);
            if (param<0 || param>USHRT_MAX) {
                throw Exception(EXCEPTION_RANGE_ERROR);
            }
            restOfLine += len;
            if (paramCount==0) {
                start = param;
            } else {
                end = param;
            }
            ok = true;
        } catch (std::invalid_argument e) {
        }
        if (!ok) {
            throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
        }

    }
    __list(start, end, &std::cout);

    return restOfLine;
}

void Command::__print(std::string s, std::basic_fstream<char>* stream) {
    if (stream== nullptr) {
        std::cout << s;
    } else {
        stream->write(s.data(), s.length());
        if (stream->bad()) {
            throw Exception(EXCEPTION_FILE_WRITE);
        }
    }
}

unsigned char* Command::print(unsigned char* restOfLine) {
    std::basic_fstream<char>* stream;
    restOfLine = __getFileHandle(restOfLine, stream);

    ShuntingYard *algorithm = new ShuntingYard();
    Value result;
    bool lineFeed = true;
    while (*restOfLine != 0 && *restOfLine != ':') {
        if (*restOfLine == ';') {
            restOfLine++;
            lineFeed = false;
        } else if (*restOfLine == ',') {
            restOfLine++;
            lineFeed = false;
            __print(" ", stream);
        } else {
            restOfLine = algorithm->run(restOfLine, result);
            std::ostringstream ss;
            switch (result.getType()) {
                case VALUE_TYPE_INT:
                    ss << result.getInt();
                    __print(ss.str(), stream);
                    break;
                case VALUE_TYPE_FLOAT:
                    ss << result.getFloat();
                    __print(ss.str(), stream);
                    break;
                case VALUE_TYPE_STRING:
                    __print(result.getString(), stream);
                    break;
            }
            lineFeed = true;
        }
    }
    if (lineFeed) {
        if (stream!= nullptr) {
            __print("\n", stream );
        } else {
            std::cout << std::endl;
        }
    }
    return restOfLine;
}

unsigned char* Command::rem(unsigned char* restOfLine) {
    while (*restOfLine != 0) { restOfLine++; }
    return restOfLine;
}

unsigned char* Command::let(unsigned char* restOfLine) {
    VarDefinition varDef;
    std::vector<int> index;
    restOfLine = __getVarIndex(restOfLine, varDef, index);

    if (*restOfLine!='=') {
        throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
    }
    restOfLine++;
    Value result;
    restOfLine = (new ShuntingYard())->run(restOfLine, result);
    if (result.getType()!= varDef.varType) {
        if (varDef.varType==VALUE_TYPE_FLOAT) {
            result = Value(result.getFloat());
        } else if (varDef.varType==VALUE_TYPE_INT) {
            result = Value(result.getInt());
        } else {
            result = Value(result.getString());
        }

    }
    if (index.size()==0) {
        Variable::getContainer()->setValue(varDef.varName, result);
    } else {
        Variable::getContainer()->setValue(varDef.varName, index, result);
    }

    return restOfLine;
}

unsigned char *Command::_if(unsigned char *restOfLine) {
    ShuntingYard *algorithm = new ShuntingYard();
    Value result;
    restOfLine = algorithm->run(restOfLine, result);
    bool isTrue = false;
    switch (result.getType()) {
        case VALUE_TYPE_INT:
            isTrue = result.getInt() != 0;
            break;
        case VALUE_TYPE_FLOAT:
            isTrue = result.getFloat() != 0.0;
            break;
        case VALUE_TYPE_STRING:
            isTrue = result.getString().length() > 0;
            break;
    }
    if (isTrue) {
        if (*restOfLine == CMD_GOTO) {
            restOfLine++;
            return _goto(restOfLine);
        } else if (*restOfLine == CMD_THEN) {
            restOfLine++;
            return restOfLine;
        }
    } else {
        while (*restOfLine != 0) { restOfLine++; }
    }
    return restOfLine;
}

unsigned char *Command::run(unsigned char *restOfLine) {
    Variable::getContainer()->clearAll();
    Global::getInstance()->setRunMode();
    Program::getInstance()->resetProgramCounter();
    Program::getInstance()->resetDataCounter();
    Program::getInstance()->stackClear();
    return restOfLine;
}

unsigned char *Command::illegal(unsigned char *restOfLine) {
    throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
}

unsigned char *Command::_new(unsigned char *restOfLine) {
    Program::getInstance()->clear();
    restOfLine = (unsigned char*) "";
    return restOfLine;
}

unsigned char *Command::end(unsigned char *restOfLine) {
    Global::getInstance()->setDirectMode();
    return restOfLine;
}

unsigned char *Command::stop(unsigned char *restOfLine) {
    if (Global::getInstance()->isRunMode()) {
        Program::getInstance()->setProgramLineCounter(restOfLine);
    }
    throw Break();
}

unsigned char *Command::cont(unsigned char *restOfLine) {
    Global::getInstance()->setRunMode();
    return restOfLine;
}

unsigned char *Command::clr(unsigned char *restOfLine) {
    Variable::getContainer()->clearAll();
    return restOfLine;
}

unsigned char *Command::input(unsigned char *restOfLine) {
    std::basic_fstream<char>* stream;
    restOfLine = __getFileHandle(restOfLine, stream);

    if (stream== nullptr) {
        if (*restOfLine == '"') {
            ShuntingYard *algorithm = new ShuntingYard();
            Value result;
            restOfLine = ShuntingYard().run(restOfLine, result);
            std::cout << result.getString();
            if (*restOfLine == ';') {
                restOfLine++;
            } else {
                throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
            }
        }
    }
    bool lastVar = false;
    do {
        VarDefinition varDef;
        std::vector<int> index;
        restOfLine = __getVarIndex(restOfLine, varDef, index);
        std::string line;
        if (stream== nullptr) {
            std::cout << '?';
            std::getline(std::cin, line);
        } else {
            std::getline(*stream, line);
            if (stream->bad()) {
                throw Exception(EXCEPTION_FILE_READ);
            }
        }
        Value result;

        try {
            switch (varDef.varType) {
                case VALUE_TYPE_STRING:
                    result = Value(line);
                    break;
                case VALUE_TYPE_INT:
                    result = Value(std::stoi(line));
                    break;
                case VALUE_TYPE_FLOAT:
                    result = Value(std::stod(line));
                    break;
            }
        } catch (std::invalid_argument e) {
            throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
        }
        Variable::getContainer()->setValue(varDef.varName, index, result);

        if (*restOfLine==',') {
            restOfLine++;
        } else {
            lastVar = true;
        }
    } while (!lastVar);
    return restOfLine;
}

unsigned char* Command::__getVarIndex(unsigned char* restOfLine, VarDefinition &varDef, std::vector<int> &index) {
    Parser *parser = Parser::getInstance(restOfLine);
    varDef = parser->getVariableDefinition();
    restOfLine = parser->inputPtr;

    index.clear();
    if (*restOfLine == '(') {
        restOfLine++;
        bool finished = false;
        do {
            Value result;
            restOfLine = (new ShuntingYard())->run(restOfLine, result);
            index.push_back(result.getInt());
            if (*restOfLine == ',') {
                restOfLine++;
            } else if (*restOfLine == ')') {
                restOfLine++;
                finished = true;
            } else {
                throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
            }
        } while (!finished);
    }
    return restOfLine;
}

unsigned char *Command::data(unsigned char *restOfLine) {
    bool searchAgain = true;
    do {
        try {
            Parser *parser = Parser::getInstance(restOfLine);
            parser->getLiteralValue();
            restOfLine = parser->inputPtr;
            if (*restOfLine == ',') {
                restOfLine++;
            } else {
                searchAgain = false;
            }
        } catch (NotFount e) {
            searchAgain = false;
        }
    } while (searchAgain);

    return restOfLine;
}

unsigned char *Command::dim(unsigned char *restOfLine) {
    bool lastVar = false;
    do {
        VarDefinition varDef;
        std::vector<int> index;
        restOfLine = __getVarIndex(restOfLine, varDef, index);
        Variable::getContainer()->dim(varDef.varName, varDef.varType, index);
        if (*restOfLine==',') {
            restOfLine++;
        } else {
            lastVar = true;
        }
    } while (!lastVar);
    return restOfLine;
}

unsigned char *Command::__getFileHandle(unsigned char *restOfLine, std::basic_fstream<char>* &stream) {
    if (*restOfLine=='#') {
        restOfLine++;
        Parser *parser = Parser::getInstance(restOfLine);
        Value fileId = parser->getLiteralValue();
        restOfLine = parser->inputPtr;
        if (*restOfLine==',') {
            restOfLine++;
        } else {
            throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
        }
        stream = Variable::getContainer()->fileGet(fileId.getInt());
    } else {
        stream = nullptr;
    }
    return restOfLine;
}

unsigned char *Command::get(unsigned char *restOfLine) {
    std::basic_fstream<char>* stream;
    restOfLine = __getFileHandle(restOfLine, stream);

    VarDefinition varDef;
    std::vector<int> index;
    restOfLine = __getVarIndex(restOfLine, varDef, index);
    char c;
    std::string line = "";
    if (stream!= nullptr && !stream->eof()) {
        c = stream->get();
        line += c;
    } else {
        if (kbhit()) {
            c = getch();
            line += c;
        }
    }
    Value result;

    try {
        switch (varDef.varType) {
            case VALUE_TYPE_STRING:
                result = Value(line);
                break;
            case VALUE_TYPE_INT:
                result = Value(std::stoi(line));
                break;
            case VALUE_TYPE_FLOAT:
                result = Value(std::stod(line));
                break;
        }
    } catch (std::invalid_argument e) {
        throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
    }
    if (index.size() == 0) {
        Variable::getContainer()->setValue(varDef.varName, result);
    } else {
        Variable::getContainer()->setValue(varDef.varName, index, result);
    }

    return restOfLine;
}

unsigned char *Command::gosub(unsigned char *restOfLine) {
    int param;
    try {
        size_t len;
        std::string line((char *) restOfLine);
        param = std::stoi(line, &len);
        if (param<0 || param>USHRT_MAX) {
            throw Exception(EXCEPTION_RANGE_ERROR);
        }
        restOfLine += len;
        StackEntry stackEntry;
        stackEntry.type = CMD_GOSUB;
        stackEntry.runMode = Global::getInstance()->isRunMode();
        stackEntry.programLineCounter = restOfLine;
        stackEntry.programCounter = stackEntry.runMode ? Program::getInstance()->getProgramCounter() : 0;
        stackEntry.forNextDefinition = nullptr;
        Program::getInstance()->stackPush(stackEntry);
        if (!Program::getInstance()->setProgramCounter(param)) {
            throw Exception(EXCEPTION_UNKNOWN_LINE);
        }
    } catch (std::invalid_argument e) {
        throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
    }
    Global::getInstance()->setRunMode();
    return Program::getInstance()->getProgramLineCounter();
}

unsigned char *Command::on(unsigned char *restOfLine) {
    Value result;
    unsigned char command;
    std::vector<int> params;

    restOfLine = ShuntingYard().run(restOfLine, result);
    if (*restOfLine==CMD_GOSUB || *restOfLine==CMD_GOTO) {
        command = *restOfLine;
        restOfLine++;
    } else {
        throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
    }
    int param;
    bool searchAgain = true;
    try {
        do {
            size_t len;
            std::string line((char *) restOfLine);
            param = std::stoi(line, &len);
            if (param < 0 || param > USHRT_MAX) {
                throw Exception(EXCEPTION_RANGE_ERROR);
            }
            restOfLine += len;
            params.push_back(param);
            if (*restOfLine == ',') {
                restOfLine++;
            } else {
                searchAgain = false;
            }
        } while (searchAgain);
    } catch (std::invalid_argument e) {
        throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
    }

    int onValue = result.getInt()-1;
    if (onValue>=0 && onValue<params.size()) {
        param = params[onValue];
        switch (command) {
            case CMD_GOTO:
                if (!Program::getInstance()->setProgramCounter(param)) {
                    throw Exception(EXCEPTION_UNKNOWN_LINE);
                }
                break;
            case CMD_GOSUB:
                StackEntry stackEntry;
                stackEntry.type = CMD_GOSUB;
                stackEntry.runMode = Global::getInstance()->isRunMode();
                stackEntry.programLineCounter = restOfLine;
                stackEntry.programCounter = stackEntry.runMode ? Program::getInstance()->getProgramCounter() : 0;
                stackEntry.forNextDefinition = nullptr;
                Program::getInstance()->stackPush(stackEntry);
                if (!Program::getInstance()->setProgramCounter(param)) {
                    throw Exception(EXCEPTION_UNKNOWN_LINE);
                }
                break;
        }
        Global::getInstance()->setRunMode();

        return Program::getInstance()->getProgramLineCounter();
    }
    return restOfLine;
}


unsigned char *Command::close(unsigned char *restOfLine) {
    Value fileId;
    restOfLine = ShuntingYard().run(restOfLine, fileId);
    if (fileId.getInt() <= 0 || fileId.getInt() > UCHAR_MAX) {
        throw Exception(EXCEPTION_RANGE_ERROR);
    }
    Variable::getContainer()->fileClose(fileId.getInt());
    return restOfLine;
}

unsigned char *Command::open(unsigned char *restOfLine) {
    Value fileId;
    restOfLine = ShuntingYard().run(restOfLine, fileId);
    if (fileId.getInt() <= 0 || fileId.getInt() > UCHAR_MAX) {
        throw Exception(EXCEPTION_RANGE_ERROR);
    }
    if (*restOfLine==',') {
        restOfLine++;
    }
    Value fileName;
    restOfLine = ShuntingYard().run(restOfLine, fileName);
    std::ios_base::openmode fileMode = std::fstream::in;
    Value fileOptions = Value("r");
    if (*restOfLine==',') {
        restOfLine++;
        restOfLine = ShuntingYard().run(restOfLine, fileOptions);
        if (fileOptions.getString().length()<1) {
            throw Exception(EXCEPTION_RANGE_ERROR);
        }
        switch (toupper(fileOptions.getString()[0])) {
            case 'R':
                break;
            case 'W':
                fileMode = std::fstream::out;
                break;
            case 'A':
                fileMode = std::fstream::out | std::fstream::app;
                break;
            default:
                throw Exception(EXCEPTION_RANGE_ERROR);
        }
    }
    if (Variable::getContainer()->fileIsOpen(fileId.getInt())) {
        throw Exception(EXCEPTION_FILE_OPEN);
    }
    std::basic_fstream<char> *fileStream = new std::basic_fstream<char>;
    //std::fstream *fileStream = new std::fstream;
    fileStream->open(fileName.getString(), fileMode);
    if (!fileStream->is_open()) {
        if (fileMode | std::fstream::in) {
            throw Exception(EXCEPTION_FILE_READ);
        }
        throw Exception(EXCEPTION_FILE_WRITE);
    }
    Variable::getContainer()->fileOpen(fileId.getInt(), fileStream);

    return restOfLine;
}

unsigned char *Command::read(unsigned char *restOfLine) {
    bool lastVar = false;
    do {
        VarDefinition varDef;
        std::vector<int> index;
        restOfLine = __getVarIndex(restOfLine, varDef, index);
        auto code = Program::getInstance();
        auto dataLinePointer = code->getDataLineCounter();
        if (*dataLinePointer != ',') {
            do {
                if (*dataLinePointer == CMD_DATA) {
                    break;
                } else if (*dataLinePointer == CMD_REM) {
                    if (!code->nextDataCounter()) {
                        throw Exception(EXCEPTION_OUT_OF_DATA);
                    }
                    dataLinePointer = code->getDataLineCounter();
                } else if (*dataLinePointer == '"') {
                    auto parser = Parser::getInstance(dataLinePointer);
                    parser->getLiteralValue();
                    dataLinePointer = parser->inputPtr;
                } else if (*dataLinePointer == 0) {
                    if (!code->nextDataCounter()) {
                        throw Exception(EXCEPTION_OUT_OF_DATA);
                    }
                    dataLinePointer = code->getDataLineCounter();
                } else {
                    dataLinePointer++;
                }

            } while (true);
        }
        dataLinePointer++;
        Value result;
        dataLinePointer = ShuntingYard().run(dataLinePointer, result);
        code->setDataLineCounter(dataLinePointer);
        if (varDef.varType != result.getType()) {
            switch (varDef.varType) {
                case VALUE_TYPE_INT:
                    result = Value(result.getInt());
                    break;
                case VALUE_TYPE_FLOAT:
                    result = Value(result.getFloat());
                    break;
                case VALUE_TYPE_STRING:
                    result = Value(result.getString());
                    break;
            }
        }
        Variable::getContainer()->setValue(varDef.varName, index, result);

        if (*restOfLine==',') {
            restOfLine++;
        } else {
            lastVar = true;
        }
    } while (!lastVar);


    return restOfLine;
}

unsigned char *Command::_return(unsigned char *restOfLine) {
    Program *code = Program::getInstance();
    while (!code->stackEmpty() && code->stackTop().type!=CMD_GOSUB) {
        code->stackPop();
    }
    if (code->stackEmpty() || code->stackTop().type!=CMD_GOSUB) {
        throw Exception(EXCEPTION_RETURN_WITHOUT_GOSUB);
    }
    StackEntry stackEntry = code->stackTop();
    code->stackPop();
    if (Global::getInstance()->isRunMode()) {
        code->setProgramCounter(stackEntry.programCounter);
    }
    if (!stackEntry.runMode) {
        Global::getInstance()->setDirectMode();
    }
    return stackEntry.programLineCounter;
}

unsigned char *Command::restore(unsigned char *restOfLine) {
    Program::getInstance()->resetDataCounter();
    return restOfLine;
}

unsigned char *Command::next(unsigned char *restOfLine) {
    Program *code = Program::getInstance();
    bool endOfParams;
    do {
        endOfParams = true;
        if (code->stackEmpty() || code->stackTop().type != CMD_FOR) {
            throw Exception(EXCEPTION_NEXT_WITHOUT_FOR);
        }
        VarDefinition varDef;
        std::vector<int> index;
        bool hasIndex = true;
        try {
            restOfLine = __getVarIndex(restOfLine, varDef, index);
        } catch (Exception) {
            hasIndex = false;
        }
        bool continueLoop;
        if (hasIndex) {
            continueLoop = code->stackTop().forNextDefinition->checkNextStep(varDef, index);
        } else {
            continueLoop = code->stackTop().forNextDefinition->checkNextStep();
        }

        if (continueLoop) {
            restOfLine = code->stackTop().programLineCounter;
            if (Global::getInstance()->isRunMode()) {
                code->setProgramCounter(code->stackTop().programCounter);
            }
            endOfParams = true;
        } else {
            code->stackPop();
        }
        if (*restOfLine==',') {
            restOfLine++;
            endOfParams = false;
        }
    } while (!endOfParams);
    return restOfLine;
}

unsigned char *Command::load(unsigned char *restOfLine) {
    Value result;

    restOfLine = ShuntingYard().run(restOfLine, result);
    std::ifstream in(result.getString(), std::ios::in);
    if (!in.is_open()) {
        throw Exception(EXCEPTION_FILE_READ);
    }
    Program *code = Program::getInstance();

    code->clear();
    Variable::getContainer()->clearAll();

    std::string line;
    while (std::getline(in, line)) {
        Tokenizer tokenizer = Tokenizer(line);
        if (tokenizer.isCodeline()) {
            auto lineNumber = tokenizer.getLineNumber();
            if (tokenizer.getLine().length()) {
                code->setLine(tokenizer.getLineNumber(), tokenizer.getLine());
            }
        }
    }
    code->resetProgramCounter();
    restOfLine = (unsigned char*) "";
    return restOfLine;
}

unsigned char *Command::save(unsigned char *restOfLine) {
    Value result;
    restOfLine = ShuntingYard().run(restOfLine, result);
    std::ofstream out(result.getString(), std::ios::trunc | std::ios::out);
    if (!out.is_open()) {
        throw Exception(EXCEPTION_FILE_WRITE);
    }
    __list(0, USHRT_MAX, &out);
    return restOfLine;
}

unsigned char *Command::_for(unsigned char *restOfLine) {
    VarDefinition varDef;
    std::vector<int> index;
    restOfLine = __getVarIndex(restOfLine, varDef, index);

    if (*restOfLine!='=') {
        throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
    }
    restOfLine++;
    Value counter;
    restOfLine = (new ShuntingYard())->run(restOfLine, counter);
    if (counter.getType() != varDef.varType) {
        if (varDef.varType==VALUE_TYPE_FLOAT) {
            counter = Value(counter.getFloat());
        } else if (varDef.varType==VALUE_TYPE_INT) {
            counter = Value(counter.getInt());
        } else {
            throw Exception(EXCEPTION_TYPE_MISMATCH);
        }
    }
    if (*restOfLine != CMD_TO) {
        throw Exception(EXCEPTION_ILLEGAL_EXPRESSION);
    }
    restOfLine++;
    Value target;
    restOfLine = (new ShuntingYard())->run(restOfLine, target);
    if (target.getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    Variable::getContainer()->setValue(varDef.varName, index, counter);

    Value step = Value(1);
    char direction = 1;
    if (*restOfLine == CMD_STEP) {
        restOfLine++;
        restOfLine = (new ShuntingYard())->run(restOfLine, step);
        if (step.getType() == VALUE_TYPE_STRING) {
            throw Exception(EXCEPTION_TYPE_MISMATCH);
        }
        if (step.getFloat()<0) {
            direction = -1;
        }
    }

    StackEntry entry;
    entry.forNextDefinition = new ForNextDefinition(varDef, index, direction, target, step);
    entry.type = CMD_FOR;
    entry.runMode = Global::getInstance()->isRunMode();
    entry.programLineCounter = restOfLine;
    entry.programCounter = entry.runMode ? Program::getInstance()->getProgramCounter() : 0;

    Program::getInstance()->stackPush(entry);
    return restOfLine;
}


unsigned char *Command::dir(unsigned char *restOfLine) {
    Value wildcard = Value("*");
    if (*restOfLine!=':' && *restOfLine!=0) {
        restOfLine = ShuntingYard().run(restOfLine, wildcard);
        if (wildcard.getString() == "") {
            wildcard = Value("*");
        }
    }
    std::string regexp = "^";
    std::string wildCardSource = wildcard.getString();
    for (int i=0; i<wildCardSource.length(); i++) {
        char ch = toupper(wildCardSource[i]);
        if ((ch>='0' && ch<='9') || (ch>='A' && ch<='Z')) {
            regexp.push_back(ch);
        } else if (ch=='*') {
            regexp += ".*";
        } else {
            regexp.push_back('\\');
            regexp.push_back(ch);
        }
    }
    regexp += "$";
    DIR *dir;
    dirent *dirEntity;
    std::vector<std::string> fileList;
    if ((dir= opendir("."))!= nullptr) {
        while ((dirEntity = readdir(dir)) != nullptr) {
            auto const regex = std::regex(regexp, std::regex::icase);
            auto const myText = std::string(dirEntity->d_name);
            auto matchResults = std::smatch{};
            if (std::string(dirEntity->d_name) != std::string(".") && std::regex_search(myText, matchResults, regex)) {
                fileList.push_back(std::string(dirEntity->d_name));
            }
        }
        for (std::string filename : fileList) {
            struct stat stat_buf;
            int rc = stat(filename.c_str(), &stat_buf);
            if(S_ISDIR(stat_buf.st_mode)) {
                std::cout << "<DIR>      ";
                std::cout << filename << std::endl;
            }
        }
        for (std::string filename : fileList) {
            struct stat stat_buf;
            int rc = stat(filename.c_str(), &stat_buf);
            int size = rc == 0 ? stat_buf.st_size : -1;
            if(!S_ISDIR(stat_buf.st_mode)) {
                std::cout << std::setw(10) << std::setfill(' ') << size << " ";
                std::cout << filename << std::endl;
            }
        }
    }
    return restOfLine;
}

unsigned char *Command::kill(unsigned char *restOfLine) {
    Value pathName;
    restOfLine = ShuntingYard().run(restOfLine, pathName);
    if (remove(pathName.getString().data()) != 0) {
        throw Exception(EXCEPTION_FILESYSTEM);
    }

    return restOfLine;
}

unsigned char *Command::chdir(unsigned char *restOfLine) {
    Value pathName;
    restOfLine = ShuntingYard().run(restOfLine, pathName);

    if (_chdir(pathName.getString().data()) != 0) {
        throw Exception(EXCEPTION_FILESYSTEM);
    }
    return restOfLine;
}

unsigned char *Command::mkdir(unsigned char *restOfLine) {
    Value pathName;
    restOfLine = ShuntingYard().run(restOfLine, pathName);

    if (_mkdir(pathName.getString().data()) != 0) {
        throw Exception(EXCEPTION_FILESYSTEM);
    }
    return restOfLine;
}
unsigned char *Command::rmdir(unsigned char *restOfLine) {
    Value pathName;
    restOfLine = ShuntingYard().run(restOfLine, pathName);
    if (_rmdir(pathName.getString().data()) != 0) {
        throw Exception(EXCEPTION_FILESYSTEM);
    }
    return restOfLine;
}
