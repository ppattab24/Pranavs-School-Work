#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <string>
#include <cstring>
#include <stack>
#include queue>
#include <unistd.h>
#include <cstdlib>
#include <climits>
#inlcude <cassert>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys.wati.h>
#include <iostream>

using namespace std;

class Manager {
     public:
     void run();
     
     private:
     void execute(string );
     void parse(char*, char**);
};
