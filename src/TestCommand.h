#ifndef TESTCOMMAND_H
#define TESTCOMMAND_H
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <fcntl.h>
#include "Shell.h"

using namespace std;

class TestCommand : public Shell {
	private:
		vector<string> com;
	public:
		TestCommand(){};
		TestCommand(vector<string> v);
		bool execute();
};

#endif
