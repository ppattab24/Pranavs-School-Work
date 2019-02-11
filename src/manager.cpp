#include "../header/manager.h"

void Manager::run() 
{
	char userInput[1024];

	while(true)
	{
		memset(userInput, 0, sizeof(userInput));
		cout << "$ ";
		cin.getline(userInput, 1024);
		userInput[cin.gcount()] = '\0';
		cout << endl;
	}
	Token userLine;
	queue<Token> delimited_token_queue, main_token_queue;
	Parse firstParse(userInput, ';', false, false);
	
	while(!limit.done())
	{
		limit >> userLine;

		Parse secondParse(userLine.toString(), ' ', true, true);
		secondParse.compressContents();
		secondParse.reinitializeStates();

		while(!secondParse.done())
		{
			Token thisInput;
			secondParse >> thisInput;
			delimited_token_queue.push(thisInput);
		}

		// Parenthesis check here

		ShuntingYard output(delimited-token_queue);
		queue<Token> parsedQueue = output.getReversePolish();
		evalPostFix(parsedQueue);
		clearQueue(delimited_token_queue);
		cout << endl;
	}
}

void Manager::execute(string command_string) {
	
	char* cString = str_to_char(command_string);
	char* command[64];
	memset(command, 0, sizeof(command));
	
	parse(command_string, command);
	
	execute(command);
	
	delete[] cString;
	
	
}

void Manager::execute(char** command) {
	
	pid_t process_id;	
	int status;
	
	if (equals(*command, "exit", false)) {
		exit(0);
	}
	
	if ((process_id = fork()) < 0) {
		exit(1)
	}
	else if (process_id == 0) {
		if (execvp(*command, command) < 0) {
			cerr<< "Command did not execute:failed" << endl;
			worked = false;
		}
		else {
			worked = true;	
		}
	}
	else {
		worked = true;
		while (wait(&status) != process_id);
		
		if (WEXITSTATUS(status)) {
			worked = false;
		}
	}
	
	
}
