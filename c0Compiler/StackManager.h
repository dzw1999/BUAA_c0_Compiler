//
// Created by andyson on 18-12-1.
//

#ifndef C0COMPILER_STACKMANAGER_H
#define C0COMPILER_STACKMANAGER_H

#include <string>
#include <vector>

using namespace std;

class StackManager {
public:
    vector<string> functionStack;
    StackManager();
};


#endif //C0COMPILER_STACKMANAGER_H
