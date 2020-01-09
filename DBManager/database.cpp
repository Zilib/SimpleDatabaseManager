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

void Database::SelectDatabase() 
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
			CreateDatabase();
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
		std::cout << Result->getString(1)<<std::endl;
		if (Result->getString(1) == DBName)
		{
			return true;
		};
	}
	return false;
}

void Database::CreateDatabase()
{
	sql::Statement* Stmt = Con->createStatement();
	// Create and use database
	std::string Query = "CREATE DATABASE " + DBName;
	Stmt->execute(Query);
	Stmt->execute("USE "+ DBName);

	// Create tables
	Stmt->execute("CREATE TABLE Poll (Id int AUTO_INCREMENT PRIMARY KEY, Title varchar(100), Description varchar(255), PublishDate timestamp, Deadline timestamp);");
	Stmt->execute("CREATE TABLE OpenQuestions (id int AUTO_INCREMENT PRIMARY KEY, PollId int, Type int, RowOrder int unsigned, Content varchar(130));");
	Stmt->execute("CREATE TABLE Types (Id int AUTO_INCREMENT PRIMARY KEY, TypeName varchar(100))");
	Stmt->execute("CREATE TABLE CloseAnswers (Id int AUTO_INCREMENT PRIMARY KEY, RowOrder int unsigned, QuestionId int, Content varchar(300))");
	Stmt->execute("CREATE TABLE CustomerAnswers (Id int AUTO_INCREMENT PRIMARY KEY, PollId int, QuestionId int, Answer varchar(255), AnswerTime timestamp)");
	
}


