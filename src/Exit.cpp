#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <vector>
#include <cstring>
#include <string>
#include "Exit.h"
#include "Connectors.h"
#include "MultiCommand.h"

Exit::Exit(bool result, vector<string>v) {
	checkit = result;
	forExit = v;

}

bool Exit::execute() {
	exit(0);
}
