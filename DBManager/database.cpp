#include "database.h"
#include <iostream>
#include <string>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

// To allow work sync, construct is an bool function
bool Database::ConstructObject()
{
	if(DatabaseConnect())
	{
		if (SelectDatabase()) { return true; }
	}
	return false;
}

bool Database::DatabaseConnect()
{
	try
	{
		Driver = get_driver_instance();
		Con = Driver->connect(Host, DBUsername, DBPassword);
		return true;
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error mesage: " << e.what() << std::endl;
		return false;
	}
}

bool Database::SelectDatabase() const
{
	if (!DoesDatabaseExist())
	{
		char Choose;
		std::cout << "Database doesn't exist, would you like to create it? \n \t1.Yes\t2.No\n";
		while(std::cin >> Choose 
			&& (Choose != '1'
			&& Choose != '2'))
		{
			system("cls");
			std::cout << "Incorrect number, please input number again\n";
			std::cout << "Database doesn't exist, would you like to create it? \n \t1.Yes\t2.No\n";
			// Clear buffor, get input again
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<unsigned short>::max(), '\n');
		}
		if (Choose == '2') // That is only one way to leave this place
		{
			std::cout << std::endl << "So i must say goodbye";
			exit(0);
		}
		else if (Choose == '1')
		{
			if(CreateDatabase())
			{
				std::cout << "Database has been created, now you can work with it!\n";
				return true;
			}
		}
	}
	Con->setSchema(DBName);
	return true; // Now database is selected! You can work! We don't want to work async
}

bool Database::DoesDatabaseExist() const
{
	sql::Statement* Stmt = Con->createStatement();
	sql::ResultSet* Result = Stmt->executeQuery("SHOW DATABASES;");

	// Filling array with database names
	while (Result->next())
	{
		if (Result->getString(1) == DBName)
		{
			return true;
		};
	}
	return false;
}

bool Database::CreateDatabase() const
{
	sql::Statement* Stmt = Con->createStatement();
	// Create and use database
	Stmt->execute("CREATE DATABASE " + DBName);
	Con->setSchema(DBName);

	// Create tables
	// Main data here
	Stmt->execute("CREATE TABLE Polls (Id int AUTO_INCREMENT PRIMARY KEY, Title varchar(100), Description varchar(255));");
	Stmt->execute("CREATE TABLE Questions (Id int AUTO_INCREMENT PRIMARY KEY, PollId int, Type int, RowOrder int unsigned, QuestionText varchar(130));");
	Stmt->execute("CREATE TABLE Available_Close_Answers (Id int AUTO_INCREMENT PRIMARY KEY, RowOrder int unsigned, QuestionId int, AnswerText varchar(300))");
	Stmt->execute("CREATE TABLE Close_Answers_Replies (Id int AUTO_INCREMENT PRIMARY KEY, PollId int, QuestionId int, AnswerId int)");
	Stmt->execute("CREATE TABLE Customer_Answers (Id int AUTO_INCREMENT PRIMARY KEY, PollId int, QuestionId int, Answer varchar(255))");

	// Add foreign keys
	Stmt->execute("ALTER TABLE Questions ADD FOREIGN KEY (PollId) REFERENCES Polls(Id)");
	Stmt->execute("ALTER TABLE Customer_Answers ADD FOREIGN KEY (QuestionId) REFERENCES Questions(Id)");
	Stmt->execute("ALTER TABLE Customer_Answers ADD FOREIGN KEY (PollId) REFERENCES Polls(Id)");
	Stmt->execute("ALTER TABLE Available_Close_Answers ADD FOREIGN KEY (QuestionId) REFERENCES Questions(Id)");
	Stmt->execute("ALTER TABLE Close_Answers_Replies ADD FOREIGN KEY (AnswerId) REFERENCES Available_Close_Answers(Id)");
	Stmt->execute("ALTER TABLE Close_Answers_Replies ADD FOREIGN KEY (QuestionId) REFERENCES Questions(Id)");
	Stmt->execute("ALTER TABLE Close_Answers_Replies ADD FOREIGN KEY (PollId) REFERENCES Polls(Id)");

	delete Stmt;
	
	return true; // When every query execute return true, to work synchronic.
}
// Functions which interact with user / DML functions
void Database::PollAnswer()
{
	if(LoadPolls())
	{
		ShowPolls();
		SelectPoll();
	}
}
// So here is something for tomorrow! And weekend
void Database::CreatePoll()
{
	std::string Title, Description;

	std::cout << "Input poll name: ";
	std::cin.ignore();
	std::getline(std::cin,Title);

	std::cout << "Input poll description: ";
	std::cin.ignore();
	std::getline(std::cin,Description);

	sql::PreparedStatement* PreparedStmt = Con->prepareStatement("INSERT INTO Polls (Title,Description) VALUES (?, ?)");
	PreparedStmt->setString(1, Title);
	PreparedStmt->setString(2, Description);
	PreparedStmt->executeUpdate();
	delete PreparedStmt;
	
	sql::Statement* Stmt = Con->createStatement();
	Stmt->execute("Set @LastPollId = LAST_INSERT_ID()");
	delete Stmt;

	unsigned short int QuestionsNumber;

	std::cout << "How many questions would you like to have? ";
	std::cin >> QuestionsNumber;
	while (QuestionsNumber > MaxNumberOfQuestions || QuestionsNumber <= 0 || std::cin.fail())
	{
		std::cout << "Wrong number please input number again!\n";
		std::cout << "How many questions would you like to have? ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin >> QuestionsNumber;
	}
	
	for (int i = 1; i <= QuestionsNumber;)
	{
		CreateQuestion(i++);
	}
}

void Database::InsertQuestion(std::string& QuestionText, unsigned short int Type) const
{
	sql::PreparedStatement* PreparedStmt = Con->prepareStatement("INSERT INTO questions (QuestionText,Type,RowOrder, PollId) VALUES (?,?,?, @LastPollId)");
	PreparedStmt->setString(1, QuestionText);
	PreparedStmt->setInt(2, Type); // Cannot be zero, we are counting from 0, so i have to add one to make of it an sql index
	PreparedStmt->setUInt(3, *pCurrentQuestionRow);
	PreparedStmt->executeUpdate();
	delete PreparedStmt;

	sql::Statement* Stmt = Con->createStatement();
	Stmt->execute("Set @LastQuestionId = LAST_INSERT_ID()"); // Save id for relations
	delete Stmt;
}

void Database::InsertAnswer(std::string& AnswerContent) const
{
	sql::PreparedStatement* PreparedStmt = Con->prepareStatement("INSERT INTO available_close_answers (RowOrder,AnswerText,QuestionId) VALUES (?, ?, @LastQuestionId)");
	PreparedStmt->setInt(1, *pCurrentAnswerRow);
	PreparedStmt->setString(2, AnswerContent);
	PreparedStmt->executeUpdate();
	delete PreparedStmt;
}

void Database::CreateQuestion(const unsigned short int RowOrder)
{
	pCurrentQuestionRow = &RowOrder; 
	system("cls");
	std::cout << "Select question type!\n";
	std::cout << "\t1.Open Question\t2.Close Question\n";

	// Choose type
	char TypeId;
	while(std::cin >> TypeId && ( TypeId != '1' && TypeId != '2'))
	{
		system("cls");
		std::cout << "Wrong number, input a number again!\n\n";

		std::cout << "Select question type!\n";
		std::cout << "\t1.Open Question\t2.Close Question\n";
	}
	// Dependent of user choose create close or open question.
	if(TypeId == '1')
	{
		CreateQuestionOpen();
	}
	else if (TypeId == '2')
	{
		CreateQuestionClose();
	}
}

void Database::CreateQuestionClose()
{
	std::string QuestionContent;
	std::cout << "Input your question content\n";
	std::cin.ignore();
	std::getline(std::cin, QuestionContent);

	InsertQuestion(QuestionContent, 2); // Insert into data

	// Create answers
	std::cout << "How many answers would you like to add?\n";
	unsigned short int NumberOfAnswers;
	std::cin >> NumberOfAnswers;
	while (NumberOfAnswers > MaxNumberOfAnswers || NumberOfAnswers <= 0 || std::cin.fail())
	{
		std::cout << "Wrong number please input number again!\n";
		std::cout << "How many questions would you like to have? ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	for(int i=1; i<=NumberOfAnswers;)
	{
		CreateQuestionAnswers(i++);
	}
}

void Database::CreateQuestionOpen() const
{
	std::string QuestionContent;
	
	system("cls");
	std::cout << "Input your question content\n";
	std::cin.ignore();
	std::getline(std::cin, QuestionContent);

	InsertQuestion(QuestionContent, 1);
}

void Database::CreateQuestionAnswers(const unsigned short int AnswerRow)
{
	pCurrentAnswerRow = &AnswerRow;
	std::string AnswerContent;
	
	std::cout << "Answer number: " << *pCurrentAnswerRow << std::endl;
	std::cout << "Input option: ";
	std::cin.ignore();
	std::getline(std::cin, AnswerContent);

	InsertAnswer(AnswerContent);
}

bool Database::LoadPolls()
{
	sql::Statement* Stmt = Con->createStatement();
	sql::ResultSet* Results = Stmt->executeQuery("SELECT count(id) FROM polls");
	
	bool DoesAnyPollExists{ false };
	while (Results->next())
	{
		DoesAnyPollExists = Results->getInt(1) != 0; // If exists at least 1 poll return true
	}

	if (DoesAnyPollExists)
	{
		Results = Stmt->executeQuery("SELECT Id,Title from polls");

		// Let user choose
		unsigned short int Iterator;
		std::cout << "Choose poll\n";
		Poll *TemporaryPool = new Poll; // Temporary data
		while (Results->next())
		{
			TemporaryPool->id = Results->getUInt(1);
			TemporaryPool->Title = Results->getString(2);
			Polls.push_back(*TemporaryPool); // Save every uploaded data into a local variable
		}
		delete TemporaryPool;
		return true;
	}
	else
	{
		// So let's make an poll only if user want it
		std::cout << "Firstly, you have to create poll, i cannot show you anything\n";
		std::cout << "Would you like to create one poll right now?\n 1.Yes\t2.No\n";
		char Choose;
		while (std::cin >> Choose
			&& (Choose != '1'
				&& Choose != '2'))
		{
			system("cls");
			std::cout << "Incorrect number, please input number again\n";
			std::cout << "Would you like to create one poll right now?\n 1.Yes\t2.No\n";
		}
		if (Choose == '1')
		{
			CreatePoll();
		}
		else if (Choose == '2')
		{
			std::cout << std::endl << "So i must say goodbye";
			exit(0);
		}
		return false;
	}
}

void Database::ShowPolls()
{
	int ArrayIndex = 0;
	for(auto& Poll : Polls)
	{
		if (ArrayIndex + 1 % 5 != 5) { std::cout << ArrayIndex++ << ". " << Poll.Title << "\t"; }
		else if (ArrayIndex + 1 % 5 == 5) { std::cout << ArrayIndex++ << ". " << Poll.Title << std::endl; }
	}
}

void Database::SelectPoll()
{
	std::cout << std::endl;
	std::cout << "Make your choice, which poll you want to answer for\n";

	short int Choose;
	while(std::cin>>Choose && (Choose > Polls.size() - 1 || Choose < 0 )) // TODO check does user input an character
	{
		std::cout << "Bad number\nPlease input number again!: ";
		std::cin.clear();
		std::cin.ignore();
	}
	std::cout << "\nOk, your choice is: " << Polls[Choose].Title;
}
