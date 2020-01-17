#pragma once

#include <string>

class Question
{
	Question(unsigned int Id, unsigned int RowOrder, std::string Content);
protected:
	unsigned int Id;
	unsigned short int RowOrder;
	std::string Content;
};

