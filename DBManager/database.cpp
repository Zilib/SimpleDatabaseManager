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
		if (SelectDatabase())
		{
			if (GetQuestionTypes()) { return true; }
		}
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
	Stmt->execute("CREATE TABLE QuestionTypes (Id int AUTO_INCREMENT PRIMARY KEY, TypeName varchar(100))");
	Stmt->execute("CREATE TABLE AvailableCloseAnswers (Id int AUTO_INCREMENT PRIMARY KEY, RowOrder int unsigned, QuestionId int, AnswerText varchar(300))");
	Stmt->execute("CREATE TABLE CloseAnswersReplies (Id int AUTO_INCREMENT PRIMARY KEY, PollId int, QuestionId int, AnswerId int)");
	Stmt->execute("CREATE TABLE CustomerAnswers (Id int AUTO_INCREMENT PRIMARY KEY, PollId int, QuestionId int, Answer varchar(255), AnswerTime timestamp)");

	// Add foreign keys
	Stmt->execute("ALTER TABLE Questions ADD FOREIGN KEY (PollId) REFERENCES Polls(Id)");
	Stmt->execute("ALTER TABLE Questions ADD FOREIGN KEY (Type) REFERENCES QuestionTypes(Id)");
	Stmt->execute("ALTER TABLE CustomerAnswers ADD FOREIGN KEY (QuestionId) REFERENCES Questions(Id)");
	Stmt->execute("ALTER TABLE CustomerAnswers ADD FOREIGN KEY (PollId) REFERENCES Polls(Id)");
	Stmt->execute("ALTER TABLE AvailableCloseAnswers ADD FOREIGN KEY (QuestionId) REFERENCES Questions(Id)");
	Stmt->execute("ALTER TABLE CloseAnswersReplies ADD FOREIGN KEY (AnswerId) REFERENCES AvailableCloseAnswers(Id)");
	Stmt->execute("ALTER TABLE CloseAnswersReplies ADD FOREIGN KEY (QuestionId) REFERENCES Questions(Id)");
	Stmt->execute("ALTER TABLE CloseAnswersReplies ADD FOREIGN KEY (PollId) REFERENCES Polls(Id)");

	delete Stmt;
	
	sql::PreparedStatement* PreparedStatement = Con->prepareStatement("INSERT INTO QuestionTypes (TypeName) VALUES (?), (?)");
	PreparedStatement->setString(1, "Open Question");
	PreparedStatement->setString(2, "Close Question");
	PreparedStatement->executeUpdate();

	delete PreparedStatement;
	
	return true; // When every query execute return true, to work synchronic.
}

bool Database::GetQuestionTypes() 
{
	sql::Statement* Stmt = Con->createStatement();
	sql::ResultSet* Result = Stmt->executeQuery("SELECT TypeName FROM questiontypes ORDER BY Id ASC");

	// Get every available question types
	while(Result->next())
	{
		QuestionTypes.push_back(Result->getString(1));
	}
	// Now you can work
	return true;
}

// Functions which interact with user / DML functions
void Database::SelectPoll()
{
	sql::Statement* Stmt = Con->createStatement();
	sql::ResultSet* Results = Stmt->executeQuery("SELECT count(id) FROM polls");

	bool DoesAnyPollExists{ false };
	while (Results->next())
	{
		DoesAnyPollExists = Results->getInt(1) != 0;
	}

	if (DoesAnyPollExists)
	{
		Results = Stmt->executeQuery("SELECT Title from polls");
		std::vector<std::string>PollNames;

		// Let user choose
		unsigned short int Iterator;
		std::cout << "Choose poll\n";
		while (Results->next())
		{
			PollNames.push_back(Results->getString(1));
			std::cout << PollNames.size() << ". " << Results->getString(1)<<std::endl;
		}
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
	while (QuestionsNumber > MaxNumberOfQuestions || QuestionsNumber <= 0 || std::cin.fail()) // TODO FIX input protect
	{
		std::cout << "Wrong number please input number again!\n";
		std::cout << "How many questions would you like to have? ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin >> QuestionsNumber;
	}
	std::cout << "ELOELO";
	for (int i = 1; i <= QuestionsNumber;)
	{
		CreateQuestion(i++);
	}
}

void Database::CreateQuestion(unsigned short int RowOrder)
{
	system("cls");
	std::string QuestionContent;
	std::cout << "Select question type!\n";

	for(int i=0;i<QuestionTypes.size();i++)
	{
		std::cout << i << ". " << QuestionTypes[i] << '\t';
	}
	std::cout << std::endl;

	int TypeId;
	std::cin >> TypeId;
	while (std::cin.fail() || TypeId > QuestionTypes.size()) // TODO FIX input protect
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<unsigned short>::max(), '\n');
		system("cls");
		std::cout << "Wrong number choose again!\n";
		
		for (int i = 0; i < QuestionTypes.size(); i++)
		{
			std::cout << i << ". " << QuestionTypes[i] << '\t';
		}
		std::cin >> TypeId;
	}
	std::cout << "Okey good choice...\n";
	std::cout << "Please insert question content \n";
	
	std::cin.ignore();
	std::getline(std::cin, QuestionContent);
	
	sql::PreparedStatement* PreparedStmt = Con->prepareStatement("INSERT INTO questions (QuestionText,Type,RowOrder, PollId) VALUES (?,?,?, @LastPollId)");
	PreparedStmt->setString(1, QuestionContent);
	PreparedStmt->setInt(2, ++TypeId); // Cannot be zero, we are counting from 0, so i have to add one to make of it an sql index
	PreparedStmt->setUInt(3, RowOrder);
	PreparedStmt->executeUpdate();
	//delete PreparedStmt;
}

