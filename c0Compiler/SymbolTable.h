//
// Created by andyson on 18-11-28.
//

#ifndef C0COMPILER_SYMBOLTABLE_H
#define C0COMPILER_SYMBOLTABLE_H

#include <cstdio>
#include <map>
#include <string>
#include "Type.h"

using namespace std;

class SymbolTable {
public:
    map<string, int> localSymTableOffsetList;
    map<string, symTable> localSymTableList;
    symTable globalSymTable;

    static SymbolTable &getSymbolTable();  // 单例设计模式
    symTableEntry createSte(objectType oType,valueType vType, int length = 0, int parameter = 0, int constValue = 0);

    bool addToTable(string ident, symTableEntry Ste, string funcName = "");
    symTableEntry searchInTable(string ident, string funcName);
    symTableEntry searchInTable(int parameterNum, string funcName);
    symTableEntry searchInGlobalTable(string ident);
    symTableEntry searchInLocalTable(string ident, string funcName);
    bool createLocalSymTable(string funcName);

private:
    int globalSymTableOffset;

    SymbolTable();
};


#endif //C0COMPILER_SYMBOLTABLE_H
