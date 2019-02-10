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
	Parse limit(userInput, ';', false, false);
}
