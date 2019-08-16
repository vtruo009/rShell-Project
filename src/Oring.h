#ifndef _ORING_H_
#define	_ORING_H_

#include <string>
#include <iostream>
#include <vector>
#include "Connectors.h"


class Oring: public Connectors {
	private:
		vector<string> forOr;
		bool checkit;
	public:
		Oring(){};
		Oring(bool result, vector<string>v);
		bool execute();
		
};

#endif
