#include <iostream>

#include <stdio.h>
#include <unistd.h>


#include <sys/types.h>
#include <sys/wait.h>

#include <vector>
#include <cstring>
#include <string>
#include "Semicolon.h"
#include "Connectors.h"
#include "MultiCommand.h"

using namespace std;

Semicolon::Semicolon(bool result, vector<string>v) {
	forSemi = v;
	checkit = result;

}

bool Semicolon::execute() {
	if (forSemi.at(0) == "exit") {
		cout << "logout" << endl;
		exit(0);
	}
	else {
		MultiCommand * exec = new MultiCommand(forSemi);
		return	exec->execute();
	}
}
