#include <iostream>
#include <string>
#include <vector>
#include "Shell.h"
#include "Connectors.h"
#include "MultiCommand.h"
#include "Anding.h"
#include "Oring.h"
#include "Semicolon.h"
#include "Exit.h"
#include "TestCommand.h"
#include "GG.h"
#include "Less.h"
#include "Greater.h"

using namespace std;

vector<string> Parse(string str) {

    vector<string> pv;
    int openParen = 0, closeParen = 0;
    for (int i = 0; i < str.size(); ++i) {
        if (str.at(i) == '"') {
            int j = str.find('"', i+1);
            i = j;
        }
        else if (str.at(i) == '(') {
            ++openParen;
        }
        else if (str.at(i) == ')') {
            ++closeParen;
        }
        else if (str.at(i) == '#') {
            break;
        }
    }

    if (openParen != closeParen) {
        cout << "ERROR: extra or missing parenthasis" << endl;
        return vector<string>();
    }

    for (int i = 0; i < str.size(); ++i) {
        if (str.at(i) == '#') {
            break;
        }
        else if (str.at(i) == ' ') {
            pv.push_back(str.substr(0,i));
            str = str.substr(i+1);
            i = -1;
        }
        else if (str.at(i) == ';') {
            if (!pv.empty()) {
                if (pv.back() == ")") {
                    string temp = ";";
                    pv.push_back(temp);
                    if (str.at(i+1) == ' ') {
                        str = str.substr(i+2, str.size());
                    }
                    else {
                        str = str.substr(i+1, str.size());
                    }
                }
                else {
                    pv.push_back(str.substr(0,i));
                    string temp = ";";
                    pv.push_back(temp);
                    if (str.at(i+1) == ' ') {
                        str = str.substr(i+2);
                    }
                    else {
                        str = str.substr(i+1);
                    }
                }
            }
            else {
            string temp = ";";
                pv.push_back(str.substr(0,i));
                pv.push_back(temp);
                str = str.substr(i+2, str.size());
            }
            i = -1;
        }
        else if (str.at(i) == '"') {
            int j = str.find('"', i+1);
            pv.push_back(str.substr(i+1, j-1));
            if (j == str.size() - 1) {
                return pv;
            }
            i = j;
            str = str.substr(i+2);
            i = -1;
        }
        else if (str.at(i) == '(') {
            string temp = "(";
            pv.push_back(temp);
            if (str.at(i+1) == ' ') {
                str = str.substr(i+2, str.size());
            }
            else {
                str = str.substr(i+1, str.size());
            }
            i = -1;
        }
        else if (str.at(i) == ')') {
            if (str.size() != 1) {
                if (str.at(i-1) == ' ') {
                    string temp = ")";
                    pv.push_back(temp);
                    if (str.at(i+1) == ' ') {
                        str = str.substr(i+2, str.size());
                    }
                    else {
                        str = str.substr(i+1, str.size());
                    }
                    i = - 1;
                }
                else {
                    pv.push_back(str.substr(0,i));
                    string temp = ")";
                    pv.push_back(temp);
                    if (i != str.size() - 1) {
                        if (str.at(i+1) == ' ') {
                            str = str.substr(i+2, str.size());
                        }
                        else {
                            str = str.substr(i+1, str.size());
                        }
                        i = - 1;
                    }
                }
            }
            else {
                string temp = ")";
                pv.push_back(temp);
                break;
            }
        }
        else if (i == str.size() - 1) {
            pv.push_back(str);
        }
    }
    
    return pv;
}

int main() {

	vector<string> ListofCommands;
	string userinput;
	while(1) {
		cout << "$ ";
		getline(cin,userinput);
		ListofCommands = Parse(userinput);
        if (!ListofCommands.empty()) {
            Connectors *con = new Connectors(ListofCommands);
            con->execute();
        }
	}
	return 0;
}


