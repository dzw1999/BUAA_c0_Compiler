cmake_minimum_required(VERSION 3.12)
project(C0Compiler)

set(CMAKE_CXX_STANDARD 11)

add_executable(C0Compiler main.cpp ExceptionHandler.cpp ExceptionHandler.h GrammarAnalyzer.cpp GrammarAnalyzer.h LexicalAnalyzer.h LexicalAnalyzer.cpp SymbolTable.cpp SymbolTable.h Quadruple.cpp Quadruple.h SemanticAnalyzer.cpp SemanticAnalyzer.h Type.h MIPSGenerator.cpp MIPSGenerator.h StackManager.cpp StackManager.h)