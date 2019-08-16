#ifndef LESS_H
#define LESS_H

#include <iostream>
#include <vector> 
#include "Connectors.h"

using namespace std;

class Less : public Connectors {
    private:
        bool checkit;
        vector<string> v;
        vector<string> b;
    public:
	Less() {}
    	Less (bool result, vector<string>, vector<string>);
    	bool execute();

};
#endif
