#ifndef SEMICOLON_H_
#define SEMICOLON_H_

#include <iostream>
#include <string>
#include "Connectors.h"

using namespace std;

class Semicolon : public Connectors {
    private:
        vector<string> forSemi;
        bool checkit;
    public:
        Semicolon() {};
        Semicolon(bool, vector<string>);
        bool execute();
};

#endif
