//
// Created by andyson on 18-12-1.
//

#include "MIPSGenerator.h"
#include <cstdio>
#include <cctype>
#include <cstring>

#define ERROR(x) \
    exceptionHandler.errorLine = MIPSTextLine; \
    throw x

bool MIPSGenerator::isNum(string a) {
    return isdigit(a[0]) || a[0] == '+' || a[0] == '-';
}

MIPSGenerator &
MIPSGenerator::getMIPSGenerator(SymbolTable &theSymbolTable, StackManager &theStackManager,
                                GlobalRegisterAllocation &theGlobalRegisterAllocation,
                                ExceptionHandler &theExceptionHandler,
                                FILE *theMIPSFile) {
    static MIPSGenerator instance(theSymbolTable, theStackManager, theGlobalRegisterAllocation, theExceptionHandler,
                                  theMIPSFile);
    return instance;
}

void MIPSGenerator::generateMIPS(Quadruple quadruple, bool saveScene) {

    for (int i = 0; i < quadruple.length(); ++i) {
        generateMIPSOfQuad(*quadruple.quadrupleList[i], saveScene);
    }
    fprintf(MIPSFile, ".data\n");
    for (int j = 0; j < MIPSDataLine; ++j) {
        fprintf(MIPSFile, "%s", MIPSDataCode[j].c_str());
    }
    fprintf(MIPSFile, "\n.text\n");
    for (int j = 0; j < MIPSTextLine; ++j) {
        fprintf(MIPSFile, "%s", MIPSTextCode[j].c_str());
    }
    fprintf(MIPSFile, "\nend:\n");
}


void MIPSGenerator::generateMIPSOfQuad(Quad quad, bool saveScene) {
    Operator op = quad.op;
    switch (op) {
        case ADD: {
            ADDToMIPS(quad);
            break;
        }
        case SUB: {
            SUBToMIPS(quad);
            break;
        }
        case MUL: {
            MULToMIPS(quad);
            break;
        }
        case DIV: {
            DIVToMIPS(quad);
            break;
        }
        case ASS: {
            ASSToMIPS(quad);
            break;
        }
        case ARAS: {
            ARASToMIPS(quad);
            break;
        }
        case GAR: {
            GARToMIPS(quad);
            break;
        }
        case LAB: {
            LABToMIPS(quad);
            break;
        }
        case BG: {
            BGToMIPS(quad);
            break;
        }
        case BGE: {
            BGEToMIPS(quad);
            break;
        }
        case BL: {
            BLToMIPS(quad);
            break;
        }
        case BLE: {
            BLEToMIPS(quad);
            break;
        }
        case BEQ: {
            BEQToMIPS(quad);
            break;
        }
        case BNE: {
            BNEToMIPS(quad);
            break;
        }
        case SWITCH_BNE: {
            SWITCH_BNEToMIPS(quad);
            break;
        }
        case SWITCH_VAR: {
            SWITCH_VARToMIPS(quad);
            break;
        }
        case J: {
            JToMIPS(quad);
            break;
        }
        case SAVE_SCENE: {
            if (saveScene)
                SAVE_SCENEToMIPS(quad);
            break;
        }
        case CALL: {
            CALLToMIPS(quad, saveScene);
            break;
        }
        case RET: {
            RETToMIPS(quad);
            break;
        }
        case PUSH: {
            PUSHToMIPS(quad);
            break;
        }
        case RI: {
            RIToMIPS(quad);
            break;
        }
        case RC: {
            RCToMIPS(quad);
            break;
        }
        case WS: {
            WSToMIPS(quad);
            break;
        }
        case WI: {
            WIToMIPS(quad);
            break;
        }
        case WC: {
            WCToMIPS(quad);
            break;
        }
        case VAR_DECLARE: {
            VAR_DECLAREToMIPS(quad);
            break;
        }
        case MAIN_FUNCTION_DEFINE: {
            MAIN_FUNCTION_DEFINEToMIPS(quad);
            break;
        }
        case FUNCTION_DEFINE: {
            FUNCTION_DEFINEToMIPS(quad);
            break;
        }
        case FUNCTION_END: {
            FUNCTION_ENDToMIPS(quad);
            break;
        }
        case PARAMETER_DECLARE: {
            PARAMETERToMIPS(quad);
            break;
        }
        default:
            throw 53;
    }
}


void MIPSGenerator::ADDToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#add: %s = %s + %s\n", quad.dst.c_str(), quad.src1.c_str(), quad.src2.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    //src2
    string op2 = getSrcToMIPS(quad.src2, 2);
    //src1
    string op1 = getSrcToMIPS(quad.src1, 1);
    string dst;
    if (quad.dst == "@t5") {
        dst = "$t5";
    } else {
        dst = "$t1";
        auto alloTable = globalRegisterAllocation.allocationTableList.find(*(stackManager.functionStack.end() - 1));
        if (alloTable != globalRegisterAllocation.allocationTableList.end()) {
            auto alloTableEntry = alloTable->second.find(quad.dst);
            if (alloTableEntry != alloTable->second.end()) {
                dst = alloTableEntry->second;
            }
        }
    }
    //add
    if (isNum(op1) && isNum(op2)) {
        sprintf(buffer, "addiu $t1, $0, %s\n", op1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
        sprintf(buffer, "addiu %s, $t1, %s\n", dst.c_str(), op2.c_str());
    } else if (isNum(op2)) {
        sprintf(buffer, "addiu %s, %s, %s\n", dst.c_str(), op1.c_str(), op2.c_str());
    } else if (isNum(op1)) {
        sprintf(buffer, "addiu %s, %s, %s\n", dst.c_str(), op2.c_str(), op1.c_str());
    } else {
        sprintf(buffer, "addu %s, %s, %s\n", dst.c_str(), op1.c_str(), op2.c_str());
    }
    MIPSTextCode[MIPSTextLine++] = buffer;
    //write dst
    if (dst == "$t1")
        writeDst(quad.dst);

    if (debug) {
        sprintf(buffer, "#end add: %s = %s + %s\n", quad.dst.c_str(), quad.src1.c_str(), quad.src2.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::SUBToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#sub: %s = %s - %s\n", quad.dst.c_str(), quad.src1.c_str(), quad.src2.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    //src2
    string op2 = getSrcToMIPS(quad.src2, 2);
    //src1
    string op1 = getSrcToMIPS(quad.src1, 1);
    string dst;
    if (quad.dst == "@t5") {
        dst = "$t5";
    } else {
        dst = "$t1";
        auto alloTable = globalRegisterAllocation.allocationTableList.find(*(stackManager.functionStack.end() - 1));
        if (alloTable != globalRegisterAllocation.allocationTableList.end()) {
            auto alloTableEntry = alloTable->second.find(quad.dst);
            if (alloTableEntry != alloTable->second.end()) {
                dst = alloTableEntry->second;
            }
        }
    }
    //sub
    if (isNum(op1) && isNum(op2)) {
        sprintf(buffer, "addiu $t1, $0, %s\n", op1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
        sprintf(buffer, "addiu %s, $t1, %d\n", dst.c_str(), -1 * atoi(op2.c_str()));
    } else if (isNum(op2)) {
        sprintf(buffer, "addiu %s, %s, %d\n", dst.c_str(), op1.c_str(), -1 * atoi(op2.c_str()));
    } else if (isNum(op1)) {
        sprintf(buffer, "addiu %s, %s, %d\n", dst.c_str(), op2.c_str(), -1 * atoi(op1.c_str()));
        MIPSTextCode[MIPSTextLine++] = buffer;
        sprintf(buffer, "subu %s, $0, %s\n", dst.c_str(), dst.c_str());
    } else {
        sprintf(buffer, "subu %s, %s, %s\n", dst.c_str(), op1.c_str(), op2.c_str());
    }
    MIPSTextCode[MIPSTextLine++] = buffer;
    //write dst
    if (dst == "$t1")
        writeDst(quad.dst);
    if (debug) {
        sprintf(buffer, "#end sub: %s = %s - %s\n", quad.dst.c_str(), quad.src1.c_str(), quad.src2.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::MULToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#mul: %s = %s * %s\n", quad.dst.c_str(), quad.src1.c_str(), quad.src2.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    //src2
    string op2 = getSrcToMIPS(quad.src2, 2, true);
    //src1
    string op1 = getSrcToMIPS(quad.src1, 1, true);
    string dst;
    if (quad.dst == "@t5") {
        dst = "$t5";
    } else {
        dst = "$t1";
        auto alloTable = globalRegisterAllocation.allocationTableList.find(*(stackManager.functionStack.end() - 1));
        if (alloTable != globalRegisterAllocation.allocationTableList.end()) {
            auto alloTableEntry = alloTable->second.find(quad.dst);
            if (alloTableEntry != alloTable->second.end()) {
                dst = alloTableEntry->second;
            }
        }
    }
    //mult
    sprintf(buffer, "mult %s, %s\n", op1.c_str(), op2.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "mflo %s\n", dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    //write dst
    if (dst == "$t1")
        writeDst(quad.dst);
    if (debug) {
        sprintf(buffer, "#end mul: %s = %s * %s\n", quad.dst.c_str(), quad.src1.c_str(), quad.src2.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::DIVToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#div: %s = %s / %s\n", quad.dst.c_str(), quad.src1.c_str(), quad.src2.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    //src2
    string op2 = getSrcToMIPS(quad.src2, 2, true);
    //src1
    string op1 = getSrcToMIPS(quad.src1, 1, true);
    string dst;
    if (quad.dst == "@t5") {
        dst = "$t5";
    } else {
        dst = "$t1";
        auto alloTable = globalRegisterAllocation.allocationTableList.find(*(stackManager.functionStack.end() - 1));
        if (alloTable != globalRegisterAllocation.allocationTableList.end()) {
            auto alloTableEntry = alloTable->second.find(quad.dst);
            if (alloTableEntry != alloTable->second.end()) {
                dst = alloTableEntry->second;
            }
        }
    }
    //div
    sprintf(buffer, "div %s, %s\n", op1.c_str(), op2.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "mflo %s\n", dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    //write dst
    if (dst == "$t1")
        writeDst(quad.dst);
    if (debug) {
        sprintf(buffer, "#end div: %s = %s / %s\n", quad.dst.c_str(), quad.src1.c_str(), quad.src2.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::ASSToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#assign: %s = %s\n", quad.dst.c_str(), quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    //src1
    string op1 = getSrcToMIPS(quad.src1, 1);
    if (quad.dst == "@t5") {
        sprintf(buffer, "addu $t5, $0, %s\n", op1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    } else
        writeDst(quad.dst, op1);

    if (debug) {
        sprintf(buffer, "#end assign: %s = %s\n", quad.dst.c_str(), quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::ARASToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#assign: %s[%s] = %s\n", quad.dst.c_str(), quad.src2.c_str(), quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    bool global = false;
    //value
    string value = getSrcToMIPS(quad.src1, 2);
    //存数组基地址
    getAddrToMIPS(quad.dst, 3, global);
    //index
    string index = getSrcToMIPS(quad.src2, 1);
    if (isNum(index)) {
        int offset;
        offset = atoi(index.c_str()) * 4;
        if (!global)
            sprintf(buffer, "addiu $t0, $t3, %d\n", -1 * offset);
        else
            sprintf(buffer, "addiu $t0, $t3, %d\n", offset);
    } else {
        sprintf(buffer, "sll $t1, %s, 2\n", index.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
        if (!global)
            sprintf(buffer, "subu $t0, $t3, $t1\n");
        else
            sprintf(buffer, "addu $t0, $t3, $t1\n");
    }
    MIPSTextCode[MIPSTextLine++] = buffer;
    if (isNum(value)) {
        sprintf(buffer, "addiu $t2, $0, %s\n", value.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
        value = "$t2";
    }
    sprintf(buffer, "sw %s, ($t0)\n", value.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    if (debug) {
        sprintf(buffer, "#end assign: %s[%s] = %s\n", quad.dst.c_str(), quad.src2.c_str(), quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::GARToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#get: %s = %s[%s]\n", quad.dst.c_str(), quad.src1.c_str(), quad.src2.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    bool global = false;
    string dst;
    if (quad.dst == "@t5") {
        dst = "$t5";
    } else {
        dst = "$t1";
        auto alloTable = globalRegisterAllocation.allocationTableList.find(*(stackManager.functionStack.end() - 1));
        if (alloTable != globalRegisterAllocation.allocationTableList.end()) {
            auto alloTableEntry = alloTable->second.find(quad.dst);
            if (alloTableEntry != alloTable->second.end()) {
                dst = alloTableEntry->second;
            }
        }
    }
    //arrname
    getAddrToMIPS(quad.src1, 1, global);
    //index
    string index = getSrcToMIPS(quad.src2, 2);
    if (isNum(index)) {
        int offset;
        offset = atoi(index.c_str()) * 4;
        if (!global)
            sprintf(buffer, "addiu $t0, $t1, %d\n", -1 * offset);
        else
            sprintf(buffer, "addiu $t0, $t1, %d\n", offset);
    } else {
        sprintf(buffer, "sll $t2, %s, 2\n", index.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
        if (!global)
            sprintf(buffer, "subu $t0, $t1, $t2\n");
        else
            sprintf(buffer, "addu $t0, $t1, $t2\n");
    }
    MIPSTextCode[MIPSTextLine++] = buffer;

    sprintf(buffer, "lw %s, ($t0)\n", dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;

    if (dst == "$t1")
        writeDst(quad.dst);
    if (debug) {
        sprintf(buffer, "#end get: %s = %s[%s]\n", quad.dst.c_str(), quad.src1.c_str(), quad.src2.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::LABToMIPS(Quad quad) {
    sprintf(buffer, "\n%s:\n", quad.src1.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::BGToMIPS(Quad quad) {
    string op2 = getSrcToMIPS(quad.src2, 2, true);
    string op1 = getSrcToMIPS(quad.src1, 1, true);
    sprintf(buffer, "bgt %s, %s, %s\n", op1.c_str(), op2.c_str(), quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::BGEToMIPS(Quad quad) {
    string op2 = getSrcToMIPS(quad.src2, 2, true);
    string op1 = getSrcToMIPS(quad.src1, 1, true);
    sprintf(buffer, "bge %s, %s, %s\n", op1.c_str(), op2.c_str(), quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::BLToMIPS(Quad quad) {
    string op2 = getSrcToMIPS(quad.src2, 2, true);
    string op1 = getSrcToMIPS(quad.src1, 1, true);
    sprintf(buffer, "blt %s, %s, %s\n", op1.c_str(), op2.c_str(), quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::BLEToMIPS(Quad quad) {
    string op2 = getSrcToMIPS(quad.src2, 2, true);
    string op1 = getSrcToMIPS(quad.src1, 1, true);
    sprintf(buffer, "ble %s, %s, %s\n", op1.c_str(), op2.c_str(), quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::BEQToMIPS(Quad quad) {
    string op2 = getSrcToMIPS(quad.src2, 2, true);
    string op1 = getSrcToMIPS(quad.src1, 1, true);
    sprintf(buffer, "beq %s, %s, %s\n", op1.c_str(), op2.c_str(), quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::BNEToMIPS(Quad quad) {
    string op2 = getSrcToMIPS(quad.src2, 2, true);
    string op1 = getSrcToMIPS(quad.src1, 1, true);
    sprintf(buffer, "bne %s, %s, %s\n", op1.c_str(), op2.c_str(), quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::SWITCH_BNEToMIPS(Quad quad) {
    string caseVar = getSrcToMIPS(quad.src2, 2, true);
    sprintf(buffer, "bne $t4, %s, %s\n", caseVar.c_str(), quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::SWITCH_VARToMIPS(Quad quad) {
    string var = getSrcToMIPS(quad.dst, 4);
    if (isNum(var)) {
        sprintf(buffer, "addiu $t4, $0, %s\n", var.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    } else if (var != "$t4") {
        sprintf(buffer, "addu $t4, $0, %s\n", var.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::JToMIPS(Quad quad) {
    sprintf(buffer, "j %s\n", quad.src1.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::SAVE_SCENEToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#save scene: %s\n", quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    auto alloTable = globalRegisterAllocation.reverseAllocationTableList.find(*(stackManager.functionStack.end() - 1));
    if (alloTable != globalRegisterAllocation.reverseAllocationTableList.end()) {
        int cnt = 0;
        for(auto &it : alloTable->second){
            if (cnt >= globalRegisterAllocation.regSave[quad.src1]) {
                break;
            }
            sprintf(buffer, "sw %s, ($sp)\n", it.first.c_str());
            MIPSTextCode[MIPSTextLine++] = buffer;
            sprintf(buffer, "addiu $sp, $sp, -4\n");
            MIPSTextCode[MIPSTextLine++] = buffer;
            cnt++;
        }
    }
    if (debug) {
        sprintf(buffer, "#end save scene: %s\n", quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::CALLToMIPS(Quad quad, bool saveScene) {
    if (debug) {
        sprintf(buffer, "#call: %s\n", quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    //存base
    sprintf(buffer, "sw $fp, ($sp)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addiu $sp, $sp, -4\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //存ra
    sprintf(buffer, "sw $ra, ($sp)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addiu $sp, $sp, -4\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //改base
    sprintf(buffer, "addiu $fp, $sp, %d\n", 8);
    MIPSTextCode[MIPSTextLine++] = buffer;
    // 跳转
    sprintf(buffer, "jal %s\n", quad.src1.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    //恢复ra
    sprintf(buffer, "lw $ra, -4($fp)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //恢复base
    sprintf(buffer, "lw $fp, ($fp)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //恢复现场
    if (saveScene) {
        auto alloTable = globalRegisterAllocation.reverseAllocationTableList.find(*(stackManager.functionStack.end() - 1));
        if (alloTable != globalRegisterAllocation.reverseAllocationTableList.end()) {
            int cnt = alloTable->second.size() -
                      globalRegisterAllocation.regSave[*(stackManager.functionStack.end() - 1)];
            for(allocationTable::reverse_iterator rit = alloTable->second.rbegin(); rit != alloTable->second.rend(); ++rit){
                if (cnt > 0) {
                    cnt--;
                    continue;
                }
                sprintf(buffer, "addiu $sp, $sp, 4\n");
                MIPSTextCode[MIPSTextLine++] = buffer;
                sprintf(buffer, "lw %s, ($sp)\n", rit->first.c_str());
                MIPSTextCode[MIPSTextLine++] = buffer;
            }
        }
    }
    //返回值入栈
    sprintf(buffer, "sw $v0, ($sp)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addiu $sp, $sp, -4\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    if (debug) {
        sprintf(buffer, "#end call: %s\n", quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::RETToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#return: %s\n", quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    symTableEntry funSte = symbolTable.searchInGlobalTable(*(stackManager.functionStack.end() - 1));
    if (funSte.oType == FAULT) {
        ERROR(54);
    }
    int paraOffset = (funSte.parameter) * 4;
    //无返回值
    if (quad.src1.empty()) {
        //恢复sp
        sprintf(buffer, "addiu $sp, $fp, %d\n", paraOffset);
        MIPSTextCode[MIPSTextLine++] = buffer;
        sprintf(buffer, "jr $ra\n");
        MIPSTextCode[MIPSTextLine++] = buffer;
    } else {
        //返回 v0
        string ret = getSrcToMIPS(quad.src1, 1);
        if (isNum(ret))
            sprintf(buffer, "addiu $v0, $0, %s\n", ret.c_str());
        else
            sprintf(buffer, "addu $v0, $0, %s\n", ret.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
        //恢复sp
        sprintf(buffer, "addiu $sp, $fp, %d\n", paraOffset);
        MIPSTextCode[MIPSTextLine++] = buffer;
        sprintf(buffer, "jr $ra\n");
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    if (debug) {
        sprintf(buffer, "#end return: %s\n", quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::PUSHToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#push: %s\n", quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    string para = getSrcToMIPS(quad.src1, 1, true);
    sprintf(buffer, "sw %s, ($sp)\n", para.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addiu $sp, $sp, -4\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    if (debug) {
        sprintf(buffer, "#end push: %s\n", quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::RIToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#read int:  %s\n", quad.dst.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    sprintf(buffer, "addiu $v0, $0, 5\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "syscall\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addu $t1, $0, $v0\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    writeDst(quad.dst);
    if (debug) {
        sprintf(buffer, "#end read int:  %s\n", quad.dst.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::RCToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#read char:  %s\n", quad.dst.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    sprintf(buffer, "addiu $v0, $0, 12\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "syscall\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addu $t1, $0, $v0\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    writeDst(quad.dst);
    if (debug) {
        sprintf(buffer, "#end read char:  %s\n", quad.dst.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::WSToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#write string:  %s\n", quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    static string str = "str";
    static int a = 0;
    string strLabel = str;
    strLabel.append(to_string(a));
    string outStr = quad.src1;
    sprintf(buffer, "%s: .asciiz %s\n", strLabel.c_str(), outStr.c_str());
    MIPSDataCode[MIPSDataLine++] = buffer;
    sprintf(buffer, "addiu $v0, $0, 4\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "la $a0, %s\n", strLabel.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "syscall\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    a++;
    if (debug) {
        sprintf(buffer, "#end write string:  %s\n", quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::WIToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#write int:  %s\n", quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    sprintf(buffer, "addiu $v0, $0, 1\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    string op1 = getSrcToMIPS(quad.src1, 1);
    if (isNum(op1)) {
        sprintf(buffer, "addiu $a0, $0, %s\n", op1.c_str());
    } else
        sprintf(buffer, "addu $a0, $0, %s\n", op1.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "syscall\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    if (debug) {
        sprintf(buffer, "#end write int:  %s\n", quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::WCToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#write char\n");
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    sprintf(buffer, "addiu $v0, $0, 11\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    string op1 = getSrcToMIPS(quad.src1, 1);
    if (isNum(op1)) {
        sprintf(buffer, "addiu $a0, $0, %s\n", op1.c_str());
    } else
        sprintf(buffer, "addu $a0, $0, %s\n", op1.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "syscall\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    if (debug) {
        sprintf(buffer, "#end write char\n");
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::VAR_DECLAREToMIPS(Quad quad) {
    if (quad.global) {
        symTableEntry ste = symbolTable.searchInGlobalTable(quad.dst);
        if (ste.oType == FAULT) {
            ERROR(54);
        }
        sprintf(buffer, "%s: .space %d\n", quad.dst.c_str(), atoi(quad.src2.c_str()) * 4);
        MIPSDataCode[MIPSDataLine++] = buffer;
    } else {
        symTableEntry ste = symbolTable.searchInLocalTable(quad.dst, *(stackManager.functionStack.end() - 1));
        if (ste.oType == FAULT) {
            ERROR(54);
        }
        sprintf(buffer, "addiu $sp, $sp, %d\n", -4 * (ste.offset + ste.length + 1));
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::MAIN_FUNCTION_DEFINEToMIPS(Quad quad) {
    //贴标签
    sprintf(buffer, "\n%s:\n", quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    //初始化
    sprintf(buffer, "addu $fp, $0, $sp\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "la $ra, end\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //存base
    sprintf(buffer, "sw $fp, ($sp)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addiu $sp, $sp, -4\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //存ra
    sprintf(buffer, "sw $ra, ($sp)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addiu $sp, $sp, -4\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    stackManager.functionStack.push_back(quad.dst);
}

void MIPSGenerator::FUNCTION_DEFINEToMIPS(Quad quad) {
    stackManager.functionStack.pop_back();
    sprintf(buffer, "\nj main\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //贴标签
    sprintf(buffer, "\n%s:\n", quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    stackManager.functionStack.push_back(quad.dst);
}

void MIPSGenerator::PARAMETERToMIPS(Quad quad, bool parameter) {
    if (parameter) {
        auto alloTable = globalRegisterAllocation.allocationTableList.find(*(stackManager.functionStack.end() - 1));
        if (alloTable != globalRegisterAllocation.allocationTableList.end()) {
            auto alloTableEntry = alloTable->second.find(quad.dst);
            if (alloTableEntry != alloTable->second.end()) {
                string para = getSrcToMIPS(quad.dst, 1, true, true);
                sprintf(buffer, "addu %s, $0, $t1\n", alloTableEntry->second.c_str());
                MIPSTextCode[MIPSTextLine++] = buffer;
            }
        }
    }
}

void MIPSGenerator::FUNCTION_ENDToMIPS(Quad quad) {
    Quad q;
    q.op = RET;
    q.src1 = "";
    RETToMIPS(q);
}

void MIPSGenerator::getAddrToMIPS(string src, int getSrc, bool &global) {
    symTableEntry ste = symbolTable.searchInLocalTable(src, *(stackManager.functionStack.end() - 1));
    symTableEntry gloSte = symbolTable.searchInGlobalTable(src);
    if (ste.oType != FAULT) {
        int MIPSOffset = (ste.offset + 2) * 4;
        sprintf(buffer, "addiu $t%d, $fp, %d\n", getSrc, -1 * MIPSOffset);
        MIPSTextCode[MIPSTextLine++] = buffer;
    } else if (gloSte.oType != FAULT) {
        global = true;
        int globalBase = 268500992;
        sprintf(buffer, "addiu $t%d, $0, %d\n", getSrc, globalBase + gloSte.offset * 4);
        MIPSTextCode[MIPSTextLine++] = buffer;
    } else {
        ERROR(54);
    }
}

string MIPSGenerator::getSrcToMIPS(string src, int quadSrc, bool intoReg, bool paraDeclare) {
    char ret[5];
    sprintf(ret, "$t%d", quadSrc);
    if (debug) {
        if (src != "'\n'") {
            sprintf(buffer, "#load %s\n", src.c_str());
        } else {
            sprintf(buffer, "#enter\n");
        }
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    //如是常量或@t5，直接返回
    if (src[0] == '\'' || isdigit(src[0]) || src[0] == '-' || src[0] == '+') {
        if (intoReg) {
            getConst(src, quadSrc);
        } else if (src[0] == '\'') {
            return to_string((int) src[1]);
        } else {
            return src;
        }
    } else if (src == "@t5") {
        return "$t5";
    } else {
        auto alloTable = globalRegisterAllocation.allocationTableList.find(*(stackManager.functionStack.end() - 1));
        symTableEntry ste = symbolTable.searchInLocalTable(src, *(stackManager.functionStack.end() - 1));
        symTableEntry gloSte = symbolTable.searchInGlobalTable(src);
        int MIPSOffset = (ste.offset + 2) * 4;
        if (ste.oType != FAULT) {
            if (ste.oType == CONSTANT) {   //获取局部常量
                if (intoReg) {
                    getConst(ste, quadSrc);
                } else
                    return to_string(ste.constValue);
            } else {        //获取局部变量
                if (!paraDeclare) {
                    if (alloTable != globalRegisterAllocation.allocationTableList.end()) {
                        auto alloTableEntry = alloTable->second.find(src);
                        if (alloTableEntry != alloTable->second.end())
                            return alloTableEntry->second;
                    }
                }
                if (ste.oType == PARAMETER && ste.parameter != 0) {
                    symTableEntry funSte = symbolTable.searchInGlobalTable(*(stackManager.functionStack.end() - 1));
                    if (funSte.oType == FAULT) {
                        ERROR(54);
                    }
                    getParameter(funSte.parameter, ste.parameter, quadSrc);
                } else {
                    getLocal(MIPSOffset, quadSrc);
                }
            }
        } else if (gloSte.oType != FAULT) {
            if (gloSte.oType == CONSTANT) { //全局常量
                if (intoReg) {
                    getConst(gloSte, quadSrc);
                } else
                    return to_string(gloSte.constValue);
            } else {        //获取全局变量
                getGlobal(gloSte.offset * 4, quadSrc);
            }
        } else {
            //取栈顶
            sprintf(buffer, "addiu $sp, $sp, 4\n");
            MIPSTextCode[MIPSTextLine++] = buffer;
            sprintf(buffer, "lw $t%d, ($sp)\n", quadSrc);
            MIPSTextCode[MIPSTextLine++] = buffer;
        }
    }
    if (debug) {
        if (src != "'\n'") {
            sprintf(buffer, "#load %s to t%d\n", src.c_str(), quadSrc);
        } else {
            sprintf(buffer, "#end enter\n");
        }
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    return ret;
}


void MIPSGenerator::writeDst(string dst, string reg) {
    if (debug) {
        sprintf(buffer, "#write %s to %s\n", reg.c_str(), dst.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    symTableEntry ste = symbolTable.searchInLocalTable(dst, *(stackManager.functionStack.end() - 1));
    symTableEntry gloSte = symbolTable.searchInGlobalTable(dst);
    int MIPSOffset = (ste.offset + 2) * 4;
    if (ste.oType != FAULT) {
        auto alloTable = globalRegisterAllocation.allocationTableList.find(*(stackManager.functionStack.end() - 1));
        if (alloTable != globalRegisterAllocation.allocationTableList.end()) {
            auto alloTableEntry = alloTable->second.find(dst);
            if (alloTableEntry != alloTable->second.end()) {
                if (isNum(reg))
                    sprintf(buffer, "addiu %s, $0, %s\n", alloTableEntry->second.c_str(), reg.c_str());
                else
                    sprintf(buffer, "addu %s, $0, %s\n", alloTableEntry->second.c_str(), reg.c_str());
                MIPSTextCode[MIPSTextLine++] = buffer;
                return;
            }
        }
        if (ste.oType == PARAMETER && ste.parameter != 0) {
            symTableEntry funSte = symbolTable.searchInGlobalTable(*(stackManager.functionStack.end() - 1));
            if (funSte.oType == FAULT) {
                ERROR(54);
            }
            writeParameter(funSte.parameter, ste.parameter, reg);
        } else {
            writeLocal(MIPSOffset, reg);
        }
    } else if (gloSte.oType != FAULT) {
        writeGlobal(gloSte.offset * 4, reg);
    } else {
        if (isNum(reg)) {
            sprintf(buffer, "addiu $t1, $0, %s\n", reg.c_str());
            MIPSTextCode[MIPSTextLine++] = buffer;
            reg = "$t1";
        }
        sprintf(buffer, "sw %s, ($sp)\n", reg.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
        sprintf(buffer, "addiu $sp, $sp, -4\n");
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    if (debug) {
        sprintf(buffer, "#end write %s to %s\n", reg.c_str(), dst.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::getLocal(int offset, int quadSrc) {
    sprintf(buffer, "addiu $t0, $0, %d\n", offset);
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "subu $t0, $fp, $t0\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "lw $t%d, ($t0)\n", quadSrc);
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::getParameter(int paraNum, int paraOrder, int quadSrc) {
    int addr = (paraNum + 1 - paraOrder) * 4;
    sprintf(buffer, "addiu $t0, $fp, %d\n", addr);
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "lw $t%d, ($t0)\n", quadSrc);
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::getGlobal(int offset, int quadSrc) {
    int globalBase = 268500992;
    sprintf(buffer, "addiu $t0, $0, %d\n", globalBase + offset);
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "lw $t%d, ($t0)\n", quadSrc);
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::getConst(string src, int quadSrc) {
    if (src[0] == '\'') {
        sprintf(buffer, "addiu $t%d, $0, %d\n", quadSrc, (int) src[1]);
        MIPSTextCode[MIPSTextLine++] = buffer;
    } else if (src[0] == '+' || src[0] == '-' || isdigit(src[0])) {
        sprintf(buffer, "addiu $t%d, $0, %d\n", quadSrc, atoi(src.c_str()));
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::getConst(symTableEntry ste, int quadSrc) {
    sprintf(buffer, "addiu $t%d, $0, %d\n", quadSrc, ste.constValue);
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::writeLocal(int offset, string reg) {
    sprintf(buffer, "addiu $t0, $fp, %d\n", -1 * offset);
    MIPSTextCode[MIPSTextLine++] = buffer;
    if (isNum(reg)) {
        sprintf(buffer, "addiu $t1, $0, %s\n", reg.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
        reg = "$t1";
    }
    sprintf(buffer, "sw %s, ($t0)\n", reg.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::writeParameter(int paraNum, int paraOrder, string reg) {
    int addr = (paraNum + 1 - paraOrder) * 4;
    sprintf(buffer, "addiu $t0, $fp, %d\n", addr);
    MIPSTextCode[MIPSTextLine++] = buffer;
    if (isNum(reg)) {
        sprintf(buffer, "addiu $t1, $0, %s\n", reg.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
        reg = "$t1";
    }
    sprintf(buffer, "sw %s, ($t0)\n", reg.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::writeGlobal(int offset, string reg) {
    int globalBase = 268500992;
    sprintf(buffer, "addiu $t0, $0, %d\n", globalBase + offset);
    MIPSTextCode[MIPSTextLine++] = buffer;
    if (isNum(reg)) {
        sprintf(buffer, "addiu $t1, $0, %s\n", reg.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
        reg = "$t1";
    }
    sprintf(buffer, "sw %s, ($t0)\n", reg.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

int MIPSGenerator::toOrder(string reg) {
    return atoi(reg.substr(1, reg.length() - 1).c_str());
}

MIPSGenerator::MIPSGenerator(SymbolTable &theSymbolTable, StackManager &theStackManager,
                             GlobalRegisterAllocation &theGlobalRegisterAllocation,
                             ExceptionHandler &theExceptionHandler,
                             FILE *theMIPSFile)
        : symbolTable(theSymbolTable),
          stackManager(theStackManager),
          globalRegisterAllocation(theGlobalRegisterAllocation),
          exceptionHandler(theExceptionHandler) {
    MIPSFile = theMIPSFile;
    MIPSTextLine = 0;
    debug = true;
}

