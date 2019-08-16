#include "GG.h"
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>

#include <fcntl.h>
#include "Connectors.h"

using namespace std;

GG::GG(bool check, vector<string> forGG, vector<string> forGG2) {
    checkit = check;
    v = forGG;
    b = forGG2;
}

bool GG::execute() {
  
    int i;
    char **work = new char*[v.size()+ 1];     // vector of string -> array of char *
    for(i = 0; i < v.size(); ++i) {       //this works
        work[i] = new char[v[i].size() + 1];
        strcpy(work[i], v[i].c_str());
    }
    work[i] = nullptr;

    int stat;
    pid_t pid = fork();
    pid_t signal;
    int waitid;

    int saveout = dup(1);
    int s = open(b.at(0).c_str(),  O_APPEND | O_WRONLY | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

   
    if (pid == 0)
    {
        // or child process

        if(s < 0) {
            perror("ERROR");
            _exit(1);
        }
        if(dup2(s,1) == -1) {
            perror("Error");
            _exit(1);
        }

        close(s);
        dup2(s,1);
        stat = execvp(work[0],work);

       //if program fails
        perror("ERROR: exec failed");
        _exit(1);
    }
    else if (pid > 0)
    {
        // for parent process
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
        // if fork failed
        perror("fork failed");
        _exit(1);
    }
    
    close(saveout);
    dup2(saveout,1);

    return false;
}
