#pragma once
#include "Question.h"
#include <string>

struct Answer
{
	unsigned int Id; // Answer id
	unsigned int RowOrder;
	std::string Content;
};

class CloseQuestion :
	public Question
{
public:
	CloseQuestion();
};

