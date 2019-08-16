#ifndef _EXIT_H_
#define	_EXIT_H_

#include <string>
#include <iostream>
#include <vector>
#include "Connectors.h"



using namespace std;

class Exit: public Connectors {
	private:
		vector<string> forExit;
		bool checkit;
	public:
		Exit() {};
		Exit(bool result, vector<string>v);
		bool execute();
};
#endif
