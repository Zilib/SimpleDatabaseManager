#pragma once

#include <string>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/exception.h>

class Database
{
public:
	Database();
private:
	// Database config variables
	const std::string Host{ "127.0.0.1q:3306" };
	const std::string DBUsername{ "root" };
	const std::string DBPassword{ "OcJYGs9zAYvw70180lZn" };
	const std::string DBName{ "cpp" };
	// Library variables
	sql::Driver* Driver;
	sql::Connection* Con;
	sql::Statement* Stmt;

	// Try to connect with database
	bool OpenDatabase();
	void SelectDatabase() const;
};
