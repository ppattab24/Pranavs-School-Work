#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdlib.h>
#include<iostream>

using namespace std;

void test() {
	    const int ITERATIONS = 5;
	    for (int i = 0; i < ITERATIONS; i++) {
		sleep(rand() % 4);
		cout << "Done " << i << " passes" << endl;
	    }
}


int main() {
	    
	    cout << "I am ";
	    cout << (int) getpid() << endl << endl;
		    
	    pid_t pid = fork();
	    // srand((int) pid);
	    cout << "Initiating fork process\n" << endl;
	    cout << "fork returned: ";
	    cout << (int) pid;
 				    
	    if (pid < 0) {
	        perror("\nFork failed");
	    }					       
	        
	    if (pid == 0) {
	        cout << "\nI am the child with pid ";
	        cout << (int) pid << endl << endl;
                cout << "Initiating test.\n" << endl;
		test();
		cout << "I am the child, with pid " << (int)pid << ", I am now terminating....\n" << endl;
	   	exit(0);
	    }	
						        
	   cout << "\n\nI am the parent, with pid " << (int) pid << " waiting for child to end.\n";
	   wait(NULL);
	   cout << "\nI am the parent, with pid " << (int)pid << ", I am terminating. Return to command console.\n" << endl;
		  	
		    
	    return 0;
}

