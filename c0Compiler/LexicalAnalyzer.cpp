//
// Created by andyson on 18-11-13.
//
#include <cstdio>
#include <cctype>
#include <cstring>
#include "LexicalAnalyzer.h"

#define ADD_TO_SYMBOL_LIST \
    LexicalAnalyzer::symbolList[symbolNum] = symGot; \
    LexicalAnalyzer::symbolTypeList[LexicalAnalyzer::symbolNum] = symType; \
    LexicalAnalyzer::symbolLineList[symbolNum] = lineCnt; \
    symbolNum++

int isStrChar(char x) {
    return x == 32 || x == 33 || x >= 35 && x <= 126;
}


LexicalAnalyzer &LexicalAnalyzer::getLexicalAnalyzer(FILE *fin, ExceptionHandler &theExceptionHandler) {
    static LexicalAnalyzer instance(fin, theExceptionHandler);
    return instance;
}

LexicalAnalyzer::LexicalAnalyzer(FILE *input, ExceptionHandler &theExceptionHandler) : exceptionHandler(
        theExceptionHandler) {
    LexicalAnalyzer::symbolNum = 0;
    LexicalAnalyzer::lineCnt = 1;
    LexicalAnalyzer::columnCnt = 0;
    LexicalAnalyzer::fin = input;
    LexicalAnalyzer::nextChar();    //预读
    //初始化keywords map
    LexicalAnalyzer::keywords.insert(pair<string, symbolType>("main", MAIN));
    LexicalAnalyzer::keywords.insert(pair<string, symbolType>("const", CONST));
    LexicalAnalyzer::keywords.insert(pair<string, symbolType>("void", VOID));
    LexicalAnalyzer::keywords.insert(pair<string, symbolType>("int", INT));
    LexicalAnalyzer::keywords.insert(pair<string, symbolType>("char", CHAR));
    LexicalAnalyzer::keywords.insert(pair<string, symbolType>("while", WHILE));
    LexicalAnalyzer::keywords.insert(pair<string, symbolType>("if", IF));
    LexicalAnalyzer::keywords.insert(pair<string, symbolType>("switch", SWITCH));
    LexicalAnalyzer::keywords.insert(pair<string, symbolType>("case", CASE));
    LexicalAnalyzer::keywords.insert(pair<string, symbolType>("default", DEFAULT));
    LexicalAnalyzer::keywords.insert(pair<string, symbolType>("return", RETURN));
    LexicalAnalyzer::keywords.insert(pair<string, symbolType>("scanf", SCANF));
    LexicalAnalyzer::keywords.insert(pair<string, symbolType>("printf", PRINTF));
}

void LexicalAnalyzer::output(FILE *fout) {
    do {
        LexicalAnalyzer::nextSymbol();
    } while (symbolTypeList[symbolNum - 1] != FINISH);
    for (int i = 0; i < symbolNum; i++) {
        fprintf(fout, "%5d\t%10s\t%12s\n", i + 1, symbolList[i].c_str(), enumToStr(symbolTypeList[i]).c_str());
    }

}

void LexicalAnalyzer::lexicalAnalyze() {
    do {
        try {
            LexicalAnalyzer::nextSymbol();
        }catch (int errorCode){
            exceptionHandler.error(55, lineCnt, symGot);
        }
    } while (symbolTypeList[symbolNum - 1] != FINISH);
}

void LexicalAnalyzer::nextChar() {
    LexicalAnalyzer::charGot = fgetc(LexicalAnalyzer::fin);
    columnCnt++;
    if (charGot == '\n') {
        lineCnt++;
        columnCnt = 0;
    }
}

void LexicalAnalyzer::nextSymbol() {
    int symPtr = 0;
    if (isdigit(charGot)) {     //整数
        if (charGot == '0') {
            symGot[symPtr++] = charGot;
            nextChar();
            symGot[symPtr++] = '\0';
            if (isdigit(charGot)) {
                symType = ILLEGAL;
            } else {
                symType = UNSIGN_NUMBER;
            }
        } else {
            do {
                symGot[symPtr++] = charGot;
                nextChar();
            } while (isdigit(charGot));
            symGot[symPtr++] = '\0';
            symType = UNSIGN_NUMBER;
        }
        ADD_TO_SYMBOL_LIST;
    } else if (isalpha(charGot) || charGot == '_') {
        do {
            symGot[symPtr++] = charGot;
            nextChar();
        } while (isalnum(charGot) || charGot == '_');
        symGot[symPtr++] = '\0';
        //查关键字表
        map<string, symbolType>::iterator it = keywords.find(symGot);
        if (it == keywords.end())       // 不在保留字表中，说明是标识符
            symType = IDENTIFIER;
        else
            symType = it->second;       // 要把保留字的具体类型存入 symType
        ADD_TO_SYMBOL_LIST;
    } else if (charGot == '\"') {
        do {
            if (charGot == '\\')
                symGot[symPtr++] = '\\';
            symGot[symPtr++] = charGot;
            nextChar();
        } while (isStrChar(charGot));
        if (charGot == '\"') {
            symType = STRING;
            symGot[symPtr++] = charGot;
            nextChar();
        } else {
            symType = ILLEGAL;
        }
        symGot[symPtr++] = '\0';
        ADD_TO_SYMBOL_LIST;
    } else if (charGot == '\'') {
        symGot[symPtr++] = charGot;
        nextChar();
        if (isalnum(charGot) || charGot == '_' || charGot == '+' || charGot == '-' || charGot == '*' ||
            charGot == '/') {
            symGot[symPtr++] = charGot;
            nextChar();
            if (charGot == '\'') {
                symType = CHARATER;
                symGot[symPtr++] = charGot;
                nextChar();
            } else {
                symType = ILLEGAL;
            }
        } else {
            symType = ILLEGAL;
        }
        symGot[symPtr++] = '\0';
        ADD_TO_SYMBOL_LIST;
    } else if (charGot == ' ' || charGot == '\n' || charGot == '\t' || charGot == '\r') {
        nextChar();
        return;
    } else {
        int preRead = 0;       //标记是否进行过预读
        symGot[symPtr++] = charGot;
        switch (charGot) {
            case EOF:
                symType = FINISH;
                break;
            case '(':
                symType = LEFT_BRACKET;
                break;
            case ')':
                symType = RIGHT_BRACKET;
                break;
            case '[':
                symType = LEFT_SQUARE;
                break;
            case ']':
                symType = RIGHT_SQUARE;
                break;
            case '{':
                symType = LEFT_BRACE;
                break;
            case '}':
                symType = RIGHT_BRACE;
                break;
            case ';':
                symType = SEMICOLON;
                break;
            case ',':
                symType = COMMA;
                break;
            case '+':
                symType = PLUS;
                break;
            case '-':
                symType = MINUS;
                break;
            case '*':
                symType = ASTERISK;
                break;
            case '/':
                symType = SLASH;
                break;
            case ':':
                symType = COLON;
                break;
            case '=':
                symType = ASSIGN;
                nextChar();
                if (charGot == '=') {
                    symGot[symPtr++] = charGot;
                    symType = EQUAL;
                } else
                    preRead = 1;
                break;
            case '!':
                symType = ILLEGAL;
                nextChar();
                if (charGot == '=') {
                    symGot[symPtr++] = charGot;
                    symType = UNEQUAL;
                } else
                    preRead = 1;
                break;
            case '<':
                symType = LESS;
                nextChar();
                if (charGot == '=') {
                    symGot[symPtr++] = charGot;
                    symType = LESS_EQUAL;
                } else
                    preRead = 1;
                break;
            case '>':
                symType = GREATER;
                nextChar();
                if (charGot == '=') {
                    symGot[symPtr++] = charGot;
                    symType = GREATER_EQUAL;
                } else
                    preRead = 1;
                break;
            default:
                symType = ILLEGAL;
        }
        symGot[symPtr++] = '\0';
        ADD_TO_SYMBOL_LIST;
        if (!preRead)
            nextChar();
    }
    if(symType == ILLEGAL)
        throw 55;
}


string LexicalAnalyzer::enumToStr(symbolType st) {
    switch (st + 1) {
        case 1:
            return "ILLEGAL";
        case 2:
            return "FINISH";
        case 3:
            return "MAIN";
        case 4:
            return "CONST";
        case 5:
            return "INT";
        case 6:
            return "CHAR";
        case 7:
            return "FOR";
        case 8:
            return "IF";
        case 9:
            return "SWITCH";
        case 10:
            return "CASE";
        case 11:
            return "DEFAULT";
        case 12:
            return "RETURN";
        case 13:
            return "SCANF";
        case 14:
            return "PRINTF";
        case 15:
            return "IDENTIFIER";
        case 16:
            return "UNSIGN_NUMBER";
        case 17:
            return "STRING";
        case 18:
            return "CHARATER";
        case 19:
            return "LEFT_BRACKET";
        case 20:
            return "RIGHT_BRACKET";
        case 21:
            return "LEFT_SQUARE";
        case 22:
            return "RIGHT_SQUARE";
        case 23:
            return "LEFT_BRACE";
        case 24:
            return "RIGHT_BRACE";
        case 25:
            return "COMMA";
        case 26:
            return "SEMICOLON";
        case 27:
            return "PLUS";
        case 28:
            return "MINUS";
        case 29:
            return "ASTERISK";
        case 30:
            return "SLASH";
        case 31:
            return "ASSIGN";
        case 32:
            return "EQUAL";
        case 33:
            return "LESS";
        case 34:
            return "LESS_EQUAL";
        case 35:
            return "GREATER";
        case 36:
            return "GREATER_EQUAL";
        case 37:
            return "UNEQUAL";
        case 38:
            return "COLON";
        case 39:
            return "VOID";
        default:
            return "ERROR!!!";
    }
}
