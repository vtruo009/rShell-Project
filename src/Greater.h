#ifndef GREATER_H
#define GREATER_H

#include <iostream>
#include "Connectors.h"
#include <stdio.h>
#include <unistd.h>


#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <cstring>
#include <string>
#include "Connectors.h"

using namespace std;

class Greater : public Connectors {
    private:
        bool checkit;
        vector<string> v;
        vector<string> b;
    public:
        Greater() {}
	Greater(bool, vector<string>, vector<string>);
        bool execute();
};

#endif
