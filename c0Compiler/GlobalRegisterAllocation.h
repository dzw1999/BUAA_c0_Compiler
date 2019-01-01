//
// Created by andyson on 18-12-31.
//

#ifndef C0COMPILER_GLOBALREGISTERALLOCATION_H
#define C0COMPILER_GLOBALREGISTERALLOCATION_H

#include <map>
#include "Type.h"
#include "Quadruple.h"

using namespace std;

class GlobalRegisterAllocation {
public:
    map<string, allocationTable> allocationTableList;

    GlobalRegisterAllocation(Quadruple &theQuadruple);
    void allocate();
    void allocateWithoutSave();

private:
    Quadruple &quadruple;
    vector<string> pool = {"$11", "$12", "$13", "$14", "$15", "$19", "$21", "$22", "$23", "$24", "$25"};

    void refreshPool();
};


#endif //C0COMPILER_GLOBALREGISTERALLOCATION_H
