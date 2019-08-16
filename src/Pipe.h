#ifndef PIPE_H
#define PIPE_H

#include <iostream>
#include <vector>
#include "Connectors.h"

using namespace std;

class Pipe : public Connectors {
    private:
        bool checkit;
        vector<string> forPipe;
    public:
        Pipe(bool, vector<string>);
        bool execute();
        bool executeHelper(vector<string>, vector<string>);
};

#endif
