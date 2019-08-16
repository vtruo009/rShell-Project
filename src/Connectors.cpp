#include <iostream>
#include "Connectors.h"
#include <string>
#include <vector>
#include <cmath>
#include <queue>
#include "MultiCommand.h"
#include "Anding.h"
#include "Oring.h"
#include "Semicolon.h"
#include "Exit.h"
#include "TestCommand.h"
#include "Greater.h"
#include "GG.h"
#include "Less.h"

using namespace std;

Connectors::Connectors(vector<string> listCommands) {
    commands = listCommands;
}

bool Connectors::execute() {
    vector<string> left;
    vector<string> right;
    vector<string> par;
    queue<string> con;
    string temp;
    int execution = 0, pipes = 0, dcount = 0;
    int count = 0;
    bool pof, flag = true;
   
    for (int i = 0; i < commands.size(); ++i) {
        if (commands.at(i) == "<" || commands.at(i) == ">" || commands.at(i) == ">>") {
            ++dcount;
            flag = false;
        }
        else if (commands.at(i) == "|") {
            ++pipes;
            flag = false;
        }
    }

    for (int i = 0; i < commands.size(); ++i) {
        if (commands.at(i) == "&&" || commands.at(i) == "||" || commands.at(i) == ";") {
            ++count;
        }

    }

    //zero pipes
    if ((dcount == 1 || dcount == 0) && pipes == 0 && flag == false) {
        for (int i = 0; i < commands.size(); ++i) {
            if (commands.at(i) == ">" || commands.at(i) == "<" || commands.at(i) == ">>") {
                con.push(commands.at(i));
            }
            else if (con.empty()) {
                left.push_back(commands.at(i));
            }
            else {
                right.push_back(commands.at(i));
            }
        }

        if (con.front() == ">") {
            Greater *g = new Greater(pof, left, right);
            g->execute();
        }
        else if (con.front() == "<") {
            Less *l = new Less(pof, left, right);
            l->execute();
        }
        else if (con.front() == ">>") {
            GG *gg = new GG(pof, left, right);
            gg->execute();
        }
        return pof;
    }

    //no connectors
    if (count == 0) {
        if (commands.at(0) == "exit") {
            Exit *e = new Exit(pof, commands);
            e->execute();
        }
        else if (commands.at(0) == "test" || commands.at(0) == "[") {
            if (commands.size() == 1) {
                return pof;
            }
            TestCommand *t = new TestCommand(commands);
            pof = t->execute();
            return pof;
        }
        else if (commands.at(0) == "(") {
            vector<string> temp;
            temp.push_back(commands.at(1));
            MultiCommand *cmd = new MultiCommand(temp);
            pof = cmd->execute();
            return pof;
        }
        else {
            MultiCommand *cmd = new MultiCommand(commands);
            pof = cmd->execute();
            return pof;
        }
    }
    //one connector
    if (count == 1) {
        vector<string> temp;
        for (int i = 0; i < commands.size(); ++i) {
            if (commands.at(i) == "(" || commands.at(i) == ")") {
                i = i;
            }
            else if (commands.at(i) != "&&" && commands.at(i) != "||" && commands.at(i) != ";") {
                if (con.empty()) {
                    left.push_back(commands.at(i));
                }
                else {
                    right.push_back(commands.at(i));
                }
            }
            else {
                con.push(commands.at(i));
            }
        }
        if (execution == 0) {
            if (left.at(0) == "test" || left.at(0) == "[") {
                TestCommand *t = new TestCommand(left);
                pof = t->execute();
                ++execution;
            }
            else {
                MultiCommand *cmd = new MultiCommand(left);
                pof = cmd->execute();
                ++execution;
            }
        }
        if (con.front() == "&&") {
            if (right.at(0) == "test" || right.at(0) == "[") {
                TestCommand *t = new TestCommand(right);
                pof = t->execute();
            }
            else {
                Anding *a = new Anding(pof, right);
                pof = a->execute();
            }
        }
        else if (con.front() == "||") {
            Oring *o = new Oring(pof, right);
            pof = o->execute();
        }
        else if (con.front() == ";") {
            Semicolon *s = new Semicolon(pof, right);
            pof = s->execute();
        }
        return pof;
    }
    //multiple connectors
    for (int i = 0; i < commands.size(); ++i) {
        int op = 0, cp = 0;
        if (commands.at(i) == ")" && i == commands.size() - 1) {
            return pof;
        }
        else if (commands.at(i) == "test" || commands.at(i) == "[") {
            if ( i != commands.size() - 1 && (commands.at(i+1) != "&&" && commands.at(i+1) != "||" && commands.at(i+1) != ";")) { //ADDED THIS IF BRANCH
                vector<string> temp;
                int j = i;
                while (commands.at(j) != "&&" && commands.at(j) != "||" && commands.at(j) != ";") {
                    temp.push_back(commands.at(j));
                    ++j;
                }
                TestCommand *t = new TestCommand(temp);
                pof = t->execute();
                ++execution;
                if (commands.at(i) == "test") {
                    i = j - 1;
                }
                else {
                    i = j;
                }
            }
            else { //ADDED THIS PART
                if (execution == 0 && con.empty()) { //if con is not empty then first command is complete
                    left.push_back(commands.at(i));
                }
                else {
                    right.push_back(commands.at(i));
                    if (!con.empty()) {
                        if (con.size() >= 2) {
                            con.pop();
                        }
                        if (i == commands.size() - 1) {
                            if (con.front() == "&&") {
                                Anding *a = new Anding(pof, right);
                                pof = a->execute();
                            }
                            else if (con.front() == "||") {
                                Oring *o = new Oring(pof, right);
                                pof = o->execute();
                                
                            }
                            else if (con.front() == ";") {
                                Semicolon *s = new Semicolon(pof, right);
                                pof = s->execute();
                            }
                            con.pop();
                            right.clear();
                            ++execution;
                        }
                        else if (commands.at(i+1) == "&&" || commands.at(i+1) == "||" || commands.at(i+1) == ";") {
                            if (con.front() == "&&") {
                                Anding *a = new Anding(pof, right);
                                pof = a->execute();
                            }
                            else if (con.front() == "||") {
                                Oring *o = new Oring(pof, right);
                                pof = o->execute();
                                
                            }
                            else if (con.front() == ";") {
                                Semicolon *s = new Semicolon(pof, right);
                                pof = s->execute();
                            }
                            con.pop();
                            right.clear();
                            ++execution;
                        }
                        else if (commands.at(i+1) == ")" && i+1 == commands.size() - 1) {
                            if (con.front() == "&&") {
                                Anding *a = new Anding(pof, right);
                                pof = a->execute();
                            }
                            else if (con.front() == "||") {
                                Oring *o = new Oring(pof, right);
                                pof = o->execute();
                                
                            }
                            else if (con.front() == ";") {
                                Semicolon *s = new Semicolon(pof, right);
                                pof = s->execute();
                            }
                            con.pop();
                            right.clear();
                            ++execution;
                        }
                    }
                }
            }
        }
        else if (commands.at(i) == "(") { //parenthasis
            int j;
            for (j = i+1; j < commands.size(); ++j) {
                if (commands.at(j) == "(") {
                    ++op;
                }
                else if (commands.at(j) == ")") {
                    ++cp;
                }
                if (op == cp - 1) {
                    break;
                }
                par.push_back(commands.at(j));
            }
            i = j;
            if (par.back() == "&&" || par.back() == "||" || par.back() == ";") {
                con.push(par.back());
                par.pop_back();
                Connectors *c = new Connectors;
                pof = c->executeHelper(pof, par, con);
                ++execution;
                --i;
            }
            else {
                if (!con.empty()) {
                    if (con.size() >= 2) {
                        con.pop();
                    }
                    if ( (con.front() == "&&" && pof == true) || (con.front() == "||" && pof == false) || (con.front() == ";") ) {
                        Connectors *c = new Connectors(par); //return
                        pof = c->execute();
                        con.pop();
                        ++execution;
                    }
                }
                else {
                    Connectors *c = new Connectors(par);
                    pof = c->execute();
                    ++execution;
                }
            }
            par.clear();
            if (con.size() >= 2) {
                con.pop();
            }
        }
        else if (commands.at(i) != "&&" && commands.at(i) != "||" && commands.at(i) != ";") {
            if (execution == 0 && con.empty()) { //if con is not empty then first command is complete
                left.push_back(commands.at(i));
            }
            else {
                right.push_back(commands.at(i));
                if (!con.empty()) {
                    if (con.size() >= 2) {
                        con.pop();
                    }
                    if (i == commands.size() - 1) {
                        if (con.front() == "&&") {
                            Anding *a = new Anding(pof, right);
                            pof = a->execute();
                        }
                        else if (con.front() == "||") {
                            Oring *o = new Oring(pof, right);
                            pof = o->execute();
                            
                        }
                        else if (con.front() == ";") {
                            Semicolon *s = new Semicolon(pof, right);
                            pof = s->execute();
                        }
                        con.pop();
                        right.clear();
                        ++execution;
                    }
                    else if (commands.at(i+1) == "&&" || commands.at(i+1) == "||" || commands.at(i+1) == ";") {
                        if (con.front() == "&&") {
                            Anding *a = new Anding(pof, right);
                            pof = a->execute();
                        }
                        else if (con.front() == "||") {
                            Oring *o = new Oring(pof, right);
                            pof = o->execute();
                            
                        }
                        else if (con.front() == ";") {
                            Semicolon *s = new Semicolon(pof, right);
                            pof = s->execute();
                        }
                        con.pop();
                        right.clear();
                        ++execution;
                    }
                    else if (commands.at(i+1) == ")" && i+1 == commands.size() - 1) {
                        if (con.front() == "&&") {
                            Anding *a = new Anding(pof, right);
                            pof = a->execute();
                        }
                        else if (con.front() == "||") {
                            Oring *o = new Oring(pof, right);
                            pof = o->execute();
                            
                        }
                        else if (con.front() == ";") {
                            Semicolon *s = new Semicolon(pof, right);
                            pof = s->execute();
                        }
                        con.pop();
                        right.clear();
                        ++execution;
                    }
                }
            }
        }
        else { //str.at(i) is a connector
            con.push(commands.at(i));
            if (!con.empty() && !left.empty() ) {
                MultiCommand *cmd = new MultiCommand(left);
                pof = cmd->execute();
                ++execution;
                left.clear();
            }
            else if (!con.empty() && !right.empty()) {
                if (con.front() == "&&") {
                    Anding *a = new Anding(pof, right);
                    pof = a->execute();
                }
                else if (con.front() == "||") {
                    Oring *o = new Oring(pof, right);
                    pof = o->execute();
                }
                else if (con.front() == ";") {
                    Semicolon *s = new Semicolon(pof, right);
                    pof = s->execute();
                }
                ++execution;
                right.clear();
            }
        }
    }
    return pof;
}

bool Connectors::executeHelper(bool tof, vector<string> forPar, queue<string>con) {
    vector<string> pleft;
    vector<string> pright;
    vector<string> paren;
    queue<string> pcon;
    string temp;
    int execution = 0;
    int count = 0;

    for (int i = 0; i < forPar.size(); ++i) {
        if (forPar.at(i) == "&&" || forPar.at(i) == "||" || forPar.at(i) == ";") {
            ++count;
        }
    }
    //no connectors
    if (count == 0) {
        if (forPar.at(0) == "(") {
            temp = forPar.at(1);
            forPar.clear();
            forPar.push_back(temp);
        }
        if (!con.empty()) {
            if (execution == 0) {
                MultiCommand *cmd = new MultiCommand(forPar);
                tof = cmd->execute();
                ++execution;
            }
            else if (con.front() == "&&") {
                Anding *a = new Anding(tof, forPar);
                tof = a->execute();
            }
            else if (con.front() == "||") {
                Oring *o = new Oring(tof, forPar);
                tof = o->execute();
            }
            else if (con.front() == ";") {
                Semicolon *s = new Semicolon(tof, forPar);
                tof = s->execute();
            }
            return tof;
        }
        else if (forPar.at(0) == "exit") {
            Exit *e = new Exit(tof, forPar);
            e->execute();
        }
        else {
            MultiCommand *cmd = new MultiCommand(forPar);
            tof = cmd->execute();
            return tof;
        }
        return tof;
    }
    //one connector
    if (count == 1) {
        for (int i = 0; i < forPar.size(); ++i) {
            if (forPar.at(i) != "&&" && forPar.at(i) != "||" && forPar.at(i) != ";") {
                if (pcon.empty()) {
                    pleft.push_back(forPar.at(i));
                }
                else {
                    pright.push_back(forPar.at(i));
                }
            }
            else {
                pcon.push(forPar.at(i));
            }
        }
        //execute the separated commands in pleft and pright
        if (con.size() >= 2) {
            if ( (con.front() == "&&" && tof == true) || (con.front() == "||" && tof == false) || (con.front() == ";") ) {
                MultiCommand *cmd = new MultiCommand(pleft);
                tof = cmd->execute();
                ++execution;
                if (pcon.front() == "&&") {
                    Anding *a = new Anding(tof, pright);
                    tof = a->execute();
                }
                else if (pcon.front() == "||") {
                    Oring *o = new Oring(tof, pright);
                    tof = o->execute();
                }
                else if (pcon.front() == ";") {
                    Semicolon *s = new Semicolon(tof, pright);
                    tof = s->execute();
                }
                return tof;
            }
            return tof;
        }
        else {
            MultiCommand *cmd = new MultiCommand(pleft);
            tof = cmd->execute();
            if (pcon.front() == "&&") {
                Anding *a = new Anding(tof, pright);
                tof = a->execute();
            }
            else if (pcon.front() == "||") {
                Oring *o = new Oring(tof, pright);
                tof = o->execute();
            }
            else if (pcon.front() == ";") {
                Semicolon *s = new Semicolon(tof, pright);
                tof = s->execute();
            }
            return tof;
        }

        return tof;
    }
    //before anything, need to separate commands
    for (int i = 0; i < forPar.size(); ++i) {
        if (forPar.at(i) != "&&" && forPar.at(i) != "||" && forPar.at(i) != ";") { //is a command
            if (execution == 0 && pcon.empty()) {
                pleft.push_back(forPar.at(i));
            }
            else {
                pright.push_back(forPar.at(i));
                if(!pcon.empty()) {
                    if (i == forPar.size() - 1) {
                        if (pcon.front() == "&&") {
                            Anding *a = new Anding(tof, pright);
                            tof = a->execute();
                        }
                        else if (pcon.front() == "||") {
                            Oring *o = new Oring(tof, pright);
                            tof = o->execute();
                        }
                        else if (pcon.front() == ";") {
                            Semicolon *s = new Semicolon(tof, pright);
                            tof = s->execute();
                        }
                        pcon.pop();
                        pright.clear();
                        ++execution;
                    }
                    else if (forPar.at(i+1) == "&&" || forPar.at(i+1) == "||" || forPar.at(i+1) == ";"){
                        if (pcon.front() == "&&") {
                            Anding *a = new Anding(tof, pright);
                            tof = a->execute();
                        }
                        else if (pcon.front() == "||") {
                            Oring *o = new Oring(tof, pright);
                            tof = o->execute();
                        }
                        else if (pcon.front() == ";") {
                            Semicolon *s = new Semicolon(tof, pright);
                            tof = s->execute();
                        }
                        pcon.pop();
                        pright.clear();
                        ++execution;
                    }               
                }
            }
        }
        else { //is a connector
            pcon.push(forPar.at(i));
            if (!con.empty()) {
                if ( (con.front() == "&&" && tof == true) || (con.front() == "||" && tof == false) || con.front() == ";") {
                    MultiCommand *cmd = new MultiCommand(pleft);
                    tof = cmd->execute();
                    ++execution;
                    if (pcon.front() == "&&") {
                        Anding *a = new Anding(tof, pright);
                        tof = a->execute();
                    }
                    else if (pcon.front() == "||") {
                        Oring *o = new Oring(tof, pright);
                        tof = o->execute();
                    }
                    else if (pcon.front() == ";") {
                        Semicolon *s = new Semicolon(tof, pright);
                        tof = s->execute();
                    }
                    return tof;
                }
            }
            else if (!pcon.empty() && !pleft.empty()) {
                MultiCommand *cmd = new MultiCommand(pleft);
                tof = cmd->execute();
                ++execution;
                pleft.clear();
            }
            else if (!pcon.empty() && !pright.empty()) {
                if (pcon.front() == "&&") {
                    Anding *a = new Anding(tof, pright);
                    tof = a->execute();
                }
                else if (pcon.front() == "||") {
                    Oring *o = new Oring(tof, pright);
                    tof = o->execute();
                }
                else if (pcon.front() == ";") {
                    Semicolon *s = new Semicolon(tof, pright);
                    tof = s->execute();
                }
                ++execution;
                pright.clear();
            }
        }
    }

    return tof;
}
