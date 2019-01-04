//
// Created by andyson on 18-12-31.
//

#ifndef C0COMPILER_GLOBALREGISTERALLOCATION_H
#define C0COMPILER_GLOBALREGISTERALLOCATION_H

#include <map>
#include <algorithm>
#include "Type.h"
#include "Quadruple.h"
#include "SymbolTable.h"

using namespace std;

class GlobalRegisterAllocation {
public:
    map<string, allocationTable> allocationTableList;
    map<string, allocationTable> reverseAllocationTableList;
    map<string, vector<string>> chain;
    map<string, int> regSave;

    explicit GlobalRegisterAllocation(Quadruple &theQuadruple, SymbolTable &theSymbolTable);

    void allocate(bool parameter = true);

    void analyzeCallingChain();

private:
    const vector<string> initPool = {"$14", "$15", "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23", "$24",
                                     "$25"};

    Quadruple &quadruple;
    SymbolTable &symbolTable;
    vector<string> pool = initPool;
    void refreshPool();
    int toOrder(string reg);
};


#endif //C0COMPILER_GLOBALREGISTERALLOCATION_H
