#pragma once

#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <vector>

class Database
{
private:
	// Database config variables
	const std::string Host{ "127.0.0.1:3306" };
	const std::string DBUsername{ "root" };
	const std::string DBPassword{ "OcJYGs9zAYvw70180lZn" };

	std::vector<std::string>BasesName; // Show databases result
	std::string DBName{ "pollapp" };
	// Library variables
	sql::Driver* Driver;
	sql::Connection* Con;

	unsigned short int MaxNumberOfQuestions{ 64 };
	const unsigned short int* pCurrentQuestionRow{ nullptr }; 
	const unsigned short int* pCurrentAnswerRow{ nullptr }; 
	
	// Database initialize functions
	bool DatabaseConnect();
	bool SelectDatabase() const;
	bool DoesDatabaseExist() const;
	bool CreateDatabase() const;
public:
	bool ConstructObject();
	// Interact with user
	void SelectPoll();
	void CreatePoll();
	void CreateQuestion(const unsigned short int RowOrder);
	void CreateQuestionOpen();
	void CreateQuestionClose();
	void QuestionAvailableAnswers();
};
