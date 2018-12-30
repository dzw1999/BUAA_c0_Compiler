//
// Created by andyson on 18-11-28.
//

#include "SymbolTable.h"

SymbolTable &SymbolTable::getSymbolTable() {
    static SymbolTable instance;
    return instance;
}

symTableEntry SymbolTable::createSte(objectType oType, valueType vType, int length, int parameter, int constValue) {
    symTableEntry Ste;
    Ste.oType = oType;
    Ste.vType = vType;
    Ste.length = length;
    Ste.parameter = parameter;
    Ste.offset = length;
    Ste.constValue = constValue;
    return Ste;
}

bool SymbolTable::addToTable(string ident, symTableEntry Ste, string funcName) {
    //printf("add to %s table : %s\n", funcName == "" ? "global" : funcName.c_str(), ident.c_str());
    if (funcName == "") {
        if(Ste.oType == VARIABLE){
            Ste.offset = globalSymTableOffset;
            globalSymTableOffset += Ste.length == 0 ? 1 : Ste.length;
        } else
            Ste.offset = 0;
        globalSymTable.insert(map<string, symTableEntry>::value_type(ident, Ste));
    } else {
        map<string, symTable>::iterator localSymTableIter;
        localSymTableIter = localSymTableList.find(funcName);
        map<string, int>::iterator localSymTableOffsetIter;
        localSymTableOffsetIter = localSymTableOffsetList.find(funcName);

        if (localSymTableIter != localSymTableList.end()) {
            if(Ste.oType == VARIABLE){
                Ste.offset = localSymTableOffsetIter->second;
                localSymTableOffsetList[funcName] += Ste.length == 0 ? 1 : Ste.length;
            } else
                Ste.offset = 0;
            localSymTableIter->second.insert(map<string, symTableEntry>::value_type(ident, Ste));
        } else
            return false;
    }
    return true;
}

bool SymbolTable::createLocalSymTable(string funcName) {
    if (funcName == "") {
        return false;
    }
    map<string, symTable>::iterator localSymTableIter;
    localSymTableIter = localSymTableList.find(funcName);
    if (localSymTableIter != localSymTableList.end()) {
        return false;
    }
    symTable localSymTable;
    localSymTableList.insert(map<string, symTable>::value_type(funcName, localSymTable));
    localSymTableOffsetList.insert(map<string, int>::value_type(funcName, 0));
    return true;
}

//对于引用
symTableEntry SymbolTable::searchInTable(string ident, string funcName) {
    symTable::iterator it;
    //找函数表
    map<string, symTable>::iterator localSymTableIter;
    localSymTableIter = localSymTableList.find(funcName);
    //如果没找到函数表, fault
    if (localSymTableIter == localSymTableList.end()) {
        symTableEntry fault = SymbolTable::createSte(FAULT, INT_TYPE, -1);
        return fault;
    }
    //从函数表中检索
    symTable localSymTable = localSymTableIter->second;
    symTable::iterator Ste = localSymTable.find(ident);
    if (Ste != localSymTable.end())
        return Ste->second;
    //从全局表中检索
    return searchInGlobalTable(ident);
}

//对于指定参数
symTableEntry SymbolTable::searchInTable(int parameterNum, string funcName) {
    map<string, symTable>::iterator localSymTableIter;
    localSymTableIter = localSymTableList.find(funcName);
    //找函数表
    if (localSymTableIter == localSymTableList.end()) {
        symTableEntry fault = SymbolTable::createSte(FAULT, INT_TYPE, -1);
        return fault;
    }
    symTable localSymTable = localSymTableIter->second;
    symTable::iterator ParameterSteIter;
    ParameterSteIter = localSymTable.begin();
    //找参数
    while (ParameterSteIter != localSymTable.end()) {
        if (ParameterSteIter->second.parameter == parameterNum && ParameterSteIter->second.oType != FUNCTION) {
            return ParameterSteIter->second;
        }
        ParameterSteIter++;
    }
    symTableEntry fault = SymbolTable::createSte(FAULT, INT_TYPE, -1);
    return fault;
}

//只查全局表
symTableEntry SymbolTable::searchInGlobalTable(string ident) {
    symTable::iterator it;
    it = globalSymTable.find(ident);
    if (it == globalSymTable.end()) {
        symTableEntry fault = SymbolTable::createSte(FAULT, INT_TYPE, -1);
        return fault;
    } else
        return it->second;
}

//只查局部表
symTableEntry SymbolTable::searchInLocalTable(string ident, string funcName) {
    symTable::iterator it;
    if (funcName == "") {
        return searchInGlobalTable(ident);
    }
    //找函数表
    map<string, symTable>::iterator localSymTableIter;
    localSymTableIter = localSymTableList.find(funcName);
    //如果没找到函数表, fault
    if (localSymTableIter == localSymTableList.end()) {
        symTableEntry fault = SymbolTable::createSte(FAULT, INT_TYPE, -1);
        return fault;
    }
    //从函数表中检索
    symTable localSymTable = localSymTableIter->second;
    symTable::iterator Ste = localSymTable.find(ident);
    if (Ste != localSymTable.end())
        return Ste->second;
        //没找到
    else {
        symTableEntry fault = SymbolTable::createSte(FAULT, INT_TYPE, -1);
        return fault;
    }
}

//私有构造函数
SymbolTable::SymbolTable() {
    globalSymTableOffset = 0;
}