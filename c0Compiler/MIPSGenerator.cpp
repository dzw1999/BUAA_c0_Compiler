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
MIPSGenerator::getMIPSGenerator(Quadruple &theQuadruple, SymbolTable &theSymbolTable, StackManager &theStackManager,
                                ExceptionHandler &theExceptionHandler,
                                FILE *theMIPSFile) {
    static MIPSGenerator instance(theQuadruple, theSymbolTable, theStackManager, theExceptionHandler, theMIPSFile);
    return instance;
}

void MIPSGenerator::generateMIPS() {
    for (int i = 0; i < quadruple.length(); ++i) {
        generateMIPSOfQuad(quadruple.quadrupleList[i]);
    }
    fprintf(MIPSFile, ".data\n");
    for (int j = 0; j < MIPSDataLine; ++j) {
        fprintf(MIPSFile, "%s", MIPSDataCode[j]);
    }
    fprintf(MIPSFile, "\n.text\n");
    for (int j = 0; j < MIPSTextLine; ++j) {
        fprintf(MIPSFile, "%s", MIPSTextCode[j]);
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
        case CONST_DECLARE: {
            CONST_DECLAREToMIPS(quad);
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
    sprintf(MIPSTextCode[MIPSTextLine++], "#add: %s = %s + %s\n", quad.dst.c_str(), quad.src1.c_str(),
            quad.src2.c_str());
    //src1
    getSrcToMIPS(quad.src1, 1);
    //src2
    getSrcToMIPS(quad.src2, 2);
    //add
    sprintf(MIPSTextCode[MIPSTextLine++], "add $s3, $s1, $s2\n");
    //write dst
    writeDst(quad.dst);
    sprintf(MIPSTextCode[MIPSTextLine++], "#end add: %s = %s + %s\n", quad.dst.c_str(), quad.src1.c_str(),
            quad.src2.c_str());
}

void MIPSGenerator::SUBToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#sub: %s = %s - %s\n", quad.dst.c_str(), quad.src1.c_str(),
            quad.src2.c_str());
    //src2
    getSrcToMIPS(quad.src2, 2);
    //src1
    getSrcToMIPS(quad.src1, 1);
    //sub
    sprintf(MIPSTextCode[MIPSTextLine++], "sub $s3, $s1, $s2\n");
    //write dst
    writeDst(quad.dst);
    sprintf(MIPSTextCode[MIPSTextLine++], "#end sub: %s = %s - %s\n", quad.dst.c_str(), quad.src1.c_str(),
            quad.src2.c_str());
}

void MIPSGenerator::MULToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#mul: %s = %s * %s\n", quad.dst.c_str(), quad.src1.c_str(),
            quad.src2.c_str());
    //src1
    getSrcToMIPS(quad.src1, 1);
    //src2
    getSrcToMIPS(quad.src2, 2);
    //mult
    sprintf(MIPSTextCode[MIPSTextLine++], "mult $s1, $s2\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "mflo $s3\n");
    //write dst
    writeDst(quad.dst);
    sprintf(MIPSTextCode[MIPSTextLine++], "#end mul: %s = %s * %s\n", quad.dst.c_str(), quad.src1.c_str(),
            quad.src2.c_str());
}

void MIPSGenerator::DIVToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#div: %s = %s / %s\n", quad.dst.c_str(), quad.src1.c_str(),
            quad.src2.c_str());
    //src2
    getSrcToMIPS(quad.src2, 2);
    //src1
    getSrcToMIPS(quad.src1, 1);
    //div
    sprintf(MIPSTextCode[MIPSTextLine++], "div $s1, $s2\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "mflo $s3\n");
    //write dst
    writeDst(quad.dst);
    sprintf(MIPSTextCode[MIPSTextLine++], "#end div: %s = %s / %s\n", quad.dst.c_str(), quad.src1.c_str(),
            quad.src2.c_str());
}

void MIPSGenerator::ASSToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#assign: %s = %s\n", quad.dst.c_str(), quad.src1.c_str());
    //src1
    getSrcToMIPS(quad.src1, 1);
    sprintf(MIPSTextCode[MIPSTextLine++], "add $s3, $0, $s1\n");
    writeDst(quad.dst);
    sprintf(MIPSTextCode[MIPSTextLine++], "#end assign: %s = %s\n", quad.dst.c_str(), quad.src1.c_str());
}

void MIPSGenerator::ARASToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#assign: %s[%s] = %s\n", quad.dst.c_str(), quad.src2.c_str(),
            quad.src1.c_str());
    bool global = false;
    getAddrtoMIPS(quad.dst, 2, global);
    //value
    getSrcToMIPS(quad.src1, 1);
    //存数组基地址
    sprintf(MIPSTextCode[MIPSTextLine++], "add $t1, $s2, $0\n");
    //index
    getSrcToMIPS(quad.src2, 2);
    sprintf(MIPSTextCode[MIPSTextLine++], "sll $s2, $s2, 2\n");
    if(!global)
        sprintf(MIPSTextCode[MIPSTextLine++], "sub $t0, $t1, $s2\n");
    else
        sprintf(MIPSTextCode[MIPSTextLine++], "add $t0, $t1, $s2\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "sw $s1, ($t0)\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "#end assign: %s[%s] = %s\n", quad.dst.c_str(), quad.src2.c_str(),
            quad.src1.c_str());
}

void MIPSGenerator::GARToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#get: %s = %s[%s]\n", quad.dst.c_str(), quad.src1.c_str(),
            quad.src2.c_str());
    bool global = false;
    //arrname
    getAddrtoMIPS(quad.src1, 1, global);
    //index
    getSrcToMIPS(quad.src2, 2);
    sprintf(MIPSTextCode[MIPSTextLine++], "sll $s2, $s2, 2\n");
    if(!global)
        sprintf(MIPSTextCode[MIPSTextLine++], "sub $t0, $s1, $s2\n");
    else
        sprintf(MIPSTextCode[MIPSTextLine++], "add $t0, $s1, $s2\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "lw $s3, ($t0)\n");
    writeDst(quad.dst);
    sprintf(MIPSTextCode[MIPSTextLine++], "#end get: %s = %s[%s]\n", quad.dst.c_str(), quad.src1.c_str(),
            quad.src2.c_str());
}

void MIPSGenerator::LABToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "\n%s:\n", quad.src1.c_str());
}

void MIPSGenerator::BGToMIPS(Quad quad) {
    getSrcToMIPS(quad.src1, 1);
    getSrcToMIPS(quad.src2, 2);
    sprintf(MIPSTextCode[MIPSTextLine++], "bgt $s1, $s2, %s\n", quad.dst.c_str());
}

void MIPSGenerator::BGEToMIPS(Quad quad) {
    getSrcToMIPS(quad.src1, 1);
    getSrcToMIPS(quad.src2, 2);
    sprintf(MIPSTextCode[MIPSTextLine++], "bge $s1, $s2, %s\n", quad.dst.c_str());
}

void MIPSGenerator::BLToMIPS(Quad quad) {
    getSrcToMIPS(quad.src1, 1);
    getSrcToMIPS(quad.src2, 2);
    sprintf(MIPSTextCode[MIPSTextLine++], "blt $s1, $s2, %s\n", quad.dst.c_str());
}

void MIPSGenerator::BLEToMIPS(Quad quad) {
    getSrcToMIPS(quad.src1, 1);
    getSrcToMIPS(quad.src2, 2);
    sprintf(MIPSTextCode[MIPSTextLine++], "ble $s1, $s2, %s\n", quad.dst.c_str());
}

void MIPSGenerator::BEQToMIPS(Quad quad) {
    getSrcToMIPS(quad.src1, 1);
    getSrcToMIPS(quad.src2, 2);
    sprintf(MIPSTextCode[MIPSTextLine++], "beq $s1, $s2, %s\n", quad.dst.c_str());
}

void MIPSGenerator::BNEToMIPS(Quad quad) {
    getSrcToMIPS(quad.src1, 1);
    getSrcToMIPS(quad.src2, 2);
    sprintf(MIPSTextCode[MIPSTextLine++], "bne $s1, $s2, %s\n", quad.dst.c_str());
}

void MIPSGenerator::SWITCH_BNEToMIPS(Quad quad) {
    getSrcToMIPS(quad.src2, 2);
    sprintf(MIPSTextCode[MIPSTextLine++], "bne $s4, $s2, %s\n", quad.dst.c_str());
}

void MIPSGenerator::SWITCH_VARToMIPS(Quad quad) {
    getSrcToMIPS(quad.dst, 4);
}

void MIPSGenerator::JToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "j %s\n", quad.src1.c_str());
}

void MIPSGenerator::CALLToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#call: %s\n", quad.src1.c_str());
    //存base
    sprintf(MIPSTextCode[MIPSTextLine++], "sw $s0, ($sp)\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "addiu $sp, $sp, -4\n");
    //存ra
    sprintf(MIPSTextCode[MIPSTextLine++], "sw $ra, ($sp)\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "addiu $sp, $sp, -4\n");
    //改base
    sprintf(MIPSTextCode[MIPSTextLine++], "addi $s0, $sp, 8\n");
    // 跳转
    sprintf(MIPSTextCode[MIPSTextLine++], "jal %s\n", quad.src1.c_str());
    //恢复ra
    sprintf(MIPSTextCode[MIPSTextLine++], "subi $t0, $s0, 4\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "lw $ra, ($t0)\n");
    //恢复base
    sprintf(MIPSTextCode[MIPSTextLine++], "lw $s0, ($s0)\n");
    //返回值入栈
    sprintf(MIPSTextCode[MIPSTextLine++], "sw $v0, ($sp)\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "addiu $sp, $sp, -4\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "#end call: %s\n", quad.src1.c_str());
}

void MIPSGenerator::RETToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#return: %s\n", quad.src1.c_str());
    symTableEntry funSte = symbolTable.searchInGlobalTable(stackManager.functionStack.top());
    if (funSte.oType == FAULT) {
        ERROR(54);
    }
    int paraOffset = (funSte.parameter) * 4;
    //无返回值
    if (quad.src1 == "") {
        //恢复sp
        sprintf(MIPSTextCode[MIPSTextLine++], "addi $sp, $s0, %d\n", paraOffset);
        sprintf(MIPSTextCode[MIPSTextLine++], "jr $ra\n");
    } else {
        //返回 v0
        getSrcToMIPS(quad.src1, 1);
        sprintf(MIPSTextCode[MIPSTextLine++], "add $v0, $0, $s1\n");
        //恢复sp
        sprintf(MIPSTextCode[MIPSTextLine++], "addi $sp, $s0, %d\n", paraOffset);
        sprintf(MIPSTextCode[MIPSTextLine++], "jr $ra\n");
    }
    sprintf(MIPSTextCode[MIPSTextLine++], "#end return: %s\n", quad.src1.c_str());
}

void MIPSGenerator::PUSHToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#push: %s\n", quad.src1.c_str());
    getSrcToMIPS(quad.src1, 1);
    sprintf(MIPSTextCode[MIPSTextLine++], "sw $s1, ($sp)\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "addiu $sp, $sp, -4\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "#end push: %s\n", quad.src1.c_str());
}

void MIPSGenerator::RIToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#read int:  %s\n", quad.dst.c_str());
    sprintf(MIPSTextCode[MIPSTextLine++], "add $v0, $0, $0\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "ori $v0, $v0, 5\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "syscall\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "add $s3, $0, $v0\n");
    writeDst(quad.dst);
    sprintf(MIPSTextCode[MIPSTextLine++], "#end read int:  %s\n", quad.dst.c_str());
}

void MIPSGenerator::RCToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#read char:  %s\n", quad.dst.c_str());
    sprintf(MIPSTextCode[MIPSTextLine++], "add $v0, $0, $0\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "ori $v0, $v0, 12\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "syscall\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "add $s3, $0, $v0\n");
    writeDst(quad.dst);
    sprintf(MIPSTextCode[MIPSTextLine++], "#end read char:  %s\n", quad.dst.c_str());
}

void MIPSGenerator::WSToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#write string:  %s\n", quad.src1.c_str());
    static string str = "str";
    static int a = 0;
    string strLabel = str;
    strLabel.append(to_string(a));
    string outStr = quad.src1;
    sprintf(MIPSDataCode[MIPSDataLine++], "%s: .asciiz %s\n", strLabel.c_str(), outStr.c_str());
    sprintf(MIPSTextCode[MIPSTextLine++], "add $v0, $0, $0\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "ori $v0, $v0, 4\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "la $a0, %s\n", strLabel.c_str());
    sprintf(MIPSTextCode[MIPSTextLine++], "syscall\n");
    a++;
    sprintf(MIPSTextCode[MIPSTextLine++], "#end write string:  %s\n", quad.src1.c_str());
}

void MIPSGenerator::WIToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#write int:  %s\n", quad.src1.c_str());
    sprintf(MIPSTextCode[MIPSTextLine++], "add $v0, $0, $0\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "ori $v0, $v0, 1\n");
    getSrcToMIPS(quad.src1, 1);
    sprintf(MIPSTextCode[MIPSTextLine++], "add $a0, $0, $s1\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "syscall\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "#end write int:  %s\n", quad.src1.c_str());
}

void MIPSGenerator::WCToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#write char\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "add $v0, $0, $0\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "ori $v0, $v0, 11\n");
    getSrcToMIPS(quad.src1, 1);
    sprintf(MIPSTextCode[MIPSTextLine++], "add $a0, $0, $s1\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "syscall\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "#end write char\n");
}

void MIPSGenerator::VAR_DECLAREToMIPS(Quad quad) {
    if (quad.global) {
        symTableEntry ste = symbolTable.searchInGlobalTable(quad.dst);
        if (ste.oType == FAULT) {
            ERROR(54);
        }
        sprintf(MIPSDataCode[MIPSDataLine++], "%s: .space %d\n", quad.dst.c_str(), atoi(quad.src2.c_str()) * 4);
    } else {
        symTableEntry ste = symbolTable.searchInLocalTable(quad.dst, stackManager.functionStack.top());
        if (ste.oType == FAULT) {
            ERROR(54);
        }
        for (int i = 0; i < atoi(quad.src2.c_str()); ++i) {
            sprintf(MIPSTextCode[MIPSTextLine++], "sw $0, ($sp)\n");
            sprintf(MIPSTextCode[MIPSTextLine++], "addiu $sp, $sp, -4\n");
        }

    }
}

void MIPSGenerator::CONST_DECLAREToMIPS(Quad quad) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#const define:  %s\n", quad.dst.c_str());
    if (quad.global) {
        sprintf(MIPSDataCode[MIPSDataLine++], "%s: .space 4\n", quad.dst.c_str());
        getSrcToMIPS(quad.src2, 1);
        sprintf(MIPSTextCode[MIPSTextLine++], "add $t0, $0, $s1\n");
        sprintf(MIPSTextCode[MIPSTextLine++], "sw $t0, %s\n", quad.dst.c_str());
    } else {
        getSrcToMIPS(quad.src2, 1);
        sprintf(MIPSTextCode[MIPSTextLine++], "sw $s%d, ($sp)\n", 1);
        sprintf(MIPSTextCode[MIPSTextLine++], "addiu $sp, $sp, -4\n");
    }
    sprintf(MIPSTextCode[MIPSTextLine++], "#end const define:  %s\n", quad.dst.c_str());
}

void MIPSGenerator::MAIN_FUNCTION_DEFINEToMIPS(Quad quad) {
    //贴标签
    sprintf(MIPSTextCode[MIPSTextLine++], "\n%s:\n", quad.dst.c_str());
    //初始化
    sprintf(MIPSTextCode[MIPSTextLine++], "add $s0, $0, $sp\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "la $ra, end\n");
    //存base
    sprintf(MIPSTextCode[MIPSTextLine++], "sw $s0, ($sp)\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "addiu $sp, $sp, -4\n");
    //存ra
    sprintf(MIPSTextCode[MIPSTextLine++], "sw $ra, ($sp)\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "addiu $sp, $sp, -4\n");
    stackManager.functionStack.push(quad.dst);
}

void MIPSGenerator::FUNCTION_DEFINEToMIPS(Quad quad) {
    stackManager.functionStack.pop();
    sprintf(MIPSTextCode[MIPSTextLine++], "\nj main\n");
    //贴标签
    sprintf(MIPSTextCode[MIPSTextLine++], "\n%s:\n", quad.dst.c_str());
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

void MIPSGenerator::getAddrtoMIPS(string src, int getSrc, bool &global) {
    symTableEntry ste = symbolTable.searchInLocalTable(src, stackManager.functionStack.top());
    symTableEntry gloSte = symbolTable.searchInGlobalTable(src);
    if (ste.oType != FAULT) {
        int MIPSOffset = (ste.offset + 2) * 4;
        sprintf(MIPSTextCode[MIPSTextLine++], "subi $s%d, $s0, %d\n", getSrc, MIPSOffset);
    } else if (gloSte.oType != FAULT) {
        global = true;
        sprintf(MIPSTextCode[MIPSTextLine++], "addi $t0, $0, 0x10010000\n");
        sprintf(MIPSTextCode[MIPSTextLine++], "addi $s%d, $t0, %d\n", getSrc, gloSte.offset * 4);
    } else {
        ERROR(54);
    }
}

void MIPSGenerator::getSrcToMIPS(string src, int quadSrc) {
    if (src != "'\n'") {
        sprintf(MIPSTextCode[MIPSTextLine++], "#load %s to s%d\n", src.c_str(), quadSrc);
    } else {
        sprintf(MIPSTextCode[MIPSTextLine++], "#enter\n");
    }

    if (src[0] == '\'' || src[0] == '"' || isdigit(src[0]) || src[0] == '-' || src[0] == '+') {
        getConst(src, quadSrc);
        return;
    }
    symTableEntry ste = symbolTable.searchInLocalTable(src, stackManager.functionStack.top());
    symTableEntry gloSte = symbolTable.searchInGlobalTable(src);
    int MIPSOffset = (ste.offset + 2) * 4;
    if (ste.oType != FAULT) {
        if (ste.parameter == 0)
            getLocal(MIPSOffset, quadSrc);
        else {
            symTableEntry funSte = symbolTable.searchInGlobalTable(stackManager.functionStack.top());
            if (funSte.oType == FAULT) {
                ERROR(54);
            }
            getParameter(funSte.parameter, ste.parameter, quadSrc);
        }
    } else if (gloSte.oType != FAULT) {
        getGlobal(gloSte.offset * 4, quadSrc);
    } else {
        sprintf(MIPSTextCode[MIPSTextLine++], "addiu $sp, $sp, 4\n");
        sprintf(MIPSTextCode[MIPSTextLine++], "lw $s%d, ($sp)\n", quadSrc);
    }

    if (src != "'\n'") {
        sprintf(MIPSTextCode[MIPSTextLine++], "#load %s to s%d\n", src.c_str(), quadSrc);
    } else {
        sprintf(MIPSTextCode[MIPSTextLine++], "#end enter\n");
    }
}


void MIPSGenerator::writeDst(string dst) {
    sprintf(MIPSTextCode[MIPSTextLine++], "#write s3 to %s\n", dst.c_str());
    symTableEntry ste = symbolTable.searchInLocalTable(dst, stackManager.functionStack.top());
    symTableEntry gloSte = symbolTable.searchInGlobalTable(dst);
    int MIPSOffset = (ste.offset + 2) * 4;
    if (ste.oType != FAULT) {
        if (ste.parameter == 0)
            writeLocal(MIPSOffset);
        else {
            symTableEntry funSte = symbolTable.searchInGlobalTable(stackManager.functionStack.top());
            if (funSte.oType == FAULT) {
                ERROR(54);
            }
            writeParameter(funSte.parameter, ste.parameter);
        }
    } else if (gloSte.oType != FAULT) {
        writeGlobal(gloSte.offset * 4);

    } else {
        sprintf(MIPSTextCode[MIPSTextLine++], "sw $s3, ($sp)\n");
        sprintf(MIPSTextCode[MIPSTextLine++], "addiu $sp, $sp, -4\n");
    }
    sprintf(MIPSTextCode[MIPSTextLine++], "#end write s3\n");
}

void MIPSGenerator::getLocal(int offset, int quadSrc) {
    sprintf(MIPSTextCode[MIPSTextLine++], "add $t0, $0, $0\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "ori $t0, $t0, %d\n", offset);
    sprintf(MIPSTextCode[MIPSTextLine++], "sub $t0, $s0, $t0\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "lw $s%d, ($t0)\n", quadSrc);
}

void MIPSGenerator::getParameter(int paraNum, int paraOrder, int quadSrc) {
    int addr = (paraNum + 1 - paraOrder) * 4;
    sprintf(MIPSTextCode[MIPSTextLine++], "add $t0, $0, $0\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "ori $t0, $t0, %d\n", addr);
    sprintf(MIPSTextCode[MIPSTextLine++], "add $t2, $s0, $t0\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "lw $s%d, ($t2)\n", quadSrc);
}

void MIPSGenerator::getGlobal(int offset, int quadSrc) {
    sprintf(MIPSTextCode[MIPSTextLine++], "addi $t0, $0, 0x10010000\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "addi $t0, $t0, %d\n", offset);
    sprintf(MIPSTextCode[MIPSTextLine++], "lw $s%d, ($t0)\n", quadSrc);
}

void MIPSGenerator::getConst(string src, int quadSrc) {
    if (src[0] == '\'') {
        sprintf(MIPSTextCode[MIPSTextLine++], "add $s%d, $0, $0\n", quadSrc);
        sprintf(MIPSTextCode[MIPSTextLine++], "ori $s%d, $s%d, %d\n", quadSrc, quadSrc, (int) src[1]);
    } else if (src[0] == '"') {
        sprintf(MIPSTextCode[MIPSTextLine++], "add $s%d, $0, $0\n", quadSrc);
        sprintf(MIPSTextCode[MIPSTextLine++], "ori $s%d, $s%d, %d\n", quadSrc, quadSrc, (int) '\n');
    } else {
        sprintf(MIPSTextCode[MIPSTextLine++], "add $s%d, $0, $0\n", quadSrc);
        sprintf(MIPSTextCode[MIPSTextLine++], "ori $s%d, $s%d, %d\n", quadSrc, quadSrc, atoi(src.c_str()));
    }
}

void MIPSGenerator::getString(string src, int quadSrc) {
    static string str = "str";
    static int strCnt = 0;
    if (src[0] == '\"') {
        sprintf(MIPSDataCode[MIPSDataLine++], "%s: .asciiz \"%s\"\n", str.append(to_string(strCnt++)).c_str(),
                src.c_str());
        sprintf(MIPSTextCode[MIPSTextLine++], "la $s%d, %s\n", quadSrc, src.c_str());

    }
}

void MIPSGenerator::writeLocal(int offset) {
    sprintf(MIPSTextCode[MIPSTextLine++], "add $t0, $0, $0\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "ori $t0, $t0, %d\n", offset);
    sprintf(MIPSTextCode[MIPSTextLine++], "sub $t0, $s0, $t0\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "sw $s3, ($t0)\n");
}

void MIPSGenerator::writeParameter(int paraNum, int paraOrder) {
    int addr = (paraNum + 1 - paraOrder) * 4;
    sprintf(MIPSTextCode[MIPSTextLine++], "add $t0, $0, $0\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "ori $t0, $t0, %d\n", addr);
    sprintf(MIPSTextCode[MIPSTextLine++], "add $t2, $s0, $t0\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "sw $s3, ($t2)\n");
}

void MIPSGenerator::writeGlobal(int offset) {
    sprintf(MIPSTextCode[MIPSTextLine++], "addi $t0, $0, 0x10010000\n");
    sprintf(MIPSTextCode[MIPSTextLine++], "addi $t0, $t0, %d\n", offset);
    sprintf(MIPSTextCode[MIPSTextLine++], "sw $s3, ($t0)\n");
}

MIPSGenerator::MIPSGenerator(Quadruple &theQuadruple, SymbolTable &theSymbolTable, StackManager &theStackManager,
                             ExceptionHandler &theExceptionHandler,
                             FILE *theMIPSFile)
        : quadruple(theQuadruple),
          symbolTable(theSymbolTable),
          stackManager(theStackManager),
          exceptionHandler(theExceptionHandler) {
    MIPSFile = theMIPSFile;
    MIPSTextLine = 0;
}

