#include  "database.h"

int main()
{
	Database* MyDB = new Database;

	if(MyDB->ConstructObject())
	{
		// Now database is selected, everything is fine so we can work!
		std::cout << "What would you like to do? Give an answer for pool or create another one?\n";
		std::cout << "1. Give an answer for existing pool\t\t 2.Create another one\n";
		
		char Choose;
		while(std::cin>>Choose && !(Choose == '1' || Choose == '2'))
		{
			system("CLS");
			std::cout << "Incorrect input, press your number again!\n";
		}
		system("CLS");
		if (Choose == '1')
		{
			MyDB->SelectPoll();
		}
		else if (Choose == '2')
		{
			MyDB->CreatePoll();
		}
	}
	return 0;
}
