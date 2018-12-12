//
// Created by andyson on 18-12-1.
//

#ifndef C0COMPILER_STACKMANAGER_H
#define C0COMPILER_STACKMANAGER_H

#include <string>
#include <stack>

using namespace std;

class StackManager {
public:
    stack<string> functionStack;
    StackManager();
};


#endif //C0COMPILER_STACKMANAGER_H
