cmake_minimum_required(VERSION 3.17)
project(JuniorBASIC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(JuniorBASIC main.cpp Token.cpp ShuntingYard.cpp ShuntingYard.h Operator.cpp Operator.h utils.h utils.cpp Value.cpp Value.h Function.h Function.cpp Variable.h Variable.cpp Program.h Program.cpp Tokenizer.h Tokenizer.cpp Command.h Command.cpp Global.h Global.cpp ForNextDefinition.h ForNextDefinition.cpp Console.h Console.cpp)