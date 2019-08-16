#ifndef ANDING_H_
#define ANDING_H_

#include <iostream>
#include <vector> 
#include "Connectors.h"

using namespace std;

class Anding : public Connectors {
    private:
        bool checkit;
        vector<string> forAnd;
    public:
        Anding(bool, vector<string>);
        bool execute();
};

#endif
