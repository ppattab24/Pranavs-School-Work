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
