#pragma once

#include <string>
#include <iostream>

enum QuestionType {Close, Open};

class Question
{
public:
	Question(unsigned int Id, unsigned int RowOrder, std::string Content,QuestionType Type);
protected:
	virtual void SetVariables(unsigned int Id, unsigned int RowOrder, std::string Content, QuestionType Type);
	unsigned int Id;
	unsigned short int RowOrder;
	QuestionType QType;
	std::string Content;
};

class OpenQuestion :
	public Question
{
public:
	OpenQuestion(unsigned int Id, unsigned int RowOrder, std::string Content, QuestionType Type);
};

class CloseQuestion :
	public Question
{
public:
	CloseQuestion(unsigned int Id, unsigned int RowOrder, std::string Content, QuestionType Type);
private:
	LoadAnswers();
};