//
// Created by andyson on 18-12-31.
//

#include "GlobalRegisterAllocation.h"

GlobalRegisterAllocation::GlobalRegisterAllocation(Quadruple &theQuadruple) :quadruple(theQuadruple){
    allocationTableList.clear();
}

void GlobalRegisterAllocation::allocate() {
    vector<string> pool;
    map<string, int> counter;
    string curFunction = "";
    allocationTable allocationTable1;
    int loop= 0;
    for (int i = 0; i < quadruple.quadrupleList.size(); ++i) {
        if (quadruple.quadrupleList[i]->op == FUNCTION_DEFINE || quadruple.quadrupleList[i]->op == MAIN_FUNCTION_DEFINE) {
            pool.clear();
            pool = {"$11", "$12", "$13", "$14", "$15", "$19", "$21", "$22", "$23", "$24", "$25"};
            curFunction = quadruple.quadrupleList[i]->dst;
            allocationTable1.clear();
            counter.clear();
            continue;
        }
        if(quadruple.quadrupleList[i]->op == VAR_DECLARE){
            if(quadruple.quadrupleList[i]->src2 == "1")
                counter[quadruple.quadrupleList[i]->dst] = 0;
            continue;
        }
        if(quadruple.quadrupleList[i]->op == PARAMETER_DECLARE){
            continue;
        }
        if(quadruple.quadrupleList[i]->op == LAB){
            if(quadruple.quadrupleList[i]->src1.find("_label_while") != string::npos){
                loop ++;
            }
        }
        if(quadruple.quadrupleList[i]->op == J){
            if(quadruple.quadrupleList[i]->src1.find("_label_while") != string::npos){
                loop --;
            }
        }
        if(quadruple.quadrupleList[i]->op == FUNCTION_END) {
            int maxRefer;
            do{
                maxRefer = 0;
                map<string, int>::iterator maxIt = counter.end();
                for (map<string, int>::iterator it = counter.begin(); it != counter.end(); ++it) {
                    if (maxRefer < it->second && it->second >= 3) {
                        maxIt = it;
                        maxRefer = it->second;
                    }
                }
                if(maxIt != counter.end()) {
                    allocationTable1[maxIt->first] = pool[0];
                    pool.erase(pool.begin());
                    counter.erase(maxIt->first);
                }
            }while (maxRefer >= 3 && pool.size() > 0);
            allocationTableList[curFunction] = allocationTable1;
        }
        map<string, int>::iterator counterIter;
        counterIter = counter.find(quadruple.quadrupleList[i]->dst);
        if (counterIter != counter.end()) {
            counter[counterIter->first] += 1 + 5 * loop;
        }
        counterIter = counter.find(quadruple.quadrupleList[i]->src1);
        if (counterIter != counter.end()) {
            counter[counterIter->first] += 1 + 5 * loop;
        }
        counterIter = counter.find(quadruple.quadrupleList[i]->src2);
        if (counterIter != counter.end()) {
            counter[counterIter->first] += 1 + 5 * loop;
        }

    }
}