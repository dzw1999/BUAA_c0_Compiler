//
// Created by andyson on 18-12-31.
//

#ifndef C0COMPILER_GLOBALREGISTERALLOCATION_H
#define C0COMPILER_GLOBALREGISTERALLOCATION_H

#include <map>
#include "Type.h"
#include "Quadruple.h"
#include "SymbolTable.h"

using namespace std;

class GlobalRegisterAllocation {
public:
    map<string, allocationTable> allocationTableList;

    explicit GlobalRegisterAllocation(Quadruple &theQuadruple, SymbolTable &theSymbolTable);

    void allocate();

    void allocateWithoutSave();

private:
    const vector<string> initPool = {"$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23", "$14", "$15", "$24",
                                     "$25"};

    Quadruple &quadruple;
    SymbolTable &symbolTable;
    vector<string> pool = initPool;

    void refreshPool();
};


#endif //C0COMPILER_GLOBALREGISTERALLOCATION_H
