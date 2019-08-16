#ifndef MULTICOMMAND_H_
#define MULTICOMMAND_H_

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include "Shell.h"

using namespace std;

class MultiCommand : public Shell {
    private:
        vector<string> commands;
        char**work;
    public:
        MultiCommand(vector<string>);
        bool execute();
};

#endif
