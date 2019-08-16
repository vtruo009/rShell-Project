#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include <vector>
#include "../src/MultiCommand.h"
#include "../src/Oring.h"
#include "../src/Anding.h"
#include "../src/Semicolon.h"
#include "../src/Connectors.h"
#include "../src/Less.h"
#include "../src/GG.h"
#include "../src/Greater.h"
      
using namespace std;  
vector<string> parsing(string str) { 
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
            else { //pv vector is empty
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
TEST(Command, execute) {
	vector<string> test;
	vector<string> test1;
	test1.push_back("hello");
	test.push_back("date");
	MultiCommand *cmd1 = new MultiCommand(test1);
	MultiCommand *cmd = new MultiCommand(test);
	bool executed = cmd->execute();
	bool executeit = cmd1->execute();
	EXPECT_EQ(true, executed);
	EXPECT_EQ(false, executeit);
}

TEST(Command, checkparse) {
	string input = "ls; echo hello";
	vector<string> v;
	v = parsing(input);
	ASSERT_EQ(4,v.size());
	EXPECT_EQ("ls", v.at(0));
	EXPECT_EQ(";", v.at(1));
	EXPECT_EQ("echo", v.at(2));
	EXPECT_EQ("hello",v.at(3));
}


TEST(SubConnect, orTrueandFalse) {
	vector<string> test;
	test.push_back("pwd");
	bool checktrue = true;
	bool checkfalse = false;

	Oring * o = new Oring(checktrue,test);
	Oring * orr = new Oring(checkfalse,test);
	bool executeit = orr->execute();
	bool executed = o->execute();	
	EXPECT_EQ(true, executed);
	EXPECT_EQ(true, executeit);
}	

TEST(SubConnect, andTrueandFalse) {
        vector<string> test;
        test.push_back("pwd");
        bool checktrue = true;
	bool checkfalse = false;
        Anding * a = new Anding(checktrue,test);
	Anding * an = new Anding(checkfalse,test);
        bool executed = a->execute();
	bool executeit = an->execute();
        EXPECT_EQ(true, executed);
	EXPECT_EQ(false,executeit);
}

TEST(SubConnect, SemiTrue) {
        vector<string> test;
        test.push_back("pwd");
        bool checktrue = true;
	bool checkfalse = false;
        Semicolon * s = new Semicolon(checktrue,test);
	Semicolon * se = new Semicolon(checkfalse,test);
        bool executed = s->execute();
	bool executeit = se->execute();
        EXPECT_EQ(true, executed);
	EXPECT_EQ(true, executeit);
}         

TEST(Connectors, execute1) {
	string line = "git branch; hello";
	vector<string> test;
	test = parsing(line);
	Connectors* c = new Connectors(test);
	bool results = c->execute();	
	EXPECT_EQ(false,results);
}   

TEST(Connectors, execute2) {
        string line = "git branch && ls || pwd";
        vector<string> test;
        test = parsing(line);
        Connectors* c = new Connectors(test);
        bool results = c->execute();
        EXPECT_EQ(true,results);
}

TEST(Connectors, execute3) {
	string line = "pwd && echo hallo; goodbye";
	vector<string> test;
	test = parsing(line);
	Connectors *c = new Connectors(test);
	bool results = c->execute();
	EXPECT_EQ(false, results);
}

TEST(Connectors, execute4) {
	string line = "(ls && pwd)";
	vector<string> test;
	test = parsing(line);
	Connectors *c = new Connectors(test);
	bool results = c->execute();
	EXPECT_EQ(true, results);
}

TEST(Connectors, execute5) {
        string line = "(ls && pwd || (echo b; echo c))";
        vector<string> test;
        test = parsing(line);
        Connectors *c = new Connectors(test);
        bool results = c->execute();
        EXPECT_EQ(true, results);
}

TEST(Connectors, execute6) {
        string line = "(ls; (echo a && pwd) || (echo c || hi))";
        vector<string> test;
        test = parsing(line);
        Connectors *c = new Connectors(test);
        bool results = c->execute();
        EXPECT_EQ(true, results);
}

TEST(Connectors, execute7) {
        string line = "echo a || (echo b && echo c) && (pwd || ls)";
        vector<string> test;
        test = parsing(line);
        Connectors *c = new Connectors(test);
        bool results = c->execute();
        EXPECT_EQ(true, results);
}

TEST(Connectors, execute8) {
	string line = "test -f src/main.cpp && echo path exists";
        vector<string> test;
        test = parsing(line);
        Connectors *c = new Connectors(test);
        bool results = c->execute();
        EXPECT_EQ(true, results);
}

TEST(Connectors, execute9) {
	string line = "[ src/main.cpp ]";
        vector<string> test;
        test = parsing(line);
        Connectors *c = new Connectors(test);
        bool results = c->execute();
        EXPECT_EQ(true, results);
}

TEST(Connectors, execute10) {
        string line = "[ src/main.cpp ] && test -e src/nameless.h ";
        vector<string> test;
        test = parsing(line);
        Connectors *c = new Connectors(test);
        bool results = c->execute();
        EXPECT_EQ(false, results);
}

TEST(Connectors, execute11) {
        string line = "[ - d images/ ] || pwd && echo a ";
        vector<string> test;
        test = parsing(line);
        Connectors *c = new Connectors(test);
        bool results = c->execute();
        EXPECT_EQ(true, results);
}

TEST(SubConnect, hash) {
	string line = "#git status && ls || pwd";
        vector<string> test;
        test = parsing(line);
        EXPECT_EQ(vector<string>(),test);
	string line2 = "pwd && git branch; echo hello #bye";
	vector<string> test1;
	test1 = parsing(line2);
	Connectors *c = new Connectors(test1);
	bool results = c->execute();
	EXPECT_EQ(true, results);
}

TEST(SubConnect, parenthesis) {
	string line = "git branch && echo hello)";
	vector<string> test;
	test = parsing(line);
	EXPECT_EQ(vector<string>(), test);
}

TEST(SubConnect, lessoperator) {
	string line = "cat < names.txt";
	vector<string>test;
	test = parsing(line);
	Connectors *c = new Connectors(test);
	bool results = c->execute();
	EXPECT_EQ(false, results);

	string line2 = "cat < outfile";
	vector<string>test2;
        test2 = parsing(line);
        Connectors *c1 = new Connectors(test2);
        bool result1 = c1->execute();
        EXPECT_EQ(false, result1);
}

TEST(SubConnect, greater) {
	string line = "echo hello > test.txt";
        vector<string>test;
        test = parsing(line);
        Connectors *c = new Connectors(test);
        bool results = c->execute();
        EXPECT_EQ(false, results);
	
	string line2 = "pwd > outfile";
        vector<string>test2;
        test2 = parsing(line);
        Connectors *c1 = new Connectors(test2);
        bool result1 = c1->execute();
        EXPECT_EQ(false, result1);

}

TEST(SubConnect, ggreater) {
	string line = "echo yes >> test.txt";
	vector<string>test;
        test = parsing(line);
        Connectors *c = new Connectors(test);
        bool results = c->execute();
        EXPECT_EQ(false, results);
	
	string line2 = "echo works >> outfile";
        vector<string>test2;
        test2 = parsing(line);
        Connectors *c1 = new Connectors(test2);
        bool result1 = c1->execute();
        EXPECT_EQ(false, result1);
}
int main() { 
       ::testing::InitGoogleTest();
       return RUN_ALL_TESTS();
}  
