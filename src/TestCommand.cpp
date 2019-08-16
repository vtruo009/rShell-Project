#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <fcntl.h>
#include "TestCommand.h"

using namespace std;

TestCommand::TestCommand(vector<string> v) {
	com = v;
}

bool TestCommand::execute() {
	string line;
	int flag0, flag, flag1, flag2;
	struct stat buffer;
	int status;
	
    for (int j = 0; j < com.size(); ++j) {
        if (com.at(j) == "]") {
            com.pop_back();
        }
    }

	for(int i = 1; i < com.size(); ++i) {
		if(com.at(i) == "-e") {
			flag = 1;
		}
		else if(com.at(i) == "-f") {
			flag1 = 1;
		}
		else if(com.at(i) == "-d") {
			flag2 = 1;
		}
		else {
			line = com.at(i);
		}
	}
	int siz = line.size();
	char arr[siz + 1];
	strcpy(arr, line.c_str());

	status = stat(arr,&buffer);
	if(status == -1) {
        cout << "Error: No file or directory exists" << endl;
		return false;
	}
	else {
		if(flag == 1 || com.size() == 2) {
			cout << "(True)" << endl;
		}
		else if(flag1 == 1) {
			if(S_ISREG(buffer.st_mode)) {
				cout << "(True)" << endl;
			}
			else {
				cout << "(False)" << endl;
                return false;
			}
		}
		else if(flag2 == 1) {
			if(S_ISDIR(buffer.st_mode)) {
				cout << "(True)" << endl;
			}
			else {
				cout << "(False)" << endl;
                return false;
			}
		}
	}
	
	return true;
}
