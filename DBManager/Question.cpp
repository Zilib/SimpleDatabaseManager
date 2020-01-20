#include "Question.h"

Question::Question(unsigned int Id, std::string Content, QuestionType Type)
{
	this->Id = Id;
	this->Content = std::move(Content);
	this->QType = Type;
}

void Question::ShowContent()
{
	system("cls");
	std::cout << Content << std::endl;
}

void OpenQuestion::AnswerForQuestion()
{
	std::cout << "Input your answer\n";
	std::cin.ignore();
	std::getline(std::cin, CustomerAnswer);
}

void CloseQuestion::ShowAnswers()
{
	int i{ 1 };
	for(auto& Answer : Answers)
	{
		std::cout <<i++<<". "<< Answer.Content<<std::endl;
	}
}

void CloseQuestion::SelectAnswer()
{
	unsigned short int Choose;
	std::cout << "Make your choice!\n";
	std::cin >> Choose;

	while(Choose > Answers.size() || Choose <= 0)
	{
		system("cls");
		ShowAnswers();
		std::cout << "Incorrect number, please input number again!\n";
		std::cin >> Choose;
	}
	// Counting is starting from 1, so we have to -1 
	pSelectedAnswer = &Answers[Choose - 1];
}

// Let user give an answer for poll
void CloseQuestion::AnswerForQuestion()
{
	ShowAnswers();
	SelectAnswer();
}

bool CloseQuestion::LoadAnswers(sql::Connection* Con, const unsigned short int Questionid)
{
	sql::PreparedStatement* PreparedStmt = Con->prepareStatement("SELECT Id,AnswerText FROM available_close_answers WHERE QuestionId = ? ORDER BY RowOrder ASC");
	PreparedStmt->setUInt(1, Questionid);

	sql::ResultSet* Results = PreparedStmt->executeQuery();

	Answer tAnswer;
	while(Results->next())
	{
		tAnswer.Id = Results->getUInt(1);
		tAnswer.Content = Results->getString(2);
		Answers.push_back(tAnswer);
	}

	return true;
}




