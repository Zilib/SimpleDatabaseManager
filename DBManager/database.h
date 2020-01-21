#pragma once

#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <vector>
#include "Question.h"

struct Poll
{
	// Cannot be undefined
	unsigned short int Id;
	std::string Title;
	std::string Description;
	
	// Can be undefined
	std::vector<Question*> Questions; // Polymorhpics class
};

class Database
{
private:
	// Database config variables
	const std::string Host{ "127.0.0.1:3306" };
	const std::string DBUsername{ "root" };
	const std::string DBPassword{ "OcJYGs9zAYvw70180lZn" };
	std::string DBName{ "pollapp" };

	std::vector<std::string>BasesName; // Show databases result
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
	
	// Save to variables
	void CreateQuestion(const unsigned short int RowOrder);
	void CreateQuestionOpen() const;
	void CreateQuestionClose();
	void CreateQuestionAnswers(const unsigned short int AnswerRow);

	
public:
	bool ConstructObject();

	Poll *pSelectedPoll{ nullptr };

	// Clear polls array for release memory
	void DestroyPolls();
	
	// Interact with user
	void CreatePoll();

	// Load data from database
	bool LoadPollQuestions(unsigned short int PollId, std::vector<Question*>& Questions) const;
	bool LoadPolls();

	// Insert into database
	void InsertQuestion(std::string& QuestionText,unsigned short int Type) const; // Type 1 = Open Question, 2 = Close Question.Quest order is saved in the pointer, and poll id is saved in the database variable
	void InsertUserAnswer(Question* pQuestion,const unsigned short int PollId) const;
	void InsertAvailableAnswer(std::string& AnswerContent) const;

	/// Getters
	std::vector<Poll> GetPolls() const;
	std::vector<Question*> GetQuestions() const; // Get questions of selected poll
};

