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

MIPSGenerator &
MIPSGenerator::getMIPSGenerator(SymbolTable &theSymbolTable, StackManager &theStackManager,
                                GlobalRegisterAllocation &theGlobalRegisterAllocation,
                                ExceptionHandler &theExceptionHandler,
                                FILE *theMIPSFile) {
    static MIPSGenerator instance(theSymbolTable, theStackManager, theGlobalRegisterAllocation, theExceptionHandler,
                                  theMIPSFile);
    return instance;
}

void MIPSGenerator::generateMIPS(Quadruple quadruple) {

    for (int i = 0; i < quadruple.length(); ++i) {
        generateMIPSOfQuad(*quadruple.quadrupleList[i]);
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


void MIPSGenerator::generateMIPSOfQuad(Quad quad) {
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
        case CALL: {
            CALLToMIPS(quad);
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
    getSrcToMIPS(quad.src2, 2);
    //src1
    getSrcToMIPS(quad.src1, 1);
    //add
    sprintf(buffer, "addu $s1, $s1, $s2\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //write dst
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
    getSrcToMIPS(quad.src2, 2);
    //src1
    getSrcToMIPS(quad.src1, 1);
    //sub
    sprintf(buffer, "subu $s1, $s1, $s2\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //write dst
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
    getSrcToMIPS(quad.src2, 2);
    //src1
    getSrcToMIPS(quad.src1, 1);
    //mult
    sprintf(buffer, "mult $s1, $s2\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "mflo $s1\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //write dst
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
    getSrcToMIPS(quad.src2, 2);
    //src1
    getSrcToMIPS(quad.src1, 1);
    //div
    sprintf(buffer, "div $s1, $s2\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "mflo $s1\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //write dst
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
    getSrcToMIPS(quad.src1, 1);
    writeDst(quad.dst);
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
    getSrcToMIPS(quad.src1, 2);
    //存数组基地址
    getAddrToMIPS(quad.dst, 1, global);
    sprintf(buffer, "addu $t1, $s1, $0\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //index
    getSrcToMIPS(quad.src2, 1);
    sprintf(buffer, "sll $s1, $s1, 2\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    if (!global)
        sprintf(buffer, "subu $t0, $t1, $s1\n");
    else
        sprintf(buffer, "addu $t0, $t1, $s1\n");
    MIPSTextCode[MIPSTextLine++] = buffer;

    sprintf(buffer, "sw $s2, ($t0)\n");
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
    //arrname
    getAddrToMIPS(quad.src1, 1, global);
    //index
    getSrcToMIPS(quad.src2, 2);
    sprintf(buffer, "sll $s2, $s2, 2\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    if (!global)
        sprintf(buffer, "subu $t0, $s1, $s2\n");
    else
        sprintf(buffer, "addu $t0, $s1, $s2\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "lw $s1, ($t0)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
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
    getSrcToMIPS(quad.src2, 2);
    getSrcToMIPS(quad.src1, 1);
    sprintf(buffer, "bgt $s1, $s2, %s\n", quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::BGEToMIPS(Quad quad) {
    getSrcToMIPS(quad.src2, 2);
    getSrcToMIPS(quad.src1, 1);
    sprintf(buffer, "bge $s1, $s2, %s\n", quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::BLToMIPS(Quad quad) {
    getSrcToMIPS(quad.src2, 2);
    getSrcToMIPS(quad.src1, 1);
    sprintf(buffer, "blt $s1, $s2, %s\n", quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::BLEToMIPS(Quad quad) {
    getSrcToMIPS(quad.src2, 2);
    getSrcToMIPS(quad.src1, 1);
    sprintf(buffer, "ble $s1, $s2, %s\n", quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::BEQToMIPS(Quad quad) {
    getSrcToMIPS(quad.src2, 2);
    getSrcToMIPS(quad.src1, 1);
    sprintf(buffer, "beq $s1, $s2, %s\n", quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::BNEToMIPS(Quad quad) {
    getSrcToMIPS(quad.src2, 2);
    getSrcToMIPS(quad.src1, 1);
    sprintf(buffer, "bne $s1, $s2, %s\n", quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::SWITCH_BNEToMIPS(Quad quad) {
    getSrcToMIPS(quad.src2, 2);
    sprintf(buffer, "bne $s4, $s2, %s\n", quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::SWITCH_VARToMIPS(Quad quad) {
    getSrcToMIPS(quad.dst, 4);
}

void MIPSGenerator::JToMIPS(Quad quad) {
    sprintf(buffer, "j %s\n", quad.src1.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::CALLToMIPS(Quad quad) {
    if (debug) {
        sprintf(buffer, "#call: %s\n", quad.src1.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    //存base
    sprintf(buffer, "sw $s0, ($sp)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addiu $sp, $sp, -4\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //存ra
    sprintf(buffer, "sw $ra, ($sp)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addiu $sp, $sp, -4\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //保存现场
    map<string, allocationTable>::iterator
    alloTableIter = globalRegisterAllocation.allocationTableList.find(stackManager.functionStack.top());
    if(alloTableIter != globalRegisterAllocation.allocationTableList.end()) {
        for (map<string, string>::iterator it = alloTableIter->second.begin();
             it != alloTableIter->second.end(); ++it) {
            sprintf(buffer, "sw %s, ($sp)\n", it->second.c_str());
            MIPSTextCode[MIPSTextLine++] = buffer;
            sprintf(buffer, "addiu $sp, $sp, -4\n");
            MIPSTextCode[MIPSTextLine++] = buffer;
        }
    }
    //改base
    sprintf(buffer, "addi $s0, $sp, %d\n", 8 + 4 * (int)(alloTableIter->second.size()));
    MIPSTextCode[MIPSTextLine++] = buffer;
    // 跳转
    sprintf(buffer, "jal %s\n", quad.src1.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    //恢复现场
    if(alloTableIter != globalRegisterAllocation.allocationTableList.end()) {
        int regOff = -8;
        for (map<string, string>::iterator it = alloTableIter->second.begin();
             it != alloTableIter->second.end(); ++it) {
            sprintf(buffer, "lw %s, %d($s0)\n", it->second.c_str(), regOff);
            MIPSTextCode[MIPSTextLine++] = buffer;
            regOff -= 4;
        }
    }
    //恢复ra
    sprintf(buffer, "lw $ra, -4($s0)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //恢复base
    sprintf(buffer, "lw $s0, ($s0)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
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
    symTableEntry funSte = symbolTable.searchInGlobalTable(stackManager.functionStack.top());
    if (funSte.oType == FAULT) {
        ERROR(54);
    }
    int paraOffset = (funSte.parameter) * 4;
    //无返回值
    if (quad.src1 == "") {
        //恢复sp
        sprintf(buffer, "addi $sp, $s0, %d\n", paraOffset);
        MIPSTextCode[MIPSTextLine++] = buffer;
        sprintf(buffer, "jr $ra\n");
        MIPSTextCode[MIPSTextLine++] = buffer;
    } else {
        //返回 v0
        getSrcToMIPS(quad.src1, 1);
        sprintf(buffer, "addu $v0, $0, $s1\n");
        MIPSTextCode[MIPSTextLine++] = buffer;
        //恢复sp
        sprintf(buffer, "addi $sp, $s0, %d\n", paraOffset);
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
    getSrcToMIPS(quad.src1, 1);
    sprintf(buffer, "sw $s1, ($sp)\n");
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
    sprintf(buffer, "addi $v0, $0, 5\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "syscall\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addu $s1, $0, $v0\n");
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
    sprintf(buffer, "addi $v0, $0, 12\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "syscall\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addu $s1, $0, $v0\n");
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
    sprintf(buffer, "addi $v0, $0, 4\n");
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
    sprintf(buffer, "addi $v0, $0, 1\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    getSrcToMIPS(quad.src1, 1);
    sprintf(buffer, "addu $a0, $0, $s1\n");
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
    sprintf(buffer, "addi $v0, $0, 11\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    getSrcToMIPS(quad.src1, 1);
    sprintf(buffer, "addu $a0, $0, $s1\n");
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
        symTableEntry ste = symbolTable.searchInLocalTable(quad.dst, stackManager.functionStack.top());
        if (ste.oType == FAULT) {
            ERROR(54);
        }
        for (int i = 0; i < atoi(quad.src2.c_str()); ++i) {
            sprintf(buffer, "sw $0, ($sp)\n");
            MIPSTextCode[MIPSTextLine++] = buffer;
            sprintf(buffer, "addiu $sp, $sp, -4\n");
            MIPSTextCode[MIPSTextLine++] = buffer;
        }

    }
}

void MIPSGenerator::MAIN_FUNCTION_DEFINEToMIPS(Quad quad) {
    //贴标签
    sprintf(buffer, "\n%s:\n", quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    //初始化
    sprintf(buffer, "addu $s0, $0, $sp\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "la $ra, end\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //存base
    sprintf(buffer, "sw $s0, ($sp)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addiu $sp, $sp, -4\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //存ra
    sprintf(buffer, "sw $ra, ($sp)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addiu $sp, $sp, -4\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    stackManager.functionStack.push(quad.dst);
}

void MIPSGenerator::FUNCTION_DEFINEToMIPS(Quad quad) {
    stackManager.functionStack.pop();
    sprintf(buffer, "\nj main\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    //贴标签
    sprintf(buffer, "\n%s:\n", quad.dst.c_str());
    MIPSTextCode[MIPSTextLine++] = buffer;
    stackManager.functionStack.push(quad.dst);
}

void MIPSGenerator::PARAMETERToMIPS(Quad quad) {
    return;
}

void MIPSGenerator::FUNCTION_ENDToMIPS(Quad quad) {
    Quad q;
    q.op = RET;
    q.src1 = "";
    RETToMIPS(q);
}

void MIPSGenerator::getAddrToMIPS(string src, int getSrc, bool &global) {
    symTableEntry ste = symbolTable.searchInLocalTable(src, stackManager.functionStack.top());
    symTableEntry gloSte = symbolTable.searchInGlobalTable(src);
    if (ste.oType != FAULT) {
        int MIPSOffset = (ste.offset + 2) * 4;
        sprintf(buffer, "subi $s%d, $s0, %d\n", getSrc, MIPSOffset);
        MIPSTextCode[MIPSTextLine++] = buffer;
    } else if (gloSte.oType != FAULT) {
        global = true;
        sprintf(buffer, "addi $t0, $0, 0x10010000\n");
        MIPSTextCode[MIPSTextLine++] = buffer;
        sprintf(buffer, "addi $s%d, $t0, %d\n", getSrc, gloSte.offset * 4);
        MIPSTextCode[MIPSTextLine++] = buffer;
    } else {
        ERROR(54);
    }
}

void MIPSGenerator::getSrcToMIPS(string src, int quadSrc) {
    if (debug) {
        if (src != "'\n'") {
            sprintf(buffer, "#load %s to s%d\n", src.c_str(), quadSrc);
        } else {
            sprintf(buffer, "#enter\n");
        }
        MIPSTextCode[MIPSTextLine++] = buffer;
    }

    if (src[0] == '\'' || isdigit(src[0]) || src[0] == '-' || src[0] == '+') {
        getConst(src, quadSrc);
    } else {
        map<string, allocationTable>::iterator
        alloTable = globalRegisterAllocation.allocationTableList.find(stackManager.functionStack.top());
        symTableEntry ste = symbolTable.searchInLocalTable(src, stackManager.functionStack.top());
        symTableEntry gloSte = symbolTable.searchInGlobalTable(src);
        int MIPSOffset = (ste.offset + 2 + alloTable->second.size()) * 4;
        if (ste.oType != FAULT) {
            //获取参数
            if (ste.oType == PARAMETER && ste.parameter != 0) {
                symTableEntry funSte = symbolTable.searchInGlobalTable(stackManager.functionStack.top());
                if (funSte.oType == FAULT) {
                    ERROR(54);
                }
                getParameter(funSte.parameter, ste.parameter, quadSrc);

            } else if (ste.oType == CONSTANT) {   //获取局部常量
                getConst(ste, quadSrc);
            } else {        //获取局部变量
                map<string, string>::iterator
                alloTableEntry = alloTable->second.find(src);
                if(alloTableEntry == alloTable->second.end())
                    getLocal(MIPSOffset, quadSrc);
                else{
                    sprintf(buffer, "addu $s%d, $0, %s\n", quadSrc, alloTableEntry->second.c_str());
                    MIPSTextCode[MIPSTextLine++] = buffer;
                }
            }
        } else if (gloSte.oType != FAULT) {
            if (gloSte.oType == CONSTANT) {
                getConst(gloSte, quadSrc);
            } else {        //获取全局变量
                getGlobal(gloSte.offset * 4, quadSrc);
            }
        } else {
            //取栈顶
            sprintf(buffer, "addiu $sp, $sp, 4\n");
            MIPSTextCode[MIPSTextLine++] = buffer;
            sprintf(buffer, "lw $s%d, ($sp)\n", quadSrc);
            MIPSTextCode[MIPSTextLine++] = buffer;
        }
    }
    if (debug) {
        if (src != "'\n'") {
            sprintf(buffer, "#load %s to s%d\n", src.c_str(), quadSrc);
        } else {
            sprintf(buffer, "#end enter\n");
        }
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}


void MIPSGenerator::writeDst(string dst) {
    if (debug) {
        sprintf(buffer, "#write s1 to %s\n", dst.c_str());
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    map<string, allocationTable>::iterator
    alloTable = globalRegisterAllocation.allocationTableList.find(stackManager.functionStack.top());
    symTableEntry ste = symbolTable.searchInLocalTable(dst, stackManager.functionStack.top());
    symTableEntry gloSte = symbolTable.searchInGlobalTable(dst);
    int MIPSOffset = (ste.offset + 2 + alloTable->second.size()) * 4;
    if (ste.oType != FAULT) {
        if (ste.parameter == 0) {
            map<string, allocationTable>::iterator
            alloTable = globalRegisterAllocation.allocationTableList.find(stackManager.functionStack.top());
            map<string, string>::iterator
            alloTableEntry = alloTable->second.find(dst);
            if (alloTableEntry == alloTable->second.end())
                writeLocal(MIPSOffset);
            else {
                sprintf(buffer, "addu %s, $0, $s1\n", alloTableEntry->second.c_str());
                MIPSTextCode[MIPSTextLine++] = buffer;
            }
        } else {
            symTableEntry funSte = symbolTable.searchInGlobalTable(stackManager.functionStack.top());
            if (funSte.oType == FAULT) {
                ERROR(54);
            }
            writeParameter(funSte.parameter, ste.parameter);
        }
    } else if (gloSte.oType != FAULT) {
        writeGlobal(gloSte.offset * 4);

    } else {
        sprintf(buffer, "sw $s1, ($sp)\n");
        MIPSTextCode[MIPSTextLine++] = buffer;
        sprintf(buffer, "addiu $sp, $sp, -4\n");
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
    if (debug) {
        sprintf(buffer, "#end write s1\n");
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::getLocal(int offset, int quadSrc) {
    sprintf(buffer, "addi $t0, $0, %d\n", offset);
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "subu $t0, $s0, $t0\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "lw $s%d, ($t0)\n", quadSrc);
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::getParameter(int paraNum, int paraOrder, int quadSrc) {
    int addr = (paraNum + 1 - paraOrder) * 4;
    sprintf(buffer, "addi $t0, $0, %d\n", addr);
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addu $t0, $s0, $t0\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "lw $s%d, ($t0)\n", quadSrc);
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::getGlobal(int offset, int quadSrc) {
    sprintf(buffer, "addi $t0, $0, 0x10010000\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addi $t0, $t0, %d\n", offset);
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "lw $s%d, ($t0)\n", quadSrc);
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::getConst(string src, int quadSrc) {
    if (src[0] == '\'') {
        sprintf(buffer, "addi $s%d, $0, %d\n", quadSrc, (int) src[1]);
        MIPSTextCode[MIPSTextLine++] = buffer;
    } else if (src[0] == '+' || src[0] == '-' || isdigit(src[0])) {
        sprintf(buffer, "addi $s%d, $0, %d\n", quadSrc, atoi(src.c_str()));
        MIPSTextCode[MIPSTextLine++] = buffer;
    }
}

void MIPSGenerator::getConst(symTableEntry ste, int quadSrc) {
    sprintf(buffer, "addi $s%d, $0, %d\n", quadSrc, ste.constValue);
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::writeLocal(int offset) {
    sprintf(buffer, "addi $t0, $0, %d\n", offset);
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "subu $t0, $s0, $t0\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "sw $s1, ($t0)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::writeParameter(int paraNum, int paraOrder) {
    int addr = (paraNum + 1 - paraOrder) * 4;
    sprintf(buffer, "addi $t0, $0, %d\n", addr);
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addu $t0, $s0, $t0\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "sw $s1, ($t0)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
}

void MIPSGenerator::writeGlobal(int offset) {
    sprintf(buffer, "addi $t0, $0, 0x10010000\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "addi $t0, $t0, %d\n", offset);
    MIPSTextCode[MIPSTextLine++] = buffer;
    sprintf(buffer, "sw $s1, ($t0)\n");
    MIPSTextCode[MIPSTextLine++] = buffer;
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

