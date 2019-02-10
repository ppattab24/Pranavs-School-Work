#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
	cout << "The exec file is starting." << endl;
       	cout << "The exec file pid is " << getpid() << "\n" << endl;

	char *args[] = {"./testFork" ,NULL};
	execvp("./testFork", args);
	return 0;
}
