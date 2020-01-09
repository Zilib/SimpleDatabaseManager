#pragma once

#include <string>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <vector>

class Database
{
public:
	Database();
private:
	// Database config variables
	const std::string Host{ "127.0.0.1:3306" };
	const std::string DBUsername{ "root" };
	const std::string DBPassword{ "OcJYGs9zAYvw70180lZn" };

	std::vector<std::string>BasesName; // Show databases result
	std::string DBName{ "pollapp" };
	// Library variables
	sql::Driver* Driver;
	sql::Connection* Con;

	// Database initialize functions
	bool DatabaseConnect();
	void SelectDatabase();
	bool DoesDatabaseExist() const;
	void CreateDatabase();
	
};
