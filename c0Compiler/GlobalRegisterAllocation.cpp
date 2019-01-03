//
// Created by andyson on 18-12-31.
//

#include "GlobalRegisterAllocation.h"

GlobalRegisterAllocation::GlobalRegisterAllocation(Quadruple &theQuadruple, SymbolTable &theSymbolTable)
        : quadruple(theQuadruple), symbolTable(theSymbolTable) {
    allocationTableList.clear();
    refreshPool();
}

void GlobalRegisterAllocation::allocate() {
    map<string, int> counter;
    string curFunction;
    allocationTable allocationTable1;
    int loop = 0;
    int threshold = 1;
    for (auto &i : quadruple.quadrupleList) {
        if (i->op == FUNCTION_DEFINE || i->op == MAIN_FUNCTION_DEFINE) {
            pool.clear();
            refreshPool();
            curFunction = i->dst;
            allocationTable1.clear();
            counter.clear();
            auto symTable = symbolTable.localSymTableList.find(i->dst);
            if(symTable == symbolTable.localSymTableList.end()){
                printf("Symbol Table Error.\n");
                throw ;
            }
            for(auto &it : symTable->second){
                if((it.second.oType == VARIABLE || it.second.oType == PARAMETER) && it.second.length == 0){
                    counter[it.first] = 0;
                }
            }
            continue;
        }
        if (i->op == LAB) {
            if (i->src1.find("_label_while") != string::npos) {
                loop++;
            }
        }
        if (i->op == J) {
            if (i->src1.find("_label_while") != string::npos) {
                loop--;
            }
        }
        if (i->op == FUNCTION_END) {
            int maxRefer;
            do {
                maxRefer = 0;
                auto maxIt = counter.end();
                for (auto it = counter.begin(); it != counter.end(); ++it) {
                    if (maxRefer < it->second && it->second >= threshold) {
                        maxIt = it;
                        maxRefer = it->second;
                    }
                }
                if (maxIt != counter.end()) {
                    allocationTable1[maxIt->first] = pool[0];
                    pool.erase(pool.begin());
                    counter.erase(maxIt->first);
                }
            } while (maxRefer >= threshold && !pool.empty());
            allocationTableList[curFunction] = allocationTable1;
        }
        map<string, int>::iterator counterIter;
        counterIter = counter.find(i->dst);
        if (counterIter != counter.end()) {
            counter[counterIter->first] += 1 + 5 * loop;
        }
        counterIter = counter.find(i->src1);
        if (counterIter != counter.end()) {
            counter[counterIter->first] += 1 + 5 * loop;
        }
        counterIter = counter.find(i->src2);
        if (counterIter != counter.end()) {
            counter[counterIter->first] += 1 + 5 * loop;
        }

    }
}

//错误的分配方式
void GlobalRegisterAllocation::allocateWithoutSave() {
    map<string, int> counter;
    string curFunction;
    allocationTable allocationTable1;
    int loop = 0;
    int threshold = 3;
    for (auto &i : quadruple.quadrupleList) {
        if (i->op == FUNCTION_DEFINE || i->op == MAIN_FUNCTION_DEFINE) {
            curFunction = i->dst;
            allocationTable1.clear();
            counter.clear();
            continue;
        }
        if (i->op == VAR_DECLARE) {
            if (i->src2 == "1")
                counter[i->dst] = 0;
            continue;
        }
        if (i->op == PARAMETER_DECLARE) {
            continue;
        }
        if (i->op == LAB) {
            if (i->src1.find("_label_while") != string::npos) {
                loop++;
            }
        }
        if (i->op == J) {
            if (i->src1.find("_label_while") != string::npos) {
                loop--;
            }
        }
        if (i->op == FUNCTION_END) {
            int maxRefer = threshold;
            while (maxRefer >= threshold && !pool.empty()) {
                maxRefer = threshold;
                auto maxIt = counter.end();
                for (auto it = counter.begin(); it != counter.end(); ++it) {
                    if (maxRefer <= it->second && it->second >= threshold) {
                        maxIt = it;
                        maxRefer = it->second;
                    }
                }
                if (maxIt != counter.end()) {
                    allocationTable1[maxIt->first] = pool[0];
                    pool.erase(pool.begin());
                    counter.erase(maxIt->first);
                } else {
                    break;
                }
            }
            allocationTableList[curFunction] = allocationTable1;
        }
        map<string, int>::iterator counterIter;
        counterIter = counter.find(i->dst);
        if (counterIter != counter.end()) {
            counter[counterIter->first] += 1 + 5 * loop;
        }
        counterIter = counter.find(i->src1);
        if (counterIter != counter.end()) {
            counter[counterIter->first] += 1 + 5 * loop;
        }
        counterIter = counter.find(i->src2);
        if (counterIter != counter.end()) {
            counter[counterIter->first] += 1 + 5 * loop;
        }

    }
}

void GlobalRegisterAllocation::refreshPool() {
    pool = initPool;
}