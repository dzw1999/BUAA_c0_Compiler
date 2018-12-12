//
// Created by andyson on 18-11-30.
//

#ifndef C0COMPILER_TYPE_H
#define C0COMPILER_TYPE_H

#include <map>
#define MAX_SYMBOL_NUM 10000        // 最大symbol数
#define MAX_SYMBOL_LENGTH 1000      // 最大symbol长度

using namespace std;

//符号表
enum objectType{
    CONSTANT,
    VARIABLE,
    FUNCTION,
    PARAMETER,
    FAULT
};

enum valueType{
    INT_TYPE,
    CHAR_TYPE,
    INT_ARRAY_TYPE,
    CHAR_ARRAY_TYPE,
    STRING_TYPE,
    VOID_TYPE
};

struct symTableEntry{
    objectType oType;
    valueType vType;
    int parameter;
    int length;
    int offset;
} ;

typedef map<string, symTableEntry> symTable;

//四元式
enum Operator {
    ADD,    // 加法
    SUB,    // 减法
    MUL,    // 乘法
    DIV,    // 除法
    ASS,    // 赋值
    ARAS,   // 给数组元素赋值
    GAR,    // 获取数组元素
    FAS,    // 给函数赋返回值
    CUT,    // 将整数截断成字符型
    LAB,    // 标签
    CMP,    // 比较
    BG,     // 大于跳转
    BGE,    // 大于或等于跳转
    BL,     // 小于跳转
    BLE,    // 小于或等于跳转
    BEQ,    // 等于跳转
    BNE,    // 不等于跳转
    SWITCH_BNE,  //switch bne
    SWITCH_VAR, //switch var
    J,      // 无条件跳转
    CALL,   // 函数调用，带返回值
    RET,    // 函数返回
    PUSH,   // 压栈
    RI,     // 读整数
    RC,     // 读字符
    WS,     // 写字符串
    WI,     // 写整数
    WC,     // 写字符
    VAR_DECLARE,        //变量声明
    CONST_DECLARE,      //常量声明
    MAIN_FUNCTION_DEFINE,    //main函数定义
    FUNCTION_DEFINE,    //函数定义
    FUNCTION_END,       //函数定义结束
    PARAMETER_DECLARE          //参数
};

struct Quad{
    Operator op;
    string src1;
    string src2;
    string dst;
    bool global;
};

//词法分析
enum symbolType {
    ILLEGAL,            // 非法字符
    FINISH,             // 文件结尾

    MAIN,               // main 关键字
    CONST,              // const 关键字
    INT,                // int 关键字
    CHAR,               // char 关键字
    WHILE,              // while 关键字
    IF,                 // if 关键字
    SWITCH,             // switch 关键字
    CASE,               // case 关键字
    DEFAULT,            // default 关键字
    RETURN,             // return 关键字
    SCANF,              // scanf 关键字
    PRINTF,             // printf 关键字

    IDENTIFIER,         // 标识符
    UNSIGN_NUMBER,      // 无符号整数
    STRING,             // 字符串
    CHARATER,           // char 类型的字符

    LEFT_BRACKET,       // 左小括号
    RIGHT_BRACKET,      // 右小括号
    LEFT_SQUARE,        // 左中括号
    RIGHT_SQUARE,       // 右中括号
    LEFT_BRACE,         // 左大括号
    RIGHT_BRACE,        // 右大括号
    COMMA,              // 逗号
    SEMICOLON,          // 分号
    PLUS,               // 加号
    MINUS,              // 减号
    ASTERISK,           // 星号,乘号
    SLASH,              // 斜杠,除号
    ASSIGN,             // 单等号,赋值号
    EQUAL,              // 双等号
    LESS,               // 小于号
    LESS_EQUAL,         // 小于等于号
    GREATER,            // 大于号
    GREATER_EQUAL,      // 大于等于号
    UNEQUAL,            // 不等号
    COLON,              // 冒号

    VOID                // void 关键字
};
#endif //C0COMPILER_TYPE_H
