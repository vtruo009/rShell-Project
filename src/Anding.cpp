#include <iostream>

#include <stdio.h>
#include <unistd.h>


#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <cstring>
#include <string>
#include "Anding.h"
#include "Connectors.h"
#include "MultiCommand.h"
#include "Exit.h"

Anding::Anding(bool result, vector<string> v) {
	checkit = result;
	forAnd = v;
}

bool Anding::execute() {
	if(checkit == false) {
		return checkit;
	}
	else {
		if (forAnd.at(0) == "exit") {
			cout << "logout" << endl;
			exit(0);

		}
		else {
			MultiCommand *exec = new MultiCommand(forAnd);
			return exec->execute();
		}
	}
}
