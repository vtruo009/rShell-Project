#ifndef GG_H
#define GG_H

#include <iostream>
#include "Connectors.h"
#include <stdio.h>
#include <unistd.h>


#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <cstring>
#include <string>
#include "Anding.h"
#include "Connectors.h"
#include "MultiCommand.h"
#include "Exit.h"

using namespace std;

class GG : public Connectors {
    private:
        bool checkit;
        vector<string> v;
        vector<string> b;
    public:
        GG() {}
        GG(bool, vector<string>, vector<string>);
        bool execute();
};

#endif
