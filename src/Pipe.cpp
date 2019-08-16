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

#include <fcntl.h>
#include <vector>
#include <cstring>
#include <string>
#include "Anding.h"
#include "Connectors.h"
#include "MultiCommand.h"
#include "Exit.h"
#include "Pipe.h"
#include "Less.h"
#include "Greater.h"
#include "GG.h"

using namespace std;

Pipe::Pipe(bool result, vector<string> v) {
    checkit = result;
    forPipe = v;
}

bool Pipe::execute() {
    vector<string> left;
    vector<string> right;
    queue<string> con;

    for (int i = 0; i < forPipe.size(); ++i) {
        if (forPipe.at(i) != "|"/* && forPipe.at(i) != "<" && forPipe.at(i) != ">" && forPipe.at(i) != ">>"*/) {
            if (con.empty()) {
                left.push_back(forPipe.at(i));
            }
            else {
                right.push_back(forPipe.at(i));
                if (i == forPipe.size() - 1) {
                    executeHelper(left, right);
                }
            }
        }
        else {
            con.push(forPipe.at(i));
        }
    }
    return true;
}

bool Pipe::executeHelper(vector<string> l, vector<string> r) {
    int i;
    char **work = new char*[l.size()+ 1];     // vector of string -> array of char *
    for(i = 0; i < l.size(); ++i) {       //this works
        work[i] = new char[l[i].size() + 1];
        strcpy(work[i], l[i].c_str());
    }
    work[i] = nullptr;

    int fds[2];
    int s = pipe(fds);

    if (s == -1) {
        perror("pipe failed");
        exit(1);
    }

    int stat;
    pid_t pid = fork();
    pid_t signal;
    int waitid;

    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0) {

        close(fds[0]); //close input side
        int saveout = dup(1);
        int f = dup2(fds[1],1);
        
        if (f < 0) {
            perror("File does not exist");
            exit(1);
        }
        if (f == -1) {
            perror("dup2 failed");
            exit(1);
        } // dup2(fds[1],1);
        
        stat = execvp(work[0], work);

        close(fds[1]);
        dup2(saveout,1);
        perror("ERROR: exec failed");
        exit(1);

    }
    else if (pid > 0) {
        signal = waitpid(pid,&waitid, 0);
        close(fds[1]);
        int savein = dup(0);
        dup2(fds[0], 0);
        
        if (signal == -1) {
            cout << "1" << endl;
            perror("WAIT");
            exit(1);
        }
        if(waitid > 0) {
            return false;
        }
        else if(waitid == 0) {
            return true;
        }
        else if(waitid == 1) {
            cout << "4" << endl;
            return false;
        }
        string temp;
        vector<string> r2;
        for (int i = 0; i < r.size(); ++i) { //get rid of < > >>
            if (r.at(i) == ">" || r.at(i) == "<" || r.at(i) == ">>") {
                temp = r.at(i);
                r2.push_back(r.at(i+1));
                r.pop_back();
                r.pop_back();
            }
        }

        if (temp == "<") {
            Less *l = new Less(true, r, r2);
            l->execute();
        }
        else if (temp == ">") {
            Greater *g = new Greater(true, r, r2);
            g->execute();
        }
        else if (temp == ">>") {
            GG *gg = new GG(true, r, r2);
            gg->execute();
        }
        int i;
        char **work = new char*[r.size()+ 1];     // vector of string -> array of char *
        for(i = 0; i < r.size(); ++i) {       //this works
            work[i] = new char[r[i].size() + 1];
            strcpy(work[i], r[i].c_str());
        }
        work[i] = nullptr;

        stat = execvp(work[0], work);
        close(fds[0]);
        dup2(savein, 0);
        perror("ERROR: exec failed");
        exit(1);

        
    }
}
