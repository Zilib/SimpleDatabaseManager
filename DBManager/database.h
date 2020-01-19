#pragma once

#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <vector>
#include "Question.h"

struct Poll
{
	// Cannot be undefined
	unsigned short int id;
	std::string Title;
	std::string Description;
	
	// Can be undefined
	std::vector<Question*> Questions;
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

	static void WaitingText(); // Call this function while you are waiting
	
	// Database initialize functions
	bool DatabaseConnect();
	bool SelectDatabase() const;
	bool DoesDatabaseExist() const;
	bool CreateDatabase() const;
	
	// Save to variables
	void CreateQuestion(const unsigned short int RowOrder);
	void CreateQuestionOpen() const;
	void CreateQuestionClose();
	void CreateQuestionAnswers(const unsigned short int AnswerRow);
public:
	Poll *pSelectedPoll{ nullptr };

	// Release a memory
	void DestroyPolls();
	
	bool ConstructObject();
	// Interact with user
	void CreatePoll();

	// Load data from database
	bool LoadPollQuestions(unsigned short int PollId, std::vector<Question*>& Questions) const;
	std::vector<Question*> GetQuestions() const;
	std::vector<Poll> GetPolls() const;
	bool LoadPolls();
	// Insert into database
	void InsertQuestion(std::string& QuestionText,unsigned short int Type) const; // Type 1 = Open Question, 2 = Close Question.Quest order is saved in the pointer, and poll id is saved in the database variable
	void InsertAnswer(std::string& AnswerContent) const;
};

