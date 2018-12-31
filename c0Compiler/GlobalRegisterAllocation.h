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
private:
    Quadruple &quadruple;


};


#endif //C0COMPILER_GLOBALREGISTERALLOCATION_H
