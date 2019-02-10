#ifndef MANAGER_H
#define MANAGER_H

#include <string>
#include <cstring>
#include <stack>
#include <queue>
#include <unistd.h>
#include <cstdlib>
#include <climits>
#include <cassert>
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
