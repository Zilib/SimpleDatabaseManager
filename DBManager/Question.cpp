#include "Question.h"



Question::Question(unsigned int Id, unsigned int RowOrder, std::string Content)
{
	this->Id = Id;
	this->RowOrder = RowOrder;
	this->Content = Content;
}

