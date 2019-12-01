#include <iostream>
#include <string>
#include "mysql_connection.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

using namespace std;
using namespace sql;

// Defining Constant Variables
#define SERVER "localhost"
#define USER "root"
#define PASSWORD "admin"
#define DATABASE "test"

//These are global variables so that they can be easily used in the various option functions
Driver *driver;
Connection *con;
Statement *stmt;
ResultSet  *res;
PreparedStatement *prep_stmt;

void option1()
{
	stmt = con->createStatement(); //creates/establishes a connection
	bool flag = false;
	int userID;
	do {
		cout << "Enter the ID of an instructor: ";
		cin >> userID;
		prep_stmt = con->prepareStatement("SELECT * FROM instructor WHERE id = (?)"); //query that selects all attributes from instructors where the id matches the user entered ID
		prep_stmt->setInt(1, userID); //prep_stmt is used to replace the ? with the entered ID
		res = prep_stmt->executeQuery(); //executes the query
		while (res->next()) { //only runs when there is a tuple to print out
			if (res->getInt("ID") == userID)
			{ //prints out the info
				cout << "ID: " << res->getInt("id") << "\nName: " << res->getString("name") << "\nDepartment: " << res->getString("dept_name") << endl;
				flag = true;
			}
		}
		if (flag == false) //flag will be false if the above while loop never runs, AKA if the ID doesn't exist in the table
			cout << "That ID is not in the database, please try again." << endl;
	} while (flag == false); //runs until a valid ID is entered
}

void option2()
{
	stmt = con->createStatement();
	bool flag = false;
	string deptName;
	cin.ignore();
	do {
		cout << "Enter the name of the department: ";
		getline(cin, deptName);
		prep_stmt = con->prepareStatement("SELECT * FROM instructor NATURAL JOIN department WHERE dept_name = (?)");
		prep_stmt->setString(1, deptName); //these statements select all attributes from department and instructor (naturally joined) where the department name is equal to that entered by the user
		res = prep_stmt->executeQuery();
		while (res->next()) { //runs through all the tuples
				cout << "Name: " << res->getString("name") <<  endl;
				flag = true;
		}
		if (flag == false) //this runs when the above while loop doesn't run at all, AKA the entered user department doesn't match one in the database
			cout << "That department name is not in the database, please try again." << endl;
		else {
			prep_stmt = con->prepareStatement("SELECT building, budget FROM department WHERE dept_name = (?)");
			prep_stmt->setString(1, deptName); //once a valid department is entered, these statements use a query to gather the building & budget of the user entered department
			res = prep_stmt->executeQuery();
			res->next(); //points res to the correct tuple
			cout << "Location: " << res->getString("building") << "\nBudget: $" << res->getInt("budget") << endl;	
		}
	} while (flag == false);
}

void option3()
{
	stmt = con->createStatement();
	bool flag = false, flag2 = false;
	int newID;
	string newName;
	string deptName;
	do {
		res = stmt->executeQuery("SELECT * FROM instructor"); //gets the entire instructor table
		cout << "Enter id: "; //asks user for an ID
		cin >> newID;
		while (res->next()) {
			if (res->getInt("id") == newID)
			{ //if the user entered ID is found, then, the user will have to try again, as, IDs cannot be repeated
				flag = true;
			}
		}
		if (flag == true)
		{ //in this case, the user entered ID is already in use
			flag2 = false; //sets flag2 to false
			flag = false; //resets the flag
		}
		else
		{
			flag2 = true; //sets flag to true
			flag = false; //resets flag
		}
		if (flag2 == false) 
			cout << "That ID is already in the database, please try again." << endl;
	} while (flag2 == false);
	cin.ignore();
	do {
		cout << "Enter department name: ";
		getline(cin, deptName); //the user enters the department they want
		res = stmt->executeQuery("SELECT * FROM department");
		while (res->next())
		{ //checks that the department name is actually is in use
			if (res->getString("dept_name") == deptName)
				flag = true;
		}
		if (flag == false) //if the department is not in the database
			cout << "That department name is not already in the database, please try again." << endl;
	} while (flag == false);
	cout << "Enter name: ";
	getline(cin, newName); //the user enters the name they want for the new instructor
	prep_stmt = con->prepareStatement("INSERT INTO instructor(id, name, dept_name) VALUES (?,?,?)");
	prep_stmt->setInt(1, newID); //these prepared statements inserts into the instructor table the values that the user wants for the new instructor
	prep_stmt->setString(2, newName);
	prep_stmt->setString(3, deptName);
	prep_stmt->execute();
	cout << "Instructor successfully added to the database" << endl;
}

int main()
{
	try {
		// Create a connection
		driver = get_driver_instance();
		con = driver->connect(SERVER, USER, PASSWORD);
		
		//establishes a connection
		stmt = con->createStatement();

		stmt->execute("DROP DATABASE IF EXISTS " DATABASE);
		stmt->execute("CREATE DATABASE " DATABASE);
		stmt->execute("USE " DATABASE);

		stmt->execute("DROP TABLE IF EXISTS department");
		stmt->execute("CREATE TABLE department(dept_name VARCHAR(25), building VARCHAR(25), budget INT, PRIMARY KEY (dept_name))"); //creates department table, dept_name, building, budget, with the primary key being dept_name
		stmt->execute("DROP TABLE IF EXISTS instructor");
		stmt->execute("CREATE TABLE instructor(id INT, name VARCHAR(50), dept_name VARCHAR(25) NOT NULL, PRIMARY KEY (id))"); //creates instructor table, id, name, dept_name, id is the primary key

		stmt->execute("INSERT INTO instructor(id, name, dept_name) VALUES (1001,'Robert Smith','CMPT')"); //inserts default values from the problem statement
		stmt->execute("INSERT INTO instructor(id, name, dept_name) VALUES (1002,'Natasha Anderson','MATH')");
		stmt->execute("INSERT INTO instructor(id, name, dept_name) VALUES (1003,'James Nassimi','BIOL')");
		stmt->execute("INSERT INTO instructor(id, name, dept_name) VALUES (1004,'Guiling Wei','CMPT')");
		stmt->execute("INSERT INTO instructor(id, name, dept_name) VALUES (1005,'Mary Harnett','BIOL')");
		stmt->execute("INSERT INTO instructor(id, name, dept_name) VALUES (1006,'David Ochs','CMPT')");

		stmt->execute("INSERT INTO department(dept_name, building, budget) VALUES ('CMPT','RLC',75000)");
		stmt->execute("INSERT INTO department(dept_name, building, budget) VALUES ('MATH','RLC',55000)");
		stmt->execute("INSERT INTO department(dept_name, building, budget) VALUES ('BIOL','LEO',60000)");

		stmt->execute("ALTER TABLE instructor ADD FOREIGN KEY (dept_name) REFERENCES department (dept_name)"); //adds the foreign key constraint to the instructor table

		int input; //Used​ ​to​ ​enter​ ​a​ ​desired​ ​action​ ​that​ ​corresponds to​ ​the​ ​entered​ ​integer
		while (true)
		{
			do{
				cout << "1.) Enter the instructor ID and I will provide you with the name of the instructor, affiliated department and the location of that department." << endl;
				cout << "2.) Enter the department name and I will provide you with the location, budget and names of all instructors that work for the department." << endl;
				cout << "3.) Insert a record about a new instructor." << endl;
				cout << "4.) EXIT" << endl << "Enter your choice: ";
				cin >> input;
				} while (input < 1 && input > 4);
			if (input == 1)
			{
				option1();
			}
			else if (input == 2)
			{
				option2();
			}
			else if (input == 3)
			{
				option3();
			}
			else if (input == 4)
			{
				return 0;//exits​ ​the​ ​program​ ​immediately
			}
			else
				cout << "ERROR: Try again\n";
		}
		//deletes pointer variables
		delete res;
		delete stmt;
		delete con;
		delete prep_stmt;
	}

	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	return 0;
}
