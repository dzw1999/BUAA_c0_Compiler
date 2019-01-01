//
// Created by andyson on 18-11-20.
//

#include "GrammarAnalyzer.h"
#include "ExceptionHandler.h"
#include "SymbolTable.h"
#include "SemanticAnalyzer.h"

#include <cstdio>
#include <cstring>

#define GRAMMAR_OUTPUT

#define SYM lexicalAnalyzer.symbolList[symbolCnt]

#define SYM_TYPE lexicalAnalyzer.symbolTypeList[symbolCnt]

#define SYM_LINE lexicalAnalyzer.symbolLineList[symbolCnt]

#define GET_SYM \
    if(symbolCnt == lexicalAnalyzer.symbolNum - 1){ \
        printf("incomplete"); \
        throw "incomplete program";    \
    } \
    symbolCnt++

#define ERROR(x) \
    exceptionHandler.errorLine = SYM_LINE; \
    throw x

static string fun = "func_";

void GrammarAnalyzer::printmsg(string format, int lineCnt) {
#ifdef GRAMMAR_OUTPUT
    fprintf(fout, format.c_str(), lineCnt);
#endif
}

GrammarAnalyzer::GrammarAnalyzer(
        LexicalAnalyzer &theLexicalAnalyzer,
        ExceptionHandler &theExceptionHandler,
        SymbolTable &theSymbolTable,
        SemanticAnalyzer &theSemanticAnalyzer,
        FILE *out)
        : lexicalAnalyzer(theLexicalAnalyzer), exceptionHandler(theExceptionHandler), symbolTable(theSymbolTable),
          semanticAnalyzer(theSemanticAnalyzer) {
    symbolCnt = -1;
    fout = out;
    currentFunction = "";
}

int GrammarAnalyzer::grammarAnalyze() {
    try {
        GET_SYM;
        program();
    } catch (int errorCode) {
        exceptionHandler.error(errorCode, SYM_LINE);
        return 1;
    }
    if (SYM_TYPE == FINISH) {
        return 0;
    }
    return 1;
}

void GrammarAnalyzer::program() {
    // [常量说明]
    if (SYM_TYPE == CONST)
        constDeclare();


    // [变量说明]
    // 使用try catch避免误识别函数定义
    saveSymbolCnt = symbolCnt;
    try {
        if (SYM_TYPE == INT || SYM_TYPE == CHAR)
            varDeclare();
    } catch (int errorCode) {
        if (errorCode == 34)
            symbolCnt = saveSymbolCnt;
    }

    // [函数定义]
    saveSymbolCnt = symbolCnt;

    while (SYM_TYPE == VOID || SYM_TYPE == INT || SYM_TYPE == CHAR) {
        try {
            if (SYM_TYPE == VOID) {
                voidFunctionDefine();
            } else {
                nonVoidFunctionDefine();
            }
            saveSymbolCnt = symbolCnt;
        }
        catch (int errorCode) {
            if (errorCode == 35) {
                symbolCnt = saveSymbolCnt;
                currentFunction = "";
                break;
            } else {
                exceptionHandler.error(errorCode, SYM_LINE);
                while (SYM_TYPE != CONST && SYM_TYPE != INT && SYM_TYPE != CHAR && SYM_TYPE != VOID &&
                       SYM_TYPE != FINISH && SYM_TYPE != RIGHT_BRACE) {
                    GET_SYM;
                }
            }
        }
    }


    // 主函数
    try {
        mainFunction();
    } catch (int errorCode) {
        exceptionHandler.error(errorCode, SYM_LINE);
    }


}

void GrammarAnalyzer::constDeclare() {
    do {
        try {
            if (SYM_TYPE != CONST) {
                ERROR(1);
            }
            GET_SYM;
            constDefine();
            if (SYM_TYPE != SEMICOLON) {
                ERROR(2);
            }
            GET_SYM;
            printmsg("line %d, This is a const declaration.\n", SYM_LINE);
        } catch (int errorCode) {
            exceptionHandler.error(errorCode, SYM_LINE);
            while (SYM_TYPE != CONST && SYM_TYPE != INT && SYM_TYPE != CHAR && SYM_TYPE != VOID && SYM_TYPE != FINISH &&
                   SYM_TYPE != RIGHT_BRACE) {
                GET_SYM;
            }
        }
    } while (SYM_TYPE == CONST);
}

void GrammarAnalyzer::varDeclare() {
    do {
        try {
            varDefine();
            if (SYM_TYPE != SEMICOLON) {
                if (SYM_TYPE == LEFT_BRACKET) {
                    ERROR(34);
                } else {
                    ERROR(2);
                }
            }
            GET_SYM;
            printmsg("line %d, This is a var declaration.\n", SYM_LINE);
            saveSymbolCnt = symbolCnt;
        } catch (int errorCode) {
            if (errorCode == 34)
                throw errorCode;
            else {
                exceptionHandler.error(errorCode, SYM_LINE);
                while (SYM_TYPE != INT && SYM_TYPE != CHAR && SYM_TYPE != VOID && SYM_TYPE != FINISH &&
                       SYM_TYPE != RIGHT_BRACE) {
                    GET_SYM;
                }
            }
        }
    } while (SYM_TYPE == INT || SYM_TYPE == CHAR);
}

void GrammarAnalyzer::nonVoidFunctionDefine() {
    string lastFunction = currentFunction;
    string thisFunction;
    valueType vType;
    declareHead(thisFunction, &vType);
    if (SYM_TYPE != LEFT_BRACKET) {
        ERROR(3);
    }

    //查全局表
    if (symbolTable.searchInGlobalTable(thisFunction).oType != FAULT) {
        ERROR(38);
    };
    //建函数符号表
    if (!symbolTable.createLocalSymTable(thisFunction)) {
        ERROR(38);
    }
    //更新当前函数
    currentFunction = thisFunction;

    semanticAnalyzer.functionDefine(VOID_TYPE, thisFunction);
    GET_SYM;
    int parameter = parameterList();
    //入表
    int length = 0;
    symTableEntry ste = symbolTable.createSte(FUNCTION, vType, length, parameter);
    if (!symbolTable.addToTable(thisFunction, ste, lastFunction)) {
        ERROR(39);
    }
    //入新表
    if (!symbolTable.addToTable(thisFunction, ste, thisFunction)) {
        ERROR(39);
    }
    if (SYM_TYPE != RIGHT_BRACKET) {
        ERROR(4);
    }
    GET_SYM;
    if (SYM_TYPE != LEFT_BRACE) {
        ERROR(5);
    }
    GET_SYM;
    compoundStatement();
    if (SYM_TYPE != RIGHT_BRACE) {
        ERROR(6);
    }
    semanticAnalyzer.functionEnd();
    printmsg("line %d, This is a definition of a function with returned value.\n", SYM_LINE);
    //恢复函数域
    currentFunction = lastFunction;
    GET_SYM;
}

void GrammarAnalyzer::voidFunctionDefine() {
    if (SYM_TYPE != VOID) {
        ERROR(7);
    }
    GET_SYM;
    if (SYM_TYPE != IDENTIFIER) {
        if (SYM_TYPE == MAIN) {
            ERROR(35);
        } else
        ERROR(8);
    }
    string thisFunction = SYM;
    string lastFunction = currentFunction;
    //查全局表
    if (symbolTable.searchInGlobalTable(thisFunction).oType != FAULT) {
        ERROR(38);
    };
    //建函数符号表
    if (!symbolTable.createLocalSymTable(thisFunction)) {
        ERROR(38);
    }
    //更新当前函数
    currentFunction = thisFunction;

    semanticAnalyzer.functionDefine(VOID_TYPE, thisFunction);

    GET_SYM;
    if (SYM_TYPE != LEFT_BRACKET) {
        ERROR(3);
    }
    GET_SYM;
    int parameter = parameterList();
    //入表
    int length = 0;
    symTableEntry ste = symbolTable.createSte(FUNCTION, VOID_TYPE, length, parameter);
    if (!symbolTable.addToTable(thisFunction, ste, lastFunction)) {
        ERROR(39);
    }
    //入新表
    if (!symbolTable.addToTable(thisFunction, ste, thisFunction)) {
        ERROR(39);
    }

    if (SYM_TYPE != RIGHT_BRACKET) {
        ERROR(4);
    }
    GET_SYM;
    if (SYM_TYPE != LEFT_BRACE) {
        ERROR(5);
    }
    GET_SYM;
    compoundStatement();
    if (SYM_TYPE != RIGHT_BRACE) {
        ERROR(6);
    }
    semanticAnalyzer.functionEnd();
    printmsg("line %d, This is a void function definition.\n", SYM_LINE);
    //恢复函数域
    currentFunction = lastFunction;
    GET_SYM;
}

void GrammarAnalyzer::mainFunction() {
    if (SYM_TYPE != VOID) {
        ERROR(7);
    }
    GET_SYM;
    if (SYM_TYPE != MAIN) {
        ERROR(9);
    }
    string thisFunction = SYM;
    string lastFunction = currentFunction;
    //查全局表
    if (symbolTable.searchInGlobalTable(thisFunction).oType != FAULT) {
        ERROR(38);
    };
    //建函数符号表
    if (!symbolTable.createLocalSymTable(thisFunction)) {
        ERROR(38);
    }
    //入表
    symTableEntry ste = symbolTable.createSte(FUNCTION, VOID_TYPE);
    if (!symbolTable.addToTable(thisFunction, ste, currentFunction)) {
        ERROR(38);
    }
    //入新表
    if (!symbolTable.addToTable(thisFunction, ste, thisFunction)) {
        ERROR(39);
    }
    //更新当前函数
    currentFunction = thisFunction;

    semanticAnalyzer.mainFunctionDefine(VOID_TYPE, thisFunction);

    GET_SYM;
    if (SYM_TYPE != LEFT_BRACKET) {
        ERROR(3);
    }
    GET_SYM;
    if (SYM_TYPE != RIGHT_BRACKET) {
        ERROR(4);
    }
    GET_SYM;
    if (SYM_TYPE != LEFT_BRACE) {
        ERROR(5);
    }
    GET_SYM;
    compoundStatement();
    if (SYM_TYPE != RIGHT_BRACE) {
        ERROR(6);
    }
    semanticAnalyzer.functionEnd();
    printmsg("line %d, This is the main function definition.\n", SYM_LINE);
    //恢复函数域
    currentFunction = lastFunction;
    GET_SYM;
}

void GrammarAnalyzer::constDefine() {
    if (SYM_TYPE != INT && SYM_TYPE != CHAR) {
        ERROR(10);
    }
    symbolType type = SYM_TYPE;
    do {
        string value;
        int constValue;
        GET_SYM;
        if (SYM_TYPE != IDENTIFIER) {
            ERROR(8);
        }
        string ident = SYM;
        GET_SYM;
        if (SYM_TYPE != ASSIGN) {
            ERROR(11);
        }
        GET_SYM;
        if (type == INT) {
            constValue = integer();
            value = to_string(constValue);
        } else {
            if (SYM_TYPE != CHARATER) {
                ERROR(12);
            }
            constValue = SYM[1];
            value = "'" + string(1, constValue) + "'";
            GET_SYM;
        }
        printmsg("line %d, This is a const definition.\n", SYM_LINE);
        //查局部表, 如果已经在表中,报错
        if (symbolTable.searchInLocalTable(ident, currentFunction).oType != FAULT) {
            ERROR(38);
        };
        //入表
        symTableEntry ste = symbolTable.createSte(CONSTANT, type == INT ? INT_TYPE : CHAR_TYPE, 0, 0, constValue);
        if (!symbolTable.addToTable(ident, ste, currentFunction)) {
            ERROR(39);
        }
    } while (SYM_TYPE == COMMA);
}

void GrammarAnalyzer::declareHead(string &functionName, valueType *vType) {
    if (SYM_TYPE != INT && SYM_TYPE != CHAR) {
        ERROR(10);
    }
    symbolType type = SYM_TYPE;
    GET_SYM;
    if (SYM_TYPE != IDENTIFIER) {
        ERROR(8);
    }
    //传函数名
    functionName = SYM;
    //传返回值类型
    *vType = type == INT ? INT_TYPE : CHAR_TYPE;
    printmsg("line %d, This is a declaration head.\n", SYM_LINE);
    GET_SYM;
}

void GrammarAnalyzer::varDefine() {
    if (SYM_TYPE != INT && SYM_TYPE != CHAR) {
        ERROR(10);
    }
    symbolType type = SYM_TYPE;
    do {
        int length = 0;
        GET_SYM;
        if (SYM_TYPE != IDENTIFIER) {
            ERROR(8);
        }
        string ident = SYM;
        GET_SYM;
        if (SYM_TYPE == LEFT_SQUARE) {
            GET_SYM;
            if (SYM_TYPE != UNSIGN_NUMBER) {
                ERROR(15);
            }
            length = atoi(SYM.c_str());
            GET_SYM;
            if (SYM_TYPE != RIGHT_SQUARE) {
                ERROR(14);
            }
            GET_SYM;
        }
        //误判
        if (SYM_TYPE == LEFT_BRACKET) {
            ERROR(34);
        }
        printmsg("line %d, This is a var definition.\n", SYM_LINE);
        //查局部表, 如果已经在表中,报错
        if (symbolTable.searchInLocalTable(ident, currentFunction).oType != FAULT) {
            ERROR(38);
        };
        //入表
        symTableEntry ste = symbolTable.createSte(
                VARIABLE,
                type == INT ? (length == 0 ? INT_TYPE : INT_ARRAY_TYPE) : (length == 0 ? CHAR_TYPE : CHAR_ARRAY_TYPE),
                length);
        if (!symbolTable.addToTable(ident, ste, currentFunction)) {
            ERROR(39);
        }
        bool global = currentFunction == "";
        semanticAnalyzer.varDefine(type, ident, ste.length == 0 ? 1 : ste.length, global);
    } while (SYM_TYPE == COMMA);
}

void GrammarAnalyzer::compoundStatement() {
    // [常量说明]
    if (SYM_TYPE == CONST)
        constDeclare();

    // [变量说明]
    try {
        if (SYM_TYPE == INT || SYM_TYPE == CHAR)
            varDeclare();
    } catch (int errorCode) {
        exceptionHandler.error(errorCode, SYM_LINE);
        return;
    }
    statementList();
    printmsg("line %d, These are compound Statements.\n", SYM_LINE);
}

int GrammarAnalyzer::parameterList() {
    int parameterCnt = 0;
    if (SYM_TYPE == INT || SYM_TYPE == CHAR) {
        parameter(++parameterCnt);
        while (SYM_TYPE == COMMA) {
            GET_SYM;
            parameter(++parameterCnt);
        }
        printmsg("line %d, This is a parameter list.\n", SYM_LINE);
    } else {
        printmsg("line %d, This is an empty parameter list.\n", SYM_LINE);
    }
    return parameterCnt;
}

void GrammarAnalyzer::parameter(int cnt) {
    if (SYM_TYPE != INT && SYM_TYPE != CHAR) {
        ERROR(10);
    }
    symbolType type = SYM_TYPE;
    GET_SYM;
    if (SYM_TYPE != IDENTIFIER) {
        ERROR(8);
    }
    string ident = SYM;
    printmsg("line %d, This is a parameter.\n", SYM_LINE);
    //查局部表, 如果已经在表中,报错
    if (symbolTable.searchInLocalTable(ident, currentFunction).oType != FAULT) {
        ERROR(38);
    };
    //入表
    int length = 0;
    symTableEntry ste = symbolTable.createSte(PARAMETER, type == INT ? INT_TYPE : CHAR_TYPE, length, cnt);
    if (!symbolTable.addToTable(SYM, ste, currentFunction)) {
        ERROR(39);
    }
    semanticAnalyzer.parameter(type, ident);
    GET_SYM;
}

void GrammarAnalyzer::expression(string &rtn_expr, valueType &type, bool &variable, int &staticValue, bool isSwitch) {
    int numSymbol = 0;
    string src1, src2, dst;
    valueType src2Type;
    bool src2Variable;
    char op;
    if (SYM_TYPE == PLUS) {
        numSymbol = 1;
        GET_SYM;
    } else if (SYM_TYPE == MINUS) {
        numSymbol = -1;
        GET_SYM;
    }
    term(dst, type, variable, staticValue);
    printmsg("line %d, This is an expression.\n", SYM_LINE);
    if (numSymbol != 0) {
        src1 = dst;
        type = INT_TYPE;
        staticValue = numSymbol * staticValue;
        if(numSymbol == -1)
            semanticAnalyzer.sub("0", src1, dst);
    }
    while (SYM_TYPE == PLUS || SYM_TYPE == MINUS) {
        if (SYM_TYPE == PLUS) {
            op = '+';
        } else {
            op = '-';
        }
        GET_SYM;
        variable = false;
        src1 = dst;
        term(src2, src2Type, src2Variable, staticValue);
        type = INT_TYPE;
        staticValue = 0;
        if (op == '+') {
            semanticAnalyzer.add(src1, src2, dst);
        } else {
            semanticAnalyzer.sub(src1, src2, dst);
        }
        printmsg("line %d, This is an expression.\n", SYM_LINE);
    }
    if (isSwitch)
        semanticAnalyzer.switchVar(dst);
    rtn_expr = dst;
}

void GrammarAnalyzer::term(string &rtn_term, valueType &type, bool &variable, int &staticValue) {
    string src1, src2, dst;
    valueType src2Type;
    bool src2Variable;
    char op;
    factor(dst, type, variable, staticValue);
    printmsg("line %d, This is a term.\n", SYM_LINE);
    while (SYM_TYPE == ASTERISK || SYM_TYPE == SLASH) {
        if (SYM_TYPE == ASTERISK) {
            op = '*';
        } else {
            op = '/';
        }
        GET_SYM;
        variable = false;
        src1 = dst;
        factor(src2, src2Type, src2Variable, staticValue);
        type = INT_TYPE;
        staticValue = 0;
        if (op == '*') {
            semanticAnalyzer.mult(src1, src2, dst);
        } else {
            semanticAnalyzer.div(src1, src2, dst);
        }
        printmsg("line %d, This is a term.\n", SYM_LINE);
    }
    rtn_term = dst;
}

void GrammarAnalyzer::factor(string &rtn_factor, valueType &type, bool &variable, int &staticValue) {
    string index;
    valueType exprType;
    bool exprVariable;
    staticValue = 0;
    int exprStaticValue;
    if (SYM_TYPE == IDENTIFIER) {
        string ident = SYM;
        //查表
        symTableEntry ste = symbolTable.searchInTable(SYM, currentFunction);
        if (ste.oType == FAULT) {
            ERROR(41);
        }
        GET_SYM;
        if (SYM_TYPE == LEFT_SQUARE) {  //数组
            // 数组类型检验
            if (ste.vType != INT_ARRAY_TYPE && ste.vType != CHAR_ARRAY_TYPE) {
                ERROR(42);
            }

            GET_SYM;
            expression(index, exprType, exprVariable, exprStaticValue);
            if(exprType != INT_TYPE && exprType != INT_ARRAY_TYPE){
                ERROR(15);
            }
            if (exprStaticValue != 0 && (exprStaticValue >= ste.length || exprStaticValue < 0)){
                ERROR(60);
            }
            if (SYM_TYPE != RIGHT_SQUARE) {
                ERROR(14);
            }
            GET_SYM;
            printmsg("line %d, This is a factor.\n", SYM_LINE);
            semanticAnalyzer.getFromArray(ident, index, rtn_factor);
            type = ste.vType;
            variable = ste.oType == VARIABLE;
        } else if (SYM_TYPE == LEFT_BRACKET) {  //函数调用
            symbolCnt--;
            functionCall(rtn_factor, type);
            // 类型检验 返回值
            if (ste.vType == VOID_TYPE) {
                ERROR(45);
            }
            printmsg("line %d, This is a factor.\n", SYM_LINE);
            variable = false;
        } else {    //标识符
            rtn_factor = ident;
            // 类型检验
            if (ste.oType == FUNCTION) {
                ERROR(45);
            }
            if (ste.vType == INT_ARRAY_TYPE || ste.vType == CHAR_ARRAY_TYPE) {
                ERROR(49);
            }
            if(ste.oType == CONSTANT){
                staticValue = ste.constValue;
                rtn_factor = to_string(ste.constValue);
            }
            type = ste.vType;
            variable = ste.oType == VARIABLE;
            printmsg("line %d, This is a factor.\n", SYM_LINE);
        }
    } else if (SYM_TYPE == LEFT_BRACKET) {
        GET_SYM;
        expression(rtn_factor, type, variable, staticValue);
        type = INT_TYPE;
        if (SYM_TYPE == RIGHT_BRACKET) {
            GET_SYM;
            printmsg("line %d, This is a factor.\n", SYM_LINE);
            return;
        } else {
            ERROR(4);
        }
    } else if (SYM_TYPE == CHARATER) {
        string s(1, SYM[1]);
        rtn_factor = "'" + s + "'";
        type = CHAR_TYPE;
        variable = false;
        staticValue = SYM[1];
        GET_SYM;
        printmsg("line %d, This is a factor.\n", SYM_LINE);
    } else if (SYM_TYPE == PLUS || SYM_TYPE == MINUS || SYM_TYPE == UNSIGN_NUMBER) {
        bool nag = SYM_TYPE == MINUS;
        rtn_factor = to_string(integer());
        type = INT_TYPE;
        variable = false;
        staticValue = nag ? -1 * atoi(rtn_factor.c_str()) : atoi(rtn_factor.c_str());
        printmsg("line %d, This is a factor.\n", SYM_LINE);
    } else {
        ERROR(16);
    }
}

void GrammarAnalyzer::statement() {
    string value;
    valueType type;
    bool variable;
    string rtn_func;
    valueType rtn_func_Type;
    if (SYM_TYPE == IF) {
        ifStatement();
        printmsg("line %d, This is a statement.\n", SYM_LINE);
    } else if (SYM_TYPE == WHILE) {
        whileStatement();
        printmsg("line %d, This is a statement.\n", SYM_LINE);
    } else if (SYM_TYPE == LEFT_BRACE) {
        GET_SYM;
        statementList();
        if (SYM_TYPE == RIGHT_BRACE) {
            GET_SYM;
            printmsg("line %d, This is a statement.\n", SYM_LINE);
            return;
        } else {
            ERROR(6);
        }
    } else if (SYM_TYPE == IDENTIFIER) {
        //查表
        symTableEntry ste = symbolTable.searchInTable(SYM, currentFunction);
        if (ste.oType == FAULT) {
            ERROR(41);
        }
        GET_SYM;
        if (SYM_TYPE == ASSIGN || SYM_TYPE == LEFT_SQUARE) {
            symbolCnt--;
            assignStatement();
            if (SYM_TYPE != SEMICOLON) {
                ERROR(2);
            }
            printmsg("line %d, This is a statement.\n", SYM_LINE);
            GET_SYM;
        } else if (SYM_TYPE == LEFT_BRACKET) {
            symbolCnt--;
            functionCall(rtn_func, rtn_func_Type);
            if (SYM_TYPE != SEMICOLON) {
                ERROR(2);
            }
            GET_SYM;
            printmsg("line %d, This is a statement.\n", SYM_LINE);
        } else {
            ERROR(17);
        }
    } else if (SYM_TYPE == SWITCH) {
        switchStatement();
        printmsg("line %d, This is a statement.\n", SYM_LINE);
    } else if (SYM_TYPE == RETURN) {
        returnStatement();
        if (SYM_TYPE != SEMICOLON) {
            ERROR(2);
        }
        GET_SYM;
        printmsg("line %d, This is a statement.\n", SYM_LINE);
    } else if (SYM_TYPE == SCANF) {
        scanfStatement();

        if (SYM_TYPE != SEMICOLON) {
            ERROR(2);
        }
        GET_SYM;
        printmsg("line %d, This is a statement.\n", SYM_LINE);
    } else if (SYM_TYPE == PRINTF) {
        printfStatement();

        if (SYM_TYPE != SEMICOLON) {
            ERROR(2);
        }
        GET_SYM;
        printmsg("line %d, This is a statement.\n", SYM_LINE);
    } else if (SYM_TYPE == SEMICOLON) {
        printmsg("line %d, This is a empty statement.\n", SYM_LINE);
        GET_SYM;
    } else {
        ERROR(18);
    }
}

void GrammarAnalyzer::assignStatement() {
    string ident;        // 标识符
    string offset;       // 如果是数组，记录下标
    string value;        // 要赋的值
    valueType identType; // 被赋值的变量的类型
    valueType exprType;  // 要赋的值的类型
    valueType offsetType;
    bool variable;       // 表达式是否为变量表达式
    bool offsetVariable;
    int staticValue;
    if (SYM_TYPE != IDENTIFIER) {
        ERROR(8);
    }
    ident = SYM;
    //查表
    symTableEntry ste = symbolTable.searchInTable(ident, currentFunction);
    if (ste.oType == FAULT) {
        ERROR(41);
    }
    GET_SYM;
    if (SYM_TYPE == ASSIGN) {
        //赋值变量检验
        if (ste.oType != VARIABLE && ste.oType != PARAMETER) {
            ERROR(46);
        }
        identType = ste.vType;
        if(identType != INT_TYPE && identType != CHAR_TYPE){
            ERROR(57);
        }
        GET_SYM;
        expression(value, exprType, variable, staticValue);
        //类型检验
        if (identType == CHAR_TYPE && exprType != CHAR_ARRAY_TYPE && exprType != CHAR_TYPE) {
            ERROR(56);
        }
        if (identType == INT_TYPE && exprType != INT_ARRAY_TYPE && exprType != INT_TYPE) {
            ERROR(56);
        }
        semanticAnalyzer.assign(ident, value, identType, exprType);
        printmsg("line %d, This is an assign statement.\n", SYM_LINE);
        return;
    } else if (SYM_TYPE == LEFT_SQUARE) {
        //数组检验
        if (ste.vType != INT_ARRAY_TYPE && ste.vType != CHAR_ARRAY_TYPE) {
            ERROR(42);
        }
        GET_SYM;
        identType = ste.vType;
        expression(offset, offsetType, offsetVariable, staticValue);
        if (staticValue != 0 && (staticValue >= ste.length || staticValue < 0)){
            ERROR(60);
        }

        if (SYM_TYPE != RIGHT_SQUARE) {
            ERROR(13);
        }
        GET_SYM;
        if (SYM_TYPE != ASSIGN) {
            ERROR(11);
        }
        //赋值变量检验
        if (ste.oType != VARIABLE && ste.oType != PARAMETER) {
            ERROR(46);
        }
        GET_SYM;
        expression(value, exprType, variable, staticValue);
        //类型检验
        if (identType == CHAR_ARRAY_TYPE && exprType != CHAR_ARRAY_TYPE && exprType != CHAR_TYPE) {
            ERROR(56);
        }
        if (identType == INT_ARRAY_TYPE && exprType != INT_ARRAY_TYPE && exprType != INT_TYPE) {
            ERROR(56);
        }
        semanticAnalyzer.assignToArray(ident, offset, value, identType, exprType);
        printmsg("line %d, This is an assign statement.\n", SYM_LINE);
    } else {
        ERROR(19);
    }
}

void GrammarAnalyzer::ifStatement() {
    symbolType comparator;
    string src1, src2;
    string endLabel = semanticAnalyzer.newLabel("condition_end");
    if (SYM_TYPE != IF) {
        ERROR(20);
    }
    GET_SYM;
    if (SYM_TYPE != LEFT_BRACKET) {
        ERROR(3);
    }
    GET_SYM;
    condition(comparator, src1, src2);
    semanticAnalyzer.bz(comparator, endLabel, src1, src2);
    if (SYM_TYPE != RIGHT_BRACKET) {
        ERROR(4);
    }
    GET_SYM;
    statement();
    printmsg("line %d, This is a if statement.\n", SYM_LINE);
    semanticAnalyzer.label(endLabel);
}

void GrammarAnalyzer::condition(symbolType &comparator, string &src1, string &src2) {
    valueType type1, type2;
    bool variable1, variable2;
    int staticValue1, staticValue2;
    expression(src1, type1, variable1, staticValue1);
    if(type1 == CHAR_TYPE || type1 == CHAR_ARRAY_TYPE){
        ERROR(58);
    }
    if (SYM_TYPE == EQUAL ||
        SYM_TYPE == LESS_EQUAL ||
        SYM_TYPE == GREATER_EQUAL ||
        SYM_TYPE == LESS ||
        SYM_TYPE == GREATER ||
        SYM_TYPE == UNEQUAL) {
        comparator = SYM_TYPE;
        GET_SYM;
        expression(src2, type2, variable2, staticValue2);
        if(type2 == CHAR_TYPE || type2 == CHAR_ARRAY_TYPE){
            ERROR(58);
        }
    } else {
        comparator = UNEQUAL;
        src2 = "0";
    }
    printmsg("line %d, This is a condition.\n", SYM_LINE);
}

void GrammarAnalyzer::whileStatement() {
    symbolType comparator;
    string src1, src2;
    string endLabel = semanticAnalyzer.newLabel("while_end");
    if (SYM_TYPE != WHILE) {
        ERROR(21);
    }
    GET_SYM;
    string startLabel = semanticAnalyzer.newLabel("while_start");
    semanticAnalyzer.label(startLabel);
    if (SYM_TYPE != LEFT_BRACKET) {
        ERROR(3);
    }
    GET_SYM;
    condition(comparator, src1, src2);
    semanticAnalyzer.bz(comparator, endLabel, src1, src2);
    if (SYM_TYPE != RIGHT_BRACKET) {
        ERROR(4);
    }
    GET_SYM;
    statement();
    semanticAnalyzer.j(startLabel);
    semanticAnalyzer.label(endLabel);
    printmsg("line %d, This is while statement.\n", SYM_LINE);
}

void GrammarAnalyzer::switchStatement() {
    string value;
    valueType type;
    bool variable;
    int staticValue;
    string switchEndLabel = semanticAnalyzer.newLabel("switch_end");
    if (SYM_TYPE != SWITCH) {
        ERROR(22);
    }
    GET_SYM;
    if (SYM_TYPE != LEFT_BRACKET) {
        ERROR(3);
    }
    GET_SYM;
    expression(value, type, variable, staticValue, true);
    if(type == INT_TYPE || type == INT_ARRAY_TYPE)
        type = INT_TYPE;
    else if(type == CHAR_TYPE || type == CHAR_ARRAY_TYPE)
        type = CHAR_TYPE;

    if (SYM_TYPE != RIGHT_BRACKET) {
        ERROR(4);
    }
    GET_SYM;
    if (SYM_TYPE != LEFT_BRACE) {
        ERROR(5);
    }
    GET_SYM;
    caseList(value, type, switchEndLabel);
    if (SYM_TYPE == DEFAULT)
        defaultStatement(switchEndLabel);
    if (SYM_TYPE != RIGHT_BRACE) {
        ERROR(6);
    }
    GET_SYM;
    semanticAnalyzer.label(switchEndLabel);
    printmsg("line %d, This is switch statement.\n", SYM_LINE);
}

void GrammarAnalyzer::caseList(const string &src1, const valueType &type, const string &switchEndLabel) {
    do {
        caseStatement(src1, type, switchEndLabel);
    } while (SYM_TYPE == CASE);
    printmsg("line %d, This is case list.\n", SYM_LINE);
}

void GrammarAnalyzer::caseStatement(const string &src1, const valueType &type, const string &switchEndLabel) {
    string src2;
    string endLabel = semanticAnalyzer.newLabel("case_end");
    valueType caseType;
    if (SYM_TYPE != CASE) {
        ERROR(23);
    }
    GET_SYM;
    if (SYM_TYPE == PLUS || SYM_TYPE == MINUS || SYM_TYPE == UNSIGN_NUMBER) {
        src2 = to_string(integer());
        caseType = INT_TYPE;
    } else if (SYM_TYPE == CHARATER) {
        src2 = "'" + string(1, SYM[1]) + "'";
        caseType = CHAR_TYPE;
        GET_SYM;
    } else {
        ERROR(24);
    }
    if(caseType != type){
        ERROR(58);
    }
    if (SYM_TYPE != COLON) {
        ERROR(25);
    }
    GET_SYM;
    semanticAnalyzer.switchBz(EQUAL, endLabel, src2);
    statement();
    semanticAnalyzer.j(switchEndLabel);
    semanticAnalyzer.label(endLabel);
    printmsg("line %d, This is case statement.\n", SYM_LINE);
}

void GrammarAnalyzer::defaultStatement(const string &switchEndLabel) {
    if (SYM_TYPE != DEFAULT) {
        ERROR(26);
    }
    GET_SYM;
    if (SYM_TYPE != COLON) {
        ERROR(25);
    }
    GET_SYM;
    if (SYM_TYPE == IF ||
        SYM_TYPE == WHILE ||
        SYM_TYPE == LEFT_BRACE ||
        SYM_TYPE == IDENTIFIER ||
        SYM_TYPE == SWITCH ||
        SYM_TYPE == RETURN ||
        SYM_TYPE == SCANF ||
        SYM_TYPE == PRINTF ||
        SYM_TYPE == SEMICOLON)
        statement();
    semanticAnalyzer.j(switchEndLabel);
    printmsg("line %d, This is default statement.\n", SYM_LINE);
}

void GrammarAnalyzer::functionCall(string &rtn_func, valueType &type) {
    if (SYM_TYPE != IDENTIFIER) {
        ERROR(8);
    }
    //查表 函数检验
    symTableEntry ste = symbolTable.searchInTable(SYM, currentFunction);
    if (ste.oType != FUNCTION) {
        ERROR(43);
    }
    string callingFunction = SYM;
    semanticAnalyzer.saveScene(callingFunction);
    GET_SYM;
    if (SYM_TYPE != LEFT_BRACKET) {
        ERROR(3);
    }
    GET_SYM;
    valueParameterList(callingFunction);
    if (SYM_TYPE != RIGHT_BRACKET) {
        ERROR(4);
    }
    printmsg("line %d, This is function call.\n", SYM_LINE);
    semanticAnalyzer.functionCall(callingFunction, rtn_func);
    type = ste.vType;
    GET_SYM;
}

void GrammarAnalyzer::valueParameterList(string callingFunction) {
    int parameterOrder = 0;
    string rtn_expr;
    valueType type;
    bool variable;
    int staticValue;
    if (SYM_TYPE == PLUS ||
        SYM_TYPE == MINUS ||
        SYM_TYPE == IDENTIFIER ||
        SYM_TYPE == LEFT_BRACKET ||
        SYM_TYPE == UNSIGN_NUMBER ||
        SYM_TYPE == CHARATER) {
        expression(rtn_expr, type, variable, staticValue);
        parameterOrder++;
        //查表 对照形参实参
        symTableEntry paraSte = symbolTable.searchInTable(parameterOrder, callingFunction);
        if (paraSte.parameter != parameterOrder) {
            ERROR(40);
        }
        if (paraSte.vType == INT_TYPE && type != INT_TYPE && type != INT_ARRAY_TYPE){
            ERROR(58);
        }
        if (paraSte.vType == CHAR_TYPE && type != CHAR_TYPE && type != CHAR_ARRAY_TYPE){
            ERROR(58);
        }
        semanticAnalyzer.valueParameter(rtn_expr);
        while (SYM_TYPE == COMMA) {
            GET_SYM;
            expression(rtn_expr, type, variable, staticValue);
            parameterOrder++;
            //查表 对照形参实参
            symTableEntry paraSte = symbolTable.searchInTable(parameterOrder, callingFunction);
            if (paraSte.parameter != parameterOrder) {
                ERROR(40);
            }
            if (paraSte.vType == INT_TYPE && type != INT_TYPE && type != INT_ARRAY_TYPE){
                ERROR(58);
            }
            if (paraSte.vType == CHAR_TYPE && type != CHAR_TYPE && type != CHAR_ARRAY_TYPE){
                ERROR(58);
            }
            semanticAnalyzer.valueParameter(rtn_expr);
        }
        printmsg("line %d, This is a parameter list with value.\n", SYM_LINE);
    } else {
        printmsg("line %d, This is an empty parameter list with value.\n", SYM_LINE);
    }
    //查全局表 对照参数个数
    symTableEntry funcSte = symbolTable.searchInGlobalTable(callingFunction);
    if (funcSte.parameter != parameterOrder) {
        ERROR(40);
    }
}

void GrammarAnalyzer::statementList() {
    while (SYM_TYPE == IF ||
           SYM_TYPE == WHILE ||
           SYM_TYPE == LEFT_BRACE ||
           SYM_TYPE == IDENTIFIER ||
           SYM_TYPE == SWITCH ||
           SYM_TYPE == RETURN ||
           SYM_TYPE == SCANF ||
           SYM_TYPE == PRINTF ||
           SYM_TYPE == SEMICOLON)
        statement();
    printmsg("line %d, This is statement list.\n", SYM_LINE);
}

void GrammarAnalyzer::scanfStatement() {
    string ident;
    if (SYM_TYPE != SCANF) {
        ERROR(27);
    }
    GET_SYM;
    if (SYM_TYPE != LEFT_BRACKET) {
        ERROR(3);
    }
    do {
        GET_SYM;
        if (SYM_TYPE != IDENTIFIER) {
            ERROR(8);
        }
        ident = SYM;
        //查表
        symTableEntry ste = symbolTable.searchInTable(SYM, currentFunction);
        if (ste.oType == FAULT) {
            ERROR(41);
        }

        if (ste.oType != VARIABLE) {
            ERROR(50);
        }
        if (ste.vType != INT_TYPE && ste.vType != CHAR_TYPE) {
            ERROR(51);
        }
        GET_SYM;
        semanticAnalyzer.read(ident, ste.vType);
    } while (SYM_TYPE == COMMA);
    if (SYM_TYPE != RIGHT_BRACKET) {
        ERROR(4);
    }
    printmsg("line %d, This is scanf statement.\n", SYM_LINE);
    GET_SYM;
}

void GrammarAnalyzer::printfStatement() {
    string rtn_expr;
    valueType type;
    bool variable;
    int staticValue;
    if (SYM_TYPE != PRINTF) {
        ERROR(28);
    }
    GET_SYM;
    if (SYM_TYPE != LEFT_BRACKET) {
        ERROR(3);
    }
    GET_SYM;
    if (SYM_TYPE == STRING) {
        //查表, 如果不在表中,入表
        if (symbolTable.searchInTable(SYM, currentFunction).oType != FAULT) {
            //入表
            symTableEntry ste = symbolTable.createSte(CONSTANT, STRING_TYPE, SYM.length());
            if (!symbolTable.addToTable(SYM, ste, currentFunction)) {
                ERROR(39);
            }
        }
        semanticAnalyzer.write(STRING_TYPE, SYM);
        GET_SYM;
        if (SYM_TYPE == COMMA) {
            GET_SYM;
            expression(rtn_expr, type, variable, staticValue);
            semanticAnalyzer.write(type, rtn_expr);
            if (SYM_TYPE == RIGHT_BRACKET) {
                printmsg("line %d, This is printf statement.\n", SYM_LINE);
                GET_SYM;
            } else {
                ERROR(4);
            }
        } else if (SYM_TYPE == RIGHT_BRACKET) {
            printmsg("line %d, This is printf statement.\n", SYM_LINE);
            GET_SYM;
        } else {
            ERROR(29);
        }
    } else if (SYM_TYPE == PLUS ||
               SYM_TYPE == MINUS ||
               SYM_TYPE == IDENTIFIER ||
               SYM_TYPE == LEFT_BRACKET ||
               SYM_TYPE == UNSIGN_NUMBER ||
               SYM_TYPE == CHARATER) {
        expression(rtn_expr, type, variable, staticValue);
        semanticAnalyzer.write(type, rtn_expr);
        if (SYM_TYPE == RIGHT_BRACKET) {
            printmsg("line %d, This is printf statement.\n", SYM_LINE);
            GET_SYM;
        } else {
            ERROR(30);
        }
    } else {
        ERROR(31);
    }
    semanticAnalyzer.write(CHAR_TYPE, "'\n'");
}

void GrammarAnalyzer::returnStatement() {
    string rtn_expr;
    valueType type;
    bool variable;
    int staticValue;
    if (SYM_TYPE != RETURN) {
        ERROR(32);
    }
    GET_SYM;
    if (SYM_TYPE == SEMICOLON) {
        symTableEntry ste = symbolTable.searchInGlobalTable(currentFunction);
        if(ste.vType != VOID_TYPE){
            ERROR(59);
        }
        printmsg("line %d, This is void return statement.\n", SYM_LINE);
        semanticAnalyzer.returnStatement("");
        return;
    }
    if (SYM_TYPE != LEFT_BRACKET) {
        ERROR(3);
    }
    GET_SYM;
    if (SYM_TYPE == PLUS ||
        SYM_TYPE == MINUS ||
        SYM_TYPE == IDENTIFIER ||
        SYM_TYPE == LEFT_BRACKET ||
        SYM_TYPE == UNSIGN_NUMBER ||
        SYM_TYPE == CHARATER) {
        expression(rtn_expr, type, variable, staticValue);
        if (SYM_TYPE != RIGHT_BRACKET) {
            ERROR(4);
        }
        GET_SYM;
        symTableEntry ste = symbolTable.searchInGlobalTable(currentFunction);
        if(ste.vType == INT_TYPE && type != INT_TYPE && type != INT_ARRAY_TYPE){
            ERROR(59);
        }
        if(ste.vType == CHAR_TYPE && type != CHAR_TYPE && type != CHAR_ARRAY_TYPE){
            ERROR(59);
        }
        if(ste.vType == VOID_TYPE){
            ERROR(59);
        }
        semanticAnalyzer.returnStatement(rtn_expr);
        printmsg("line %d, This is return statement.\n", SYM_LINE);
    } else {
        ERROR(4);
    }
}

int GrammarAnalyzer::integer() {
    bool nag = false;
    if (SYM_TYPE == PLUS || SYM_TYPE == MINUS) {
        if (SYM_TYPE == MINUS)
            nag = true;
        GET_SYM;
        if (SYM_TYPE == UNSIGN_NUMBER) {
            int rtn = atoi(SYM.c_str());
            GET_SYM;
            return nag ? -rtn : rtn;
        } else {
            ERROR(15);
        }
    } else if (SYM_TYPE == UNSIGN_NUMBER) {
        int rtn = atoi(SYM.c_str());
        GET_SYM;
        return rtn;
    } else {
        ERROR(33);
    }
}
