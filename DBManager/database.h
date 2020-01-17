#pragma once

#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <vector>


struct Poll
{
	unsigned short int id;
	std::string Title;
};

class Database
{
private:
	// Database config variables
	const std::string Host{ "127.0.0.1:3306" };
	const std::string DBUsername{ "root" };
	const std::string DBPassword{ "OcJYGs9zAYvw70180lZn" };

	std::vector<std::string>BasesName; // Show databases result
	std::string DBName{ "pollapp" };
	std::vector<Poll> Polls;
	// Library variables
	sql::Driver* Driver;
	sql::Connection* Con;
	
	
	unsigned short int MaxNumberOfQuestions{ 64 };
	unsigned short int MaxNumberOfAnswers{ 64 };
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
	void PollAnswer();
	void CreatePoll();
	// Save to variables
	void CreateQuestion(const unsigned short int RowOrder);
	void CreateQuestionOpen() const;
	void CreateQuestionClose();
	void CreateQuestionAnswers(const unsigned short int AnswerRow);
	// Operations with vectors
	void ShowPolls();
	void SelectPoll();
	// Load data from database
	bool LoadPoll();
	bool LoadPolls();
	// Insert into database
	void InsertQuestion(std::string& QuestionText,unsigned short int Type) const; // Type 1 = Open Question, 2 = Close Question.Quest order is saved in the pointer, and poll id is saved in the database variable
	void InsertAnswer(std::string& AnswerContent) const;
};

