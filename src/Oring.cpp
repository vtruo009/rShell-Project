#include <iostream>

#include <stdio.h>
#include <unistd.h>


#include <sys/types.h>
#include <sys/wait.h>

#include <vector>
#include <cstring>
#include <string>
#include "Oring.h"
#include "Connectors.h"
#include "MultiCommand.h"

using namespace std;

Oring::Oring(bool result, vector<string>v) {
	forOr = v;
	checkit = result;

}

bool Oring::execute() {
	if(checkit) {
		return checkit;
	}
	else {
		if (forOr.at(0) == "exit") {
			cout << "logout" << endl;
			exit(0);
		}
		else {
			MultiCommand * exec = new MultiCommand(forOr);
			return exec->execute();
		}
	}  
}
