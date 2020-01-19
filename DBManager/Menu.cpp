#include "Menu.h"

char Menu::ChooseChar()
{
	if (WasIncorrect)
	{
		system("CLS");
		std::cout << IncorrectInputText << std::endl;
	}
	if (WasIncorrect == false) { WasIncorrect = true; }
	std::cout << Information[0];
	char Choose;
	std::cin >> Choose;
	return Choose;
}