#ifndef CONNECTORS_H_
#define CONNECTORS_H_

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include "Shell.h"

using namespace std;

class Connectors : public Shell {
    protected:
        string str;
        vector<string> commands;
    public:
        Connectors() {};
        Connectors(vector<string>);
        bool execute();
	bool executeHelper(bool, vector<string>, queue<string>);
};

#endif
