#include "Question.h"

Question::Question(unsigned Id, unsigned RowOrder, std::string Content, QuestionType Type)
{
	Question::SetVariables(Id, RowOrder, std::move(Content),Type);
}

void Question::SetVariables(unsigned Id, unsigned RowOrder, std::string Content, QuestionType Type)
{
	this->Id = Id;
	this->RowOrder = RowOrder;
	this->Content = std::move(Content);
	this->QType = Type;
}

OpenQuestion::OpenQuestion(unsigned Id, unsigned RowOrder, std::string Content, QuestionType Type)
	: Question(Id, RowOrder, std::move(Content),Type) 
{
	std::cout << "Implemented open";
}

CloseQuestion::CloseQuestion(unsigned Id, unsigned RowOrder, std::string Content, QuestionType Type)
	: Question(Id, RowOrder, std::move(Content),Type)
{
	std::cout << "Implemented close";
}


