#include <string>
#include <vector>
#include <iostream>

#pragma once

class Menu
{
public:
	char ChooseChar(); // cin char 
private:
	bool WasIncorrect{ false };
	
	// First text which will appear at the screen
	std::string MainText {"What would you like to do? Give an answer for pool or create another one?" };
	std::string Options[2]{ "Give an answer for existing poll", "Create another one" };
	std::string IncorrectInputText{ "Incorrect input, please try again!" };

	std::string Information[1]{ MainText + "\n1. " + Options[0] + "\t\t2. " + Options[1] + "\n" };
};

