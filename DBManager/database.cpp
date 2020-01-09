#include "database.h"
#include <jdbc/cppconn/statement.h>

Database::Database()
{
	if(DatabaseConnect())
	{
		SelectDatabase();
	}
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
		if (Choose == '2')
		{
			std::cout << std::endl << "So i must say goodbye";
			exit(0);
		}
		else if (Choose == '1')
		{
			if(CreateDatabase())
			{
				std::cout << "Database has been created, now you can work with it!\n";
				return true; // Now database is selected! You can work! We don't want to work async
			}
		}
	}
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
	std::string Query = "CREATE DATABASE " + DBName;
	Stmt->execute(Query);
	Con->setSchema(DBName);

	// Create tables
	// Main data here
	Stmt->execute("CREATE TABLE Poll (Id int AUTO_INCREMENT PRIMARY KEY, Title varchar(100), Description varchar(255), PublishDate timestamp, Deadline timestamp);");
	Stmt->execute("CREATE TABLE Questions (Id int AUTO_INCREMENT PRIMARY KEY, PollId int, Type int, RowOrder int unsigned, QuestionText varchar(130));");
	Stmt->execute("CREATE TABLE QuestionTypes (Id int AUTO_INCREMENT PRIMARY KEY, TypeName varchar(100))");
	Stmt->execute("CREATE TABLE AvailableCloseAnswers (Id int AUTO_INCREMENT PRIMARY KEY, RowOrder int unsigned, QuestionId int, AnswerText varchar(300))");
	Stmt->execute("CREATE TABLE CloseAnswersReplies (Id int AUTO_INCREMENT PRIMARY KEY, PollId int, QuestionId int, AnswerId int)");
	Stmt->execute("CREATE TABLE CustomerAnswers (Id int AUTO_INCREMENT PRIMARY KEY, PollId int, QuestionId int, Answer varchar(255), AnswerTime timestamp)");

	// Add foreign keys
	Stmt->execute("ALTER TABLE Questions ADD FOREIGN KEY (PollId) REFERENCES Poll(Id)");
	Stmt->execute("ALTER TABLE Questions ADD FOREIGN KEY (Type) REFERENCES QuestionTypes(Id)");
	Stmt->execute("ALTER TABLE CustomerAnswers ADD FOREIGN KEY (QuestionId) REFERENCES Questions(Id)");
	Stmt->execute("ALTER TABLE CustomerAnswers ADD FOREIGN KEY (PollId) REFERENCES Poll(Id)");
	Stmt->execute("ALTER TABLE AvailableCloseAnswers ADD FOREIGN KEY (QuestionId) REFERENCES Questions(Id)");
	Stmt->execute("ALTER TABLE CloseAnswersReplies ADD FOREIGN KEY (AnswerId) REFERENCES AvailableCloseAnswers(Id)");
	Stmt->execute("ALTER TABLE CloseAnswersReplies ADD FOREIGN KEY (QuestionId) REFERENCES Questions(Id)");
	Stmt->execute("ALTER TABLE CloseAnswersReplies ADD FOREIGN KEY (PollId) REFERENCES Poll(Id)");

	delete Stmt;

	return true; // When every query execute return true, to work synchronic.
}


