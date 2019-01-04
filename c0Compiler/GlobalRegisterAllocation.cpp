//
// Created by andyson on 18-12-31.
//

#include "GlobalRegisterAllocation.h"

GlobalRegisterAllocation::GlobalRegisterAllocation(Quadruple &theQuadruple, SymbolTable &theSymbolTable)
        : quadruple(theQuadruple), symbolTable(theSymbolTable) {
    allocationTableList.clear();
    refreshPool();
}

void GlobalRegisterAllocation::allocate(bool parameter) {
    map<string, int> counter;
    string curFunction;
    allocationTable allocationTable1;
    allocationTable allocationTable2;
    int loop = 0;
    int threshold = 3;
    for (auto &i : quadruple.quadrupleList) {
        if (i->op == FUNCTION_DEFINE || i->op == MAIN_FUNCTION_DEFINE) {
            pool.clear();
            refreshPool();
            curFunction = i->dst;
            allocationTable1.clear();
            allocationTable2.clear();
            counter.clear();
            auto symTable = symbolTable.localSymTableList.find(i->dst);
            if (symTable == symbolTable.localSymTableList.end()) {
                printf("Symbol Table Error.\n");
                throw;
            }
            for (auto &it : symTable->second) {
                if (parameter) {
                    if ((it.second.oType == VARIABLE || it.second.oType == PARAMETER) && it.second.length == 0) {
                        counter[it.first] = 0;
                    }
                } else {
                    if (it.second.oType == VARIABLE && it.second.length == 0) {
                        counter[it.first] = 0;
                    }
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
                    allocationTable2[pool[0]] = maxIt->first;
                    pool.erase(pool.begin());
                    counter.erase(maxIt->first);
                }
            } while (maxRefer >= threshold && !pool.empty());
            allocationTableList[curFunction] = allocationTable1;
            reverseAllocationTableList[curFunction] = allocationTable2;
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

void GlobalRegisterAllocation::analyzeCallingChain() {
    string curFunc;
    for (auto &i : quadruple.quadrupleList) {
        if (i->op == FUNCTION_DEFINE || i->op == MAIN_FUNCTION_DEFINE) {
            curFunc = i->dst;
            chain[curFunc].clear();
            continue;
        }
        if (i->op == CALL) {
            if (std::find(chain[curFunc].begin(), chain[curFunc].end(), i->src1) == chain[curFunc].end())
                chain[curFunc].push_back(i->src1);
        }
    }

    while (1) {
        auto copy = chain;
        for (auto &i : chain) {
            for (auto &j : i.second) {
                auto line = chain[j];
                for (auto &k : line) {
                    if (std::find(i.second.begin(), i.second.end(), k) == i.second.end()) {
                        i.second.push_back(k);
                    }
                }
            }
        }
        if (chain == copy) {
            break;
        }
    }

    for (auto &i : chain) {
        auto selfAlloTable = allocationTableList.find(i.first);
        int max = 0;
        if (selfAlloTable == allocationTableList.end()) {
            regSave[i.first] = 0;
            continue;
        }
        for (auto &j : i.second) {
            auto alloTable = allocationTableList.find(j);
            if (alloTable != allocationTableList.end()) {
                if (alloTable->second.size() > selfAlloTable->second.size()) {
                    max = selfAlloTable->second.size();
                    break;
                } else {
                    max = alloTable->second.size() > max ? alloTable->second.size() : max;
                }
            }
        }
        regSave[i.first] = max;
    }
}

void GlobalRegisterAllocation::refreshPool() {
    pool = initPool;
}

int GlobalRegisterAllocation::toOrder(string reg) {
    return atoi(reg.substr(1, reg.length() - 1).c_str());
}