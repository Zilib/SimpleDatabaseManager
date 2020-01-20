#include  "database.h"
#include "Menu.h"

void ShowHeader(Poll* pPoll)
{
	// static header, which will always showed
	std::cout << pPoll->Title << std::endl;
	std::cout << pPoll->Description << std::endl;
}

void ShowPolls(Database* PollDB)
{
	int ArrayIndex{ 0 };
	for(auto& Poll: PollDB->GetPolls())
	{
		if (ArrayIndex + 1 % 5 != 5) { std::cout << ArrayIndex++ << ". " << Poll.Title << "\t"; }
		else if (ArrayIndex + 1 % 5 == 5) { std::cout << ArrayIndex++ << ". " << Poll.Title << std::endl; }
	}
}

void SelectPoll(Database* PollDB,Poll& Poll)
{
	std::cout << std::endl << std::endl;
	std::cout << "Make your choice, which poll you want to answer for\n";

	short int Choose;
	while (std::cin >> Choose && (Choose > PollDB->GetPolls().size() - 1 || Choose < 0)) // TODO check does user input an character
	{
		std::cout << "Bad number\nPlease input number again!: ";
		std::cin.clear();
		std::cin.ignore();
	}
	Poll = PollDB->GetPolls()[Choose];
	PollDB->DestroyPolls(); 
}

void LoadAnswersToVariables(Poll* pPoll)
{
	for (auto* Question : pPoll->Questions)
	{
		Question->ShowContent();
		Question->AnswerForQuestion(); // Load data into variable
	}
}

// Send whenever customer answer for every question.
void SendAnswersToDatabase(Poll* pPoll,Database* DB)
{
	for (auto* Question : pPoll->Questions)
	{
		DB->InsertUserAnswer(Question, pPoll->Id);
	}
}
int main()
{
	Database* MyDB = new Database;
	Menu* pMenu = new Menu;
	Poll Poll;

	::Poll *pPoll{&Poll};
	
	if(MyDB->ConstructObject())
	{
		/// Now database is selected, everything is fine so we can work!
		char InputChoice = pMenu->ChooseChar();
		
		while(InputChoice != '1' && InputChoice != '2')
		{
			InputChoice = pMenu->ChooseChar();
		}
		system("cls");
		if(InputChoice == '1')
		{	
			ShowPolls(MyDB);
			SelectPoll(MyDB,Poll);
			if(MyDB->LoadPollQuestions(Poll.Id, Poll.Questions))
			{
				LoadAnswersToVariables(pPoll);
				SendAnswersToDatabase(pPoll, MyDB);
			}
		}
		else if (InputChoice == '2')
		{
			MyDB->CreatePoll();
		}
	}
	return 0;
}
