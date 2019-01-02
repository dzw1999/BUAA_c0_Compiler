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

    explicit GlobalRegisterAllocation(Quadruple &theQuadruple);
    void allocate();
    void allocateWithoutSave();

private:
    const vector<string> initPool = {"$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23"};
    Quadruple &quadruple;
    vector<string> pool = initPool;

    void refreshPool();
};


#endif //C0COMPILER_GLOBALREGISTERALLOCATION_H
