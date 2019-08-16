#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include "MultiCommand.h"

MultiCommand::MultiCommand(vector<string> cmdVect) {
        commands = cmdVect;
}

bool MultiCommand::execute() {
	if (commands.at(0) == "exit") {
		cout << "logout" << endl;
		exit(0);
	}
    work = new char*[commands.size()+ 1];		
    int i;
    for(i = 0; i < commands.size(); ++i) {		
        work[i] = new char[commands[i].size() + 1];
        strcpy(work[i], commands[i].c_str());
    }
    work[i] = nullptr;

    pid_t pid = fork();
    pid_t signal;
    int waitid;
    
    
    if (pid == 0)
    {
        execvp(work[0],work);
        perror("ERROR: exec failed");
        _exit(1);
        
    }
    
    else if (pid > 0)
    {
        
        signal = waitpid(pid, &waitid, 0);
        
        if(signal == -1) {
            perror("WAIT");
            _exit(1);
        }
        if(waitid > 0) {
            return false;
        }
        else if(waitid == 0) {
            return true;
        }
        else if(waitid == 1) {
            return false;
        }
        

    }
    else
    {
        
        perror("fork failed");
        _exit(1);
    }
    
    
    return false;
    
}
