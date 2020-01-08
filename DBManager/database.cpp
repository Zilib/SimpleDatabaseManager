#include "database.h"

Database::Database()
{
	if(OpenDatabase())
	{
		SelectDatabase();
	}
}

bool Database::OpenDatabase()
{
	try
	{
		Driver = get_driver_instance();
		Con = Driver->connect(Host, DBUsername, DBPassword);
		return true;
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error mesage: " << e.what() << std::endl;
		return false;
	}
}

void Database::SelectDatabase() const
{
	Con->setSchema(DBName);
}


