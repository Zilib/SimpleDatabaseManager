#pragma once

#include <string>
#include <iostream>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <vector>

enum QuestionType {Close, Open};

struct Answer
{
	unsigned short int Id;
	std::string Content;
};

class Question
{
public:
	Question(unsigned int Id, std::string Content,QuestionType Type);
	void ShowContent(); // cout question text
	virtual void AnswerForQuestion() = 0;
	QuestionType QType;
protected:
	unsigned int Id;
	unsigned short int RowOrder;
	std::string Content;
};

class OpenQuestion :
	virtual public Question
{
public:
	OpenQuestion(unsigned int Id, std::string Content, QuestionType Type)
		: Question(Id, std::move(Content), Type) {};
	void AnswerForQuestion() override;
private:
	std::string CustomerAnswer;
};

class CloseQuestion :
	virtual public Question
{
public:
	CloseQuestion(unsigned int Id, std::string Content, QuestionType Type)
		: Question(Id, std::move(Content), Type) {};

	void AnswerForQuestion() override;
	bool LoadAnswers(sql::Connection* Con, const unsigned short int Questionid);
	void SelectAnswer();
private:
	void ShowAnswers();
	std::vector<Answer> Answers;
	unsigned short int* pSelectedAnswerId{ nullptr };
};