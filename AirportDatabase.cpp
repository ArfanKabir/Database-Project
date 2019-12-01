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

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "admin"
#define DATABASE "proj3"

Driver *driver; //global variables makes it easier to use them in functions
Connection *con;
Statement *stmt;
ResultSet  *res;
ResultSet  *res1;
PreparedStatement *prep_stmt;

void delayCancel(); //function prototypes
void bookTicket();
void cancelTicket();
void display();

int main()
{
	try {

		// Create a connection
		driver = get_driver_instance();
		con = driver->connect(SERVER, USER, PASSWORD);

		stmt = con->createStatement();

		stmt->execute("DROP DATABASE IF EXISTS " DATABASE);
		stmt->execute("CREATE DATABASE " DATABASE);
		stmt->execute("USE " DATABASE);

		stmt->execute("DROP TABLE IF EXISTS plane"); //plane table
		stmt->execute("CREATE TABLE plane(plane_ID varchar(6), model varchar(20),primary key(plane_ID))");

		stmt->execute("DROP TABLE IF EXISTS seat_type"); //seat_type table
		stmt->execute("CREATE TABLE seat_type(no_seats_first int,no_seats_business int,no_seats_coach int,plane_ID varchar(6),primary key(no_seats_first, no_seats_business, no_seats_coach, plane_ID),foreign Key(plane_ID) references plane(plane_ID) on delete cascade)");

		stmt->execute("DROP TABLE IF EXISTS flight"); //flight table
		stmt->execute("CREATE TABLE flight(flight_ID varchar(6),plane_ID varchar(6),dep_dest char(4),arrive_dest char(4),dept_time DATETIME,arrive_time DATETIME,price numeric(5,2),primary key(flight_ID),foreign key(plane_ID) references plane(plane_ID) on delete set null)");

		stmt->execute("DROP TABLE IF EXISTS customer"); //customer table
		stmt->execute("CREATE TABLE customer(customer_ID varchar(6),firstName varchar(35),middleName varchar(35),lastName varchar(35),gender varchar(6),birthMonth int,birthDay int,BirthYear int,country varchar(35),known_traveler_no	numeric(10, 0),primary key(customer_ID))");

		stmt->execute("DROP TABLE IF EXISTS boarding_pass"); //boarding_pass table
		stmt->execute("CREATE TABLE boarding_pass(boarding_ID numeric(6,0),price numeric(5, 2),seat_no varchar(3),flight_ID varchar(6),customer_ID varchar(6),primary key(boarding_ID),foreign key(customer_ID) references customer(customer_ID),foreign key(flight_ID) references flight(flight_ID) on delete cascade)");

		stmt->execute("DROP TABLE IF EXISTS customer_emails"); //customer_emails table
		stmt->execute("CREATE TABLE customer_emails(customer_ID varchar(6),email varchar(50),primary key(customer_ID, email),foreign key(customer_ID) references customer(customer_ID) on delete cascade)");

		stmt->execute("DROP TABLE IF EXISTS extras"); //extras table
		stmt->execute("CREATE TABLE extras(boarding_ID numeric(6,0),TSA_precheck boolean,preBoard boolean,elite_status boolean,primary key(boarding_ID),foreign key(boarding_ID) references boarding_pass(boarding_ID) on delete cascade)");

		stmt->execute("DROP TABLE IF EXISTS rewards"); //rewards table
		stmt->execute("CREATE TABLE	rewards(member_ID numeric(6,0), customer_ID varchar(6), miles numeric(7,0), joinMonth int, joinDay int, joinYear int, status varchar(15), primary key(member_ID), foreign key(customer_ID) references customer (customer_ID))");

		stmt->execute("DROP TABLE IF EXISTS seats"); //seats table, this is new
		stmt->execute("CREATE TABLE seats(plane_ID varchar(6), row varchar(2), type varchar(2), primary key(plane_ID, row), foreign key (plane_ID) references plane (plane_ID))");

		//insert into plane
		stmt->execute("INSERT INTO plane(plane_ID, model) VALUES (01,'Boeing 737-800')");
		stmt->execute("INSERT INTO plane(plane_ID, model) VALUES (02,'Boeing 737-800')");
		stmt->execute("INSERT INTO plane(plane_ID, model) VALUES (03,'Boeing 737-800')");
		stmt->execute("INSERT INTO plane(plane_ID, model) VALUES (04,'Boeing 737-800')");
		stmt->execute("INSERT INTO plane(plane_ID, model) VALUES (05,'Boeing 737-800')");
		stmt->execute("INSERT INTO plane(plane_ID, model) VALUES (06,'Boeing 737-800')");
		stmt->execute("INSERT INTO plane(plane_ID, model) VALUES (07,'Boeing 737-800')");
		stmt->execute("INSERT INTO plane(plane_ID, model) VALUES (08,'Boeing 737-800')");
		stmt->execute("INSERT INTO plane(plane_ID, model) VALUES (09,'Boeing 737-800')");
		stmt->execute("INSERT INTO plane(plane_ID, model) VALUES (10,'Boeing 757-200')");
		//insert into seat type
		stmt->execute("INSERT INTO seat_type (no_seats_first,no_seats_business, no_seats_coach, plane_ID) VALUES (16,30,114,01)");
		stmt->execute("INSERT INTO seat_type (no_seats_first,no_seats_business, no_seats_coach, plane_ID) VALUES (16,30,114,02)");
		stmt->execute("INSERT INTO seat_type (no_seats_first,no_seats_business, no_seats_coach, plane_ID) VALUES (16,30,114,03)");
		stmt->execute("INSERT INTO seat_type (no_seats_first,no_seats_business, no_seats_coach, plane_ID) VALUES (16,30,114,04)");
		stmt->execute("INSERT INTO seat_type (no_seats_first,no_seats_business, no_seats_coach, plane_ID) VALUES (16,30,114,05)");
		stmt->execute("INSERT INTO seat_type (no_seats_first,no_seats_business, no_seats_coach, plane_ID) VALUES (16,30,114,06)");
		stmt->execute("INSERT INTO seat_type (no_seats_first,no_seats_business, no_seats_coach, plane_ID) VALUES (16,30,114,07)");
		stmt->execute("INSERT INTO seat_type (no_seats_first,no_seats_business, no_seats_coach, plane_ID) VALUES (16,30,114,08)");
		stmt->execute("INSERT INTO seat_type (no_seats_first,no_seats_business, no_seats_coach, plane_ID) VALUES (16,30,114,09)");
		stmt->execute("INSERT INTO seat_type (no_seats_first,no_seats_business, no_seats_coach, plane_ID) VALUES (16,36,132,10)");

		//plane 1's flights
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (01,01,'KJFK','KLAX','2017-12-01 21:55:00', '2017-12-02 01:14:00',250.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (02,01,'KLAX','KJFK','2017-12-02 21:05:00','2017-12-03 05:18:00',250.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (03,01,'KJFK','KALB','2017-12-03 19:00:00','2017-12-03 19:40:00',100.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (04,01,'KALB','KJFK','2017-12-04 08:00:00','2017-12-04 09:00:00',100.00)");

		//plane 2's flights
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (05,02,'KJFK','KMCO','2018-01-01 10:55:00', '2018-01-01 13:15:00',200.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (06,02,'KMCO','KJFK','2018-01-03 17:00:00', '2018-01-03 20:10:00',200.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (07,02,'KJFK','KCLT','2018-01-04 09:00:00', '2018-01-04 10:37:00',175.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (08,02,'KCLT','KJFK','2018-01-05 19:00:00', '2018-01-05 20:30:00',175.00)");

		//plane 3's flights
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (09,03,'KJFK','KPDX','2017-12-05 21:55:00', '2017-12-06 01:14:00',250.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (10,03,'KPDX','KJFK','2017-12-06 21:05:00', '2017-12-07 05:18:00',250.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (11,03,'KJFK','KSEA','2017-12-07 21:55:00', '2017-12-08 01:14:00',225.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (12,03,'KSEA','KJFK','2017-12-08 21:05:00', '2017-12-09 05:18:00',225.00)");

		//plane 4's flights
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (13,04,'KJFK','KDEN','2017-12-10 21:55:00', '2017-12-10 23:14:00',225.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (14,04,'KDEN','KJFK','2017-12-12 22:05:00', '2017-12-13 05:18:00',225.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (15,04,'KJFK','KSLC','2017-12-13 21:55:00', '2017-12-13 23:14:00',215.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (16,04,'KSLC','KJFK','2017-12-14 22:05:00', '2017-12-15 05:18:00',215.00)");

		//plane 5's flights
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (17,05,'KJFK','CYYZ','2017-12-16 16:00:00', '2017-12-16 18:00:00',175.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (18,05,'CYYZ','KJFK','2017-12-17 10:00:00', '2017-12-17 12:00:00',175.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (19,05,'KJFK','CYUL','2017-12-18 16:00:00', '2017-12-18 17:45:00',175.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (20,05,'CYUL','KJFK','2017-12-19 10:00:00', '2017-12-19 11:45:00',175.00)");

		//plane 6's flights
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (21,06,'KJFK','KPWM','2017-12-20 16:00:00', '2017-12-20 18:00:00',150.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (22,06,'KPWM','KJFK','2017-12-21 10:00:00', '2017-12-21 12:00:00',150.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (23,06,'KJFK','KMHT','2017-12-22 16:00:00', '2017-12-22 17:45:00',150.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (24,06,'KMHT','KJFK','2017-12-23 10:00:00', '2017-12-23 11:45:00',150.00)");

		//plane 7's flights
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (25,07,'KJFK','CYVR','2017-12-24 08:40:00', '2017-12-24 11:45:00',275.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (26,07,'CYVR','KJFK','2017-12-25 14:00:00', '2017-12-25 22:00:00',275.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (27,07,'KJFK','PANC','2017-12-26 16:00:00', '2017-12-26 20:00:00',275.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (28,07,'PANC','KJFK','2017-12-27 08:30:00', '2017-12-27 18:30:00',275.00)");

		//plane 8's flights
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (29,08,'KJFK','KORD','2017-12-28 07:30:00', '2017-12-28 09:30:00',125.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (30,08,'KORD','KJFK','2017-12-29 06:30:00', '2017-12-29 09:45:00',125.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (31,08,'KJFK','KDCA','2017-12-30 08:55:00', '2017-12-30 10:30:00',100.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (32,08,'KDCA','KJFK','2017-12-31 19:30:00', '2017-12-31 21:00:00',100.00)");

		//plane 9's flights
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (33,09,'KJFK','KMIA','2018-01-06 09:00:00', '2018-01-06 11:12:00',250.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (34,09,'KMIA','KJFK','2018-01-07 19:30:00', '2018-01-07 22:30:00',250.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (35,09,'KJFK','KAUS','2018-01-08 09:10:00', '2018-01-08 12:43:00',200.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (36,09,'KAUS','KJFK','2018-01-09 13:47:00', '2018-01-09 18:20:00',200.00)");

		//plane 10's flights
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (37,10,'KJFK','EGLL','2018-01-10 23:15:00', '2018-01-11 11:15:00',325.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (38,10,'EGLL','KJFK','2018-01-12 18:00:00', '2018-01-12 21:05:00',325.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (39,10,'KJFK','OMDB','2018-01-13 23:00:00', '2018-01-14 08:45:00',400.00)");
		stmt->execute("INSERT INTO flight (flight_ID, plane_ID, dep_dest, arrive_dest, dept_time, arrive_time, price) VALUES (40,10,'OMDB','KJFK','2018-01-14 15:00:00', '2018-01-14 20:40:00',400.00)");
		//insert into customers
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (01,'Clark','Joseph','Kent','male',04,07,1938,'United States',523310270)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (02,'Bruce','John','Wyane','male',11,07,1939,'United States',489285511)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (03,'Peter','Benjamin','Parker','male',10,01,1997,'United States',412050435)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (04,'Diana','W','Prince','female',05,16,1942,'United States',410588203)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (05,'Son','of','Goku','male',09,15,1975,'United States',500559359)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (06,'Scott','M','Summers','male',11,25,1985,'United States',956170890)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (07,'Tony','Robert','Stark','male',08,14,1965,'United States',683303714)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (08,'Steve','Grant','Rogers','male',07,04,1925,'United States',848703072)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (09,'Arthur','D','Curry','male',11,08,1941,'United States',774069378)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (10,'Wade','Winston','Wilson','male',02,15,1991,'United States',636397761)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (11, 'Lionel','Andres','Messi','male',06,24,1987,'Argentina',9876541)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (12, 'Eden','Michael','Hazard','male',01,07,1991,'Belgium',99665514)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (13, 'Antoine','Abella','Griezmann','male',03,21,1991,'France',632565415)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (14, 'Christian','Mate','Pulisic','male',09,18,1998,'United States',194545236)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (15, 'David','Fraser','Santan','male',06,05,1998,'United Kingdom',789541136)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (16, 'Theodore','Jasper','Detweiler','male',07,21,1988,'United States',165425478)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (17, 'Finn','The','Human','male',05,16,1996,'United States',216542554)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (18, 'Luis','Alberto','Suarez','male',01,27,1987,'Uruguay',799856221)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (19, 'Neymar','da Silva','Santos','male',02,03,1992,'Brazil',655442541)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (20, 'Denzel','Hayes','Washington','male',12,28,1954,'United States',452115644)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (21, 'Austin','Richard','Post','male',07,05,1995,'United States',126544897)");
		stmt->execute("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (22, 'Paulo','Bruno','Dybala','male',11,13,1993,'Argentina',465215479)");


		//boarding passes
		//flight 1
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111111,500.02,'3A',01,01)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111112,500.02,'4A',01,02)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111113,350.85,'10A',01,03)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111115,350.85,'11A',01,04)");
		//flight 2
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111116,500.02,'3A',02,01)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111117,500.02,'4A',02,02)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111118,350.85,'10A',02,03)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111119,350.85,'11A',02,04)");
		//flight 3
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111120,150.75,'3A',03,09)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111121,150.75,'4A',03,10)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111122,75.42,'10A',03,11)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111123,75.42,'11A',03,12)");
		//flight 4
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111124,150.75,'3A',04,09)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111125,150.75,'4A',04,10)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111126,75.42,'10A',04,11)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111127,75.42,'11A',04,12)");
		//flight 5
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111128,250.75,'3A',05,05)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111129,250.75,'4A',05,06)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111130,175.42,'10A',05,07)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111131,175.42,'11A',05,08)");
		//flight 6
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111132,250.75,'3A',06,05)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111133,250.75,'4A',06,06)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111134,175.42,'12A',06,07)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111135,175.42,'13A',06,08)");
		//flight 7
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111136,225.75,'3A',07,13)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111137,225.75,'4A',07,14)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111138,150.42,'12A',07,15)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111139,150.42,'13A',07,16)");
		//flight 8
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111140,225.75,'3A',08,13)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111141,225.75,'4A',08,14)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111142,150.42,'12A',08,15)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111143,150.42,'13A',08,16)");
		//flight 9
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111144,300.57,'3A',09,17)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111145,300.57,'4A',09,18)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111146,225.98,'12A',09,19)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111147,225.98,'13A',09,20)");
		//flight 10
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111148,300.57,'3A',10,17)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111149,300.57,'4A',10,18)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111150,225.98,'12A',10,19)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111151,225.98,'13A',10,20)");
		//flight 11
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111152,315.42,'3A',11,21)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111153,315.42,'4A',11,22)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111154,200.72,'12A',11,01)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111155,200.72,'13A',11,02)");
		//flight 12
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111156,315.42,'3A',12,21)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111157,315.42,'4A',12,22)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111158,200.72,'12A',12,01)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111159,200.72,'13A',12,02)");
		//flight 13
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111160,275.75,'3A',13,03)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111161,275.75,'4A',13,4)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111162,200.42,'12A',13,05)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111163,200.42,'13A',13,06)");
		//flight 14
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111164,275.75,'3A',14,03)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111165,275.75,'4A',14,04)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111166,200.42,'12A',14,05)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111167,200.42,'13A',14,06)");
		//flight 15
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111168,250.75,'3A',15,07)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111169,250.75,'4A',15,08)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111170,175.42,'12A',15,09)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111171,175.42,'13A',15,10)");
		//flight 16
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111172,250.75,'3A',16,07)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111173,250.75,'4A',16,08)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111174,175.42,'12A',16,09)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111175,175.42,'13A',16,10)");
		//flight 17
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111176,200.42,'3A',17,11)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111177,200.42,'4A',17,12)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111178,125.42,'12A',17,13)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111179,125.42,'13A',17,14)");
		//flight 18
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111180,200.42,'3A',18,11)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111181,200.42,'4A',18,12)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111182,125.42,'12A',18,13)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111183,125.42,'13A',18,14)");
		//flight 19
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111184,185.62,'3A',19,15)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111185,185.62,'4A',19,16)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111186,115.24,'12A',19,17)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111187,115.24,'13A',19,18)");
		//flight 20
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111188,185.62,'3A',20,15)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111189,185.62,'4A',20,16)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111190,115.24,'12A',20,17)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111191,115.24,'13A',20,18)");
		//flight 21
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111192,160.62,'3A',21,19)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111193,160.62,'4A',21,20)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111194,110.27,'12A',21,21)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111195,110.27,'13A',21,22)");
		//flight 22
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111196,160.62,'3A',22,19)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111197,160.62,'4A',22,20)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111198,110.27,'12A',22,21)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111199,110.27,'13A',22,22)");
		//flight 23
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111200,160.62,'3A',23,01)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111201,160.62,'4A',23,02)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111202,110.27,'12A',23,03)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111203,110.27,'13A',23,04)");
		//flight 24
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111204,160.62,'3A',24,01)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111205,160.62,'4A',24,02)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111206,110.27,'12A',24,03)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111207,110.27,'13A',24,04)");
		//flight 25
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111208,200.42,'3A',25,05)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111209,200.42,'4A',25,06)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111210,125.42,'12A',25,07)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111211,125.42,'13A',25,08)");
		//flight 26
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111212,200.42,'3A',26,05)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111213,200.42,'4A',26,06)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111214,125.42,'12A',26,07)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111215,125.42,'13A',26,08)");
		//flight 27
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111216,300.00,'3A',27,09)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111217,300.00,'4A',27,10)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111218,225.00,'12A',27,11)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111219,225.00,'13A',27,12)");
		//flight 28
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111220,300.00,'3A',28,09)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111221,300.00,'4A',28,10)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111222,225.00,'12A',28,11)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111223,225.00,'13A',28,12)");
		//flight 29
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111224,150.75,'3A',29,13)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111225,150.75,'4A',29,14)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111226,75.42,'12A',29,15)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111227,75.42,'13A',29,16)");
		//flight 30
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111228,150.75,'3A',30,13)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111229,150.75,'4A',30,14)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111230,75.42,'12A',30,15)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111231,75.42,'13A',30,16)");
		//flight 31
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111232,175.75,'3A',31,17)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111233,175.75,'4A',31,18)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111234,100.42,'12A',31,19)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111235,100.42,'13A',31,20)");
		//flight 32
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111236,175.75,'3A',32,17)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111237,175.75,'4A',32,18)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111238,100.42,'12A',32,19)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111239,100.42,'13A',32,20)");
		//flight 33
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111240,250.75,'3A',33,21)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111241,250.75,'4A',33,22)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111242,175.42,'12A',33,01)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111243,175.42,'13A',33,02)");
		//flight 34
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111244,250.75,'3A',34,21)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111245,250.75,'4A',34,22)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111246,175.42,'12A',34,01)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111247,175.42,'13A',34,02)");
		//flight 35
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111248,225.52,'3A',35,03)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111249,225.52,'4A',35,04)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111250,175.84,'12A',35,05)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111251,175.84,'13A',35,06)");
		//flight 36
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111252,225.52,'3A',36,03)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111253,225.52,'4A',36,04)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111254,175.84,'12A',36,05)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111255,175.84,'13A',36,06)");
		//flight 37
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111256,550.00,'3A',37,07)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111257,550.00,'4A',37,08)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111258,295.00,'12A',37,09)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111259,295.00,'13A',37,10)");
		//flight 38
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111260,550.00,'3A',38,07)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111261,550.00,'4A',38,08)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111262,295.00,'12A',38,09)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111263,295.00,'13A',38,10)");
		//flight 39
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111264,750.00,'1A',39,11)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111265,750.00,'2A',39,12)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111266,325.00,'12A',39,13)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111267,325.00,'13A',39,14)");
		//flight 40
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111268,750.00,'1A',40,11)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111269,750.00,'2A',40,12)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111270,325.00,'12A',40,13)");
		stmt->execute("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (111271,325.00,'13A',40,14)");

		//inserts for extras
		//flights 1+2
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111111,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111112,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111113,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111115,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111116,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111117,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111118,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111119,true,false,false)");
		//flights 3+4
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111120,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111121,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111122,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111123,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111124,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111125,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111126,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111127,true,false,true)");
		//flights 5+6
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111128,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111129,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111130,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111131,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111132,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111133,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111134,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111135,true,false,false)");
		//flights 7+8
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111136,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111137,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111138,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111139,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111140,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111141,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111142,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111143,true,false,true)");
		//flights 9+10
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111144,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111145,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111146,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111147,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111148,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111149,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111150,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111151,true,false,true)");
		//flights 11+12
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111152,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111153,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111154,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111155,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111156,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111157,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111158,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111159,true,false,false)");
		//flights 13+14
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111160,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111161,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111162,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111163,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111164,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111165,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111166,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111167,true,false,false)");
		//flights 15+16
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111168,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111169,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111170,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111171,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111172,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111173,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111174,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111175,true,false,false)");
		//flights 17+18
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111176,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111177,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111178,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111179,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111180,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111181,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111182,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111183,true,false,true)");
		//flights 19+20
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111184,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111185,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111186,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111187,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111188,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111189,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111190,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111191,true,false,true)");
		//flights 21+22
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111192,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111193,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111194,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111195,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111196,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111197,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111198,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111199,true,false,true)");
		//flights 23+24
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111200,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111201,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111202,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111203,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111204,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111205,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111206,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111207,true,false,false)");
		//flights 25+26
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111208,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111209,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111210,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111211,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111212,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111213,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111214,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111215,true,false,false)");
		//flights 27+28
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111216,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111217,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111218,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111219,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111220,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111221,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111222,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111223,true,false,true)");
		//flights 29+30
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111224,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111225,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111226,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111227,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111228,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111229,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111230,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111231,true,false,true)");
		//flights 31+32
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111232,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111233,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111234,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111235,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111236,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111237,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111238,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111239,true,false,true)");
		//flights 33+34
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111240,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111241,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111242,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111243,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111244,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111245,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111246,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111247,true,false,false)");
		//flights 35+36
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111248,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111249,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111250,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111251,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111252,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111253,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111254,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111255,true,false,false)");
		//flights 37+38
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111256,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111257,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111258,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111259,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111260,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111261,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111262,true,false,false)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111263,true,false,false)");
		//flights 39+40
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111264,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111265,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111266,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111267,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111268,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111269,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111270,true,false,true)");
		stmt->execute("INSERT INTO extras(boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (111271,true,false,true)");
		//inserts for emails
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (01,'superMan@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (02,'batMan@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (03,'spiderMan@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (04,'wonderWoman@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (05,'ssjGoku@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (06,'cyclops@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (07,'theRealIronMan@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (08,'cptUSA@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (09,'aquaMan@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (10,'deadPoolLOL@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (11,'theChoosenOne@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (12,'goldenBoy@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (13,'griezmann@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (14,'mrAmerica@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (15,'santanDave@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (16,'TJ@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (17,'finn@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (18,'shoulderBiter@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (19,'nextPele@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (20,'Denzel.Washington@fordham.edu')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (21,'postMalone@gmail.com')");
		stmt->execute("INSERT INTO customer_emails (customer_ID, email) VALUES (22,'bruno@gmail.com')");
		//inserts for rewards
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100000,02,1500,05,04,2017,'Elite')");
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100001,07,1500,05,05,2017,'Member')");
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100002,11,1500,05,06,2017,'Elite')");
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100003,12,500,05,07,2017,'Elite')");
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100004,13,500,05,08,2017,'Elite')");
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100005,14,500,05,09,2017,'Elite')");
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100006,15,500,05,10,2017,'Elite')");
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100007,16,500,05,11,2017,'Elite')");
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100008,17,500,05,12,2017,'Elite')");
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100009,18,500,05,13,2017,'Elite')");
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100010,19,500,05,14,2017,'Elite')");
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100011,20,500,05,15,2017,'Elite')");
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100012,21,500,05,16,2017,'Elite')");
		stmt->execute("INSERT INTO rewards (member_ID,customer_ID,miles,joinMonth,joinDay,joinYear,status) VALUES (100013,22,500,05,17,2017,'Elite')");
		//inserts for seats
		//for all planes, F: First Class; B: Business Class; C: Coach Class; BE: Business Class, emergency exit row; CE: Coach Class, emergency exit row
		//All first class rows have 4 seats each (AB/CD)
		//All business & coach class rows have 6 seats each (ABC/DEF)
		//planes 1-9, all the same
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,3,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,4,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,5,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,6,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,7,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,8,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,9,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,10,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,11,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,12,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,13,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,14,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,15,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,16,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,17,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,18,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,19,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,20,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,21,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,22,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,23,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,24,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,25,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,26,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,27,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,28,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,29,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (01,30,'C')");

		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,3,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,4,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,5,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,6,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,7,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,8,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,9,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,10,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,11,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,12,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,13,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,14,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,15,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,16,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,17,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,18,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,19,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,20,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,21,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,22,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,23,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,24,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,25,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,26,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,27,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,28,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,29,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (02,30,'C')");

		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,3,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,4,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,5,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,6,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,7,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,8,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,9,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,10,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,11,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,12,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,13,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,14,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,15,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,16,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,17,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,18,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,19,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,20,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,21,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,22,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,23,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,24,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,25,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,26,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,27,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,28,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,29,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (03,30,'C')");

		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,3,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,4,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,5,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,6,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,7,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,8,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,9,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,10,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,11,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,12,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,13,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,14,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,15,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,16,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,17,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,18,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,19,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,20,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,21,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,22,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,23,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,24,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,25,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,26,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,27,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,28,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,29,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (04,30,'C')");

		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,3,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,4,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,5,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,6,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,7,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,8,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,9,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,10,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,11,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,12,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,13,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,14,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,15,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,16,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,17,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,18,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,19,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,20,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,21,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,22,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,23,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,24,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,25,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,26,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,27,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,28,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,29,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (05,30,'C')");

		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,3,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,4,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,5,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,6,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,7,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,8,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,9,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,10,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,11,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,12,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,13,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,14,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,15,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,16,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,17,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,18,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,19,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,20,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,21,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,22,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,23,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,24,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,25,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,26,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,27,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,28,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,29,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (06,30,'C')");

		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,3,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,4,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,5,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,6,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,7,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,8,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,9,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,10,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,11,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,12,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,13,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,14,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,15,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,16,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,17,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,18,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,19,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,20,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,21,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,22,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,23,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,24,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,25,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,26,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,27,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,28,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,29,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (07,30,'C')");

		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,3,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,4,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,5,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,6,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,7,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,8,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,9,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,10,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,11,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,12,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,13,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,14,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,15,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,16,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,17,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,18,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,19,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,20,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,21,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,22,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,23,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,24,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,25,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,26,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,27,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,28,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,29,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (08,30,'C')");

		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,3,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,4,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,5,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,6,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,7,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,8,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,9,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,10,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,11,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,12,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,13,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,14,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,15,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,16,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,17,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,18,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,19,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,20,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,21,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,22,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,23,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,24,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,25,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,26,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,27,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,28,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,29,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (09,30,'C')");
		//plane 10
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,1,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,2,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,3,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,4,'F')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,5,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,6,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,7,'BE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,8,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,9,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,10,'B')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,11,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,12,'CE')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,13,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,14,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,15,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,16,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,17,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,18,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,19,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,20,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,21,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,22,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,23,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,24,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,25,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,26,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,27,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,28,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,29,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,30,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,31,'C')");
		stmt->execute("INSERT INTO seats (plane_ID, row, type) VALUES (10,32,'C')");

		int ans;
		while (true)
		{ //mail menu, runs for until user does option 5
			cout << "Welcome to the PanAm database system!" << endl;
			cout << "1.) Change or cancel time of arrival and/or departure in case of delay" << endl;
			cout << "2.) Purchase a ticket" << endl;
			cout << "3.) Cancel a ticket" << endl;
			cout << "4.) Display what people will be on a future flight" << endl;
			cout << "5.) Quit the program" << endl;
			cout << "Input a choice: ";
			cin >> ans;
			while (ans < 1 || ans > 5)
			{
				cout << "Invalid data, try again please" << endl;
				cout << "1.) Change or cancel time of arrival and/or departure in case of delay" << endl;
				cout << "2.) Purchase a ticket" << endl;
				cout << "3.) Cancel a ticket" << endl;
				cout << "4.) Display what people will be on a particular flight" << endl;
				cout << "5.) Quit the program" << endl;
				cout << "Input a choice: ";
				cin >> ans;
			}
			if (ans == 1)
				delayCancel();
			else if (ans == 2)
				bookTicket();
			else if (ans == 3)
				cancelTicket();
			else if (ans == 4)
				display();
			else
			{ //deletes pointer variables
				delete res;
				delete stmt;
				delete con;
				return 0;
			}
		}
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

void delayCancel()
{
	int ans, choice;
	bool flag = false;
	res = stmt->executeQuery("SELECT * FROM flight ORDER BY (dept_time)");
	while (res->next()) { //lists all flights with helpful info
		cout << "Flight ID: " << res->getString(1) << ", Plane ID: " << res->getString(2) << ", Departure: " << res->getString(3) << ", Arrival: " << res->getString(4) << ", Departure Time: " << res->getString(5) << ", Arrival Time: " << res->getString(6) << endl;
	}
	cout << "Enter the flight ID of the flight that you are changing: ";
	cin >> ans;

	res = stmt->executeQuery("SELECT flight_ID FROM flight ORDER BY (dept_time)");
	while (res->next()) { //input validation
		if (res->getInt(1) == ans)
			flag = true;
	}
	while (flag == false)
	{
		flag = true;
		cout << "You have entered an invalid flight id, please try again" << endl;
		cout << "Enter the flight ID of the flight that you are changing: ";
		cin >> ans;

		res = stmt->executeQuery("SELECT flight_ID FROM flight ORDER BY (dept_time)");
		while (res->next()) {
			if (res->getInt(1) == ans)
				flag = true;
		}
	}

	cout << "1.) Change Departure/Arrival Time(s)" << endl << "2.) Cancel flight" << endl <<"Input: ";
	cin >> choice;
	while (choice !=1 && choice != 2)
	{ //input validation
		cout << "Invalid input, please try again" << endl;
		cout << "1.) Change Departure/Arrival Time(s)" << endl << "2.) Cancel flight" << endl << "Input: ";
		cin >> choice;
	}
	if (choice == 2)
	{ //cancel flight
		prep_stmt = con->prepareStatement("DELETE FROM flight WHERE flight_ID = (?)");
		prep_stmt->setInt(1, ans);
		res = prep_stmt->executeQuery();
		cout << "Flight has been cancelled" << endl;
	}
	else
	{ //this changes flight
		prep_stmt = con->prepareStatement("SELECT * FROM flight WHERE flight_ID = (?)");
		prep_stmt->setInt(1, ans); //this prepares for the flight the user wants to delay for updating
		res = prep_stmt->executeQuery();

		string DNewY, DNewM, DNewD, DNewHour, DNewMin, DNewSec;
		string ANewY, ANewM, ANewD, ANewHour, ANewMin, ANewSec;
		cout << "Enter the new departure date by typing the YYYY MM DD: ";
		cin >> DNewY >> DNewM >> DNewD; //new departure date (some times a flight is super delayed to the next day)
		bool flag = true; //input validation
		if (DNewY.length() != 4)
			flag = false;
		if (DNewM.length() != 2)
			flag = false;
		if (DNewD.length() != 2)
			flag = false;
		for (int i = 0; i < DNewY.length(); i++)
		{
			if (isdigit(DNewY[i]) == false)
				flag = false;
		}
		for (int i = 0; i < DNewM.length(); i++)
		{
			if (isdigit(DNewM[i]) == false)
				flag = false;
		}
		for (int i = 0; i < DNewD.length(); i++)
		{
			if (isdigit(DNewD[i]) == false)
				flag = false;
		}
		while (flag == false)
		{
			cout << "Invalid input, please try again" << endl;
			cout << "Enter the new departure date by typing the YYYY MM DD: ";
			cin >> DNewY >> DNewM >> DNewD;
			flag = true;
			if (DNewY.length() != 4)
				flag = false;
			if (DNewM.length() != 2)
				flag = false;
			if (DNewD.length() != 2)
				flag = false;
			for (int i = 0; i < DNewY.length(); i++)
			{
				if (isdigit(DNewY[i]) == false)
					flag = false;
			}
			for (int i = 0; i < DNewM.length(); i++)
			{
				if (isdigit(DNewM[i]) == false)
					flag = false;
			}
			for (int i = 0; i < DNewD.length(); i++)
			{
				if (isdigit(DNewD[i]) == false)
					flag = false;
			}
		}
		cout << "Enter the new departure time by typing the HH MM SS: ";
		cin >> DNewHour >> DNewMin >> DNewSec; //new departure time
		flag = true;  //more input validation
		if (DNewHour.length() != 2)
			flag = false;
		if (DNewMin.length() != 2)
			flag = false;
		if (DNewSec.length() != 2)
			flag = false;
		for (int i = 0; i < DNewHour.length(); i++)
		{
			if (isdigit(DNewHour[i]) == false)
				flag = false;
		}
		for (int i = 0; i < DNewMin.length(); i++)
		{
			if (isdigit(DNewMin[i]) == false)
				flag = false;
		}
		for (int i = 0; i < DNewSec.length(); i++)
		{
			if (isdigit(DNewSec[i]) == false)
				flag = false;
		}
		while (flag == false)
		{
			cout << "Invalid input, please try again" << endl;
			cout << "Enter the new departure time by typing the HH MM SS: ";
			cin >> DNewHour >> DNewMin >> DNewSec;
			flag = true;
			if (DNewHour.length() != 2)
				flag = false;
			if (DNewMin.length() != 2)
				flag = false;
			if (DNewSec.length() != 2)
				flag = false;
			for (int i = 0; i < DNewHour.length(); i++)
			{
				if (isdigit(DNewHour[i]) == false)
					flag = false;
			}
			for (int i = 0; i < DNewMin.length(); i++)
			{
				if (isdigit(DNewMin[i]) == false)
					flag = false;
			}
			for (int i = 0; i < DNewSec.length(); i++)
			{
				if (isdigit(DNewSec[i]) == false)
					flag = false;
			}
		}


		cout << "Enter the new arrival date by typing the YYYY MM DD: ";
		cin >> ANewY >> ANewM >> ANewD;
		flag = true; //new arrival date (some times a flight is super delayed to the next day)
		if (ANewY.length() != 4)
			flag = false; //input validation
		if (ANewM.length() != 2)
			flag = false;
		if (ANewD.length() != 2)
			flag = false;
		for (int i = 0; i < ANewY.length(); i++)
		{
			if (isdigit(ANewY[i]) == false)
				flag = false;
		}
		for (int i = 0; i < ANewM.length(); i++)
		{
			if (isdigit(ANewM[i]) == false)
				flag = false;
		}
		for (int i = 0; i < ANewD.length(); i++)
		{
			if (isdigit(ANewD[i]) == false)
				flag = false;
		}
		while (flag == false)
		{
			cout << "Invalid input, please try again" << endl;
			cout << "Enter the new arrival date by typing the YYYY MM DD: ";
			cin >> ANewY >> ANewM >> ANewD;
			flag = true;
			if (ANewY.length() != 4)
				flag = false;
			if (ANewM.length() != 2)
				flag = false;
			if (ANewD.length() != 2)
				flag = false;
			for (int i = 0; i < ANewY.length(); i++)
			{
				if (isdigit(ANewY[i]) == false)
					flag = false;
			}
			for (int i = 0; i < ANewM.length(); i++)
			{
				if (isdigit(ANewM[i]) == false)
					flag = false;
			}
			for (int i = 0; i < ANewD.length(); i++)
			{
				if (isdigit(ANewD[i]) == false)
					flag = false;
			}
		}


		cout << "Enter the new arrival time by typing the HH MM SS: ";
		cin >> ANewHour >> ANewMin >> ANewSec; //new arrival time
		flag = true;
		if (ANewHour.length() != 2)
			flag = false; //input validation
		if (ANewMin.length() != 2)
			flag = false;
		if (ANewSec.length() != 2)
			flag = false;
		for (int i = 0; i < ANewHour.length(); i++)
		{
			if (isdigit(ANewHour[i]) == false)
				flag = false;
		}
		for (int i = 0; i < ANewMin.length(); i++)
		{
			if (isdigit(ANewMin[i]) == false)
				flag = false;
		}
		for (int i = 0; i < ANewSec.length(); i++)
		{
			if (isdigit(ANewSec[i]) == false)
				flag = false;
		}
		while (flag == false)
		{
			cout << "Invalid input, please try again" << endl;
			cout << "Enter the new arrival time by typing the HH MM SS: ";
			cin >> ANewHour >> ANewMin >> ANewSec;
			flag = true;
			if (ANewHour.length() != 2)
				flag = false;
			if (ANewMin.length() != 2)
				flag = false;
			if (ANewSec.length() != 2)
				flag = false;
			for (int i = 0; i < ANewHour.length(); i++)
			{
				if (isdigit(ANewHour[i]) == false)
					flag = false;
			}
			for (int i = 0; i < ANewMin.length(); i++)
			{
				if (isdigit(ANewMin[i]) == false)
					flag = false;
			}
			for (int i = 0; i < ANewSec.length(); i++)
			{
				if (isdigit(ANewSec[i]) == false)
					flag = false;
			}
		}


		string dep_t = DNewY+DNewM+DNewD+DNewHour+DNewMin+DNewSec; //combines all the user info for the new departure
		prep_stmt = con->prepareStatement("UPDATE flight SET dept_time = (?) WHERE flight_ID = (?)");
		prep_stmt->setDateTime(1,dep_t);
		prep_stmt->setInt(2,ans); //updates the departure time & date
		prep_stmt->executeUpdate();
		
		string arv_t = ANewY + ANewM + ANewD + ANewHour + ANewMin + ANewSec; //combines all the user info for the new arrival
		prep_stmt = con->prepareStatement("UPDATE flight SET arrive_time = (?) WHERE flight_ID = (?)");
		prep_stmt->setDateTime(1, arv_t);
		prep_stmt->setInt(2, ans); //updates the arrival time & date
		prep_stmt->executeUpdate();

		cout << "Flight has been updated" << endl;
	}
}

void bookTicket()
{ //booking a new ticket
	string fName, mName, lName, gend, country, email;
	int bMonth, bYear, bDay, KTN, flightNum, level, gender, ans, customer_ID, rewards_ID;
	char ch;
	cout << "Please enter your first, middle, and last names, each followed by a space: ";
	cin >> fName >> mName >> lName; //uers info for full name
	cout << "Please enter your gender" << endl << "1.) Male" << endl << "2.) Female" << endl << "Input: ";
	cin >> gender; 
	if (gender == 1)
		gend = "male";
	else if (gender == 2)
		gend = "female";
	else
	{ //input validation
		while (gender != 1 || gender != 2)
		{
			cout << "Invalid input, please try again" << endl;
			cout << "Please enter your gender" << endl << "1.) Male" << "2.) Female" << "Input: ";
			cin >> gender;
			if (gender == 1)
				gend == "male";
			else if (gender == 2)
				gend = "female";
		}
	}
	cout << "Please enter your birth date (YYYY MM DD): ";
	cin >> bYear >> bMonth >> bDay;
	bool flag = true; //uers's birthday with indput validation
	if (bYear < 1900 || bYear > 2017)
		flag = false;
	if (bMonth < 1 || bMonth > 12)
		flag = false;
	if (bDay < 1 || bDay > 31)
		flag = false;
	while (flag == false)
	{
		cout << "Invalid input, please try again" << endl;
		cout << "Please enter your birth date (YYYY MM DD): ";
		cin >> bYear >> bMonth >> bDay;
		flag = true;
		if(bYear < 1900 || bYear > 2017)
			flag = false;
		if (bMonth < 1 || bMonth > 12)
			flag = false;
		if (bDay < 1 || bDay > 31)
			flag = false;
	}

	cout << "Please enter your Known Traveler Number (KTN) if unknow or you do not have one, please enter -1: ";
	cin >> KTN;
	//optional known traveler number for each booking
	if (KTN < 100000000 || KTN > 999999999)
	{
		if(KTN!=-1)
			flag = false;
	}
	else
		flag = true;
	while (flag == false)
	{ //input validation
		cout << "Invalid input, please try again" << endl;
		cout << "Please enter your Known Traveler Number (KTN) if unknow or you do not have one, please enter -1: ";
		cin >> KTN;

		if (KTN < 100000000 || KTN > 999999999)
		{
			if (KTN==-1)
				flag=true;
		}
		else
			flag = true;
	}

	cin.ignore();
	

	cout << "Please enter your country: ";
	getline(cin, country); //inputing the user's country
	cout << "Available flights are: " << endl;
	res = stmt->executeQuery("SELECT * FROM flight ORDER BY (dept_time)");
	while (res->next()) { //pulls up all flights
		cout << "Flight ID: " << res->getString(1) << ", Plane ID: " << res->getString(2) << ", Departure: " << res->getString(3) << ", Arrival: " << res->getString(4) << ", Departure Time: " << res->getString(5) << ", Arrival Time: " << res->getString(6) << endl;
	}
	cout << "Please choose a flight to book: ";
	cin >> flightNum; //user selects flight to book
	res = stmt->executeQuery("SELECT flight_ID FROM flight");
	flag = false;
	while (res->next())
	{ //input validation
		if (res->getInt(1) == flightNum)
			flag = true;
	}
	while (flag == false)
	{
		cout << "Invalid input, please try again" << endl;
		cout << "Please choose a flight to book: ";
		cin >> flightNum;
		res = stmt->executeQuery("SELECT flight_ID FROM flight");
		flag = false;
		while (res->next())
		{ //basically, the program checks ther user's flight to all flights
			if (res->getInt(1) == flightNum)
				flag = true;
		}
	}

	do
	{
		flag = false;
		cout << "1.) Coach" << endl << "2.) Business Class" << endl << "3.) First Class" << endl << "Input: ";
		cin >> level;
		if (level < 1 || level > 3)
		{ //class selection & input validation
			flag = false;
			cout << "Invalid input, please try again" << endl;
		}
		else
			flag = true;
	} while (flag == false);
	string seatType;
	double price = 0.0;
	if (level == 1)
	{ //calculates and shows the price for the user depending on their seat class
		prep_stmt = con->prepareStatement("SELECT price FROM flight WHERE flight_ID = (?)");
		prep_stmt->setInt(1, flightNum);
		res = prep_stmt->executeQuery();
		res->next();
		price = res->getDouble(1);
		cout << "The price is: $" << price << endl;
		seatType = "C";
	}
	else if (level == 2)
	{ //price increased by 25% for business class
		prep_stmt = con->prepareStatement("SELECT price*1.25 FROM flight WHERE flight_ID = (?)");
		prep_stmt->setInt(1, flightNum);
		res = prep_stmt->executeQuery();
		res->next();
		price = res->getDouble(1);
		cout << "The price is: $" << price << endl;
		seatType = "B";
	}
	else
	{//price increased by 75% for first class
		prep_stmt = con->prepareStatement("SELECT price*1.75 FROM flight WHERE flight_ID = (?)");
		prep_stmt->setInt(1, flightNum);
		res = prep_stmt->executeQuery();
		res->next();
		price = res->getDouble(1);
		cout << "The price is: $" << price << endl;
		seatType = "F";
	}

	do
	{
		flag = false; //asking user about emergency exit row
		cout << "Would you be okay with an emergency exit seat? (y/n): " << endl << "Input: ";
		cin >> ch;
		if (ch != 'y'&&ch != 'n')
		{ //input validation
			flag = false;
			cout << "Invalid input, please try again" << endl;
		}
		else
			flag = true;
	} while (flag == false);
	bool preB, eliteS;
	do
	{ //same deal as above but with pre boarding
		flag = false;
		cout << "Are you a pre-boarder? (y/n)" << endl << "Input: ";
		cin >> ch;
		if (ch != 'y'&&ch != 'n')
		{
			flag = false;
			cout << "Invalid input, please try again" << endl;
		}
		else
			flag = true;
	} while (flag == false);
	if (ch == 'y')
		preB = true;
	else
		preB = false;

	do
	{ //same deal as above but rewards member
		flag = false;
		cout << "Are you a rewards member? (y/n)" << endl << "Input: ";
		cin >> ch;
		if (ch != 'y'&&ch != 'n')
		{
			flag = false;
			cout << "Invalid input, please try again" << endl;
		}
		else
			flag = true;
	} while (flag == false);
	if (ch == 'y')
	{
		do
		{
			cout << "Enter you number: ";
			cin >> rewards_ID; //asks the user to enter member_ID and checks to make sure it is a real rewards member
			res = stmt->executeQuery("SELECT member_ID FROM rewards");
			flag = false;
			while (res->next())
			{
				if (res->getInt(1) == rewards_ID)
					flag = true;
			}
			if (flag == false)
			{
				cout << "Invalid input, please try again" << endl;
				flag = false;
			}
			else
				flag = true;
		} while (flag == false);
		prep_stmt = con->prepareStatement("SELECT status FROM rewards WHERE member_ID = (?)");
		prep_stmt->setInt(1,rewards_ID); //figures out the status of the rewards member (AKA check if elite)
		res = prep_stmt->executeQuery();
		res->next();
		if(res->getString(1)=="elite")
			eliteS = true;
		else
			eliteS = false;
	}
	else
		eliteS = false;


	do
	{
		flag = false;
		cout << "Is everything above correct? (y/n)" << endl << "Input: ";
		cin >> ch; //gives the user a chance to cancel
		if (ch != 'y'&&ch != 'n')
		{
			flag = false;
			cout << "Invalid input, please try again" << endl;
		}
		else
			flag = true;
	} while (flag == false);
	if (ch == 'y')
	{
		int cID=-1;
		//generate customer_ID
		res = stmt->executeQuery("SELECT customer_ID FROM customer ORDER BY (customer_ID)");
		while (res->next())
		{
			if(cID < res->getInt(1))
				cID = res->getInt(1);
		}
		cID++; //inserts the new customer
		prep_stmt = con->prepareStatement("INSERT INTO customer (customer_ID,firstName,middleName,lastName,gender,birthMonth,birthDay,birthYear,country,known_traveler_no) VALUES (?,?,?,?,?,?,?,?,?,?)");
		prep_stmt->setInt(1,cID);
		prep_stmt->setString(2, fName);
		prep_stmt->setString(3, mName);
		prep_stmt->setString(4, lName);
		prep_stmt->setString(5, gend);
		prep_stmt->setInt(6, bMonth);
		prep_stmt->setInt(7, bDay);
		prep_stmt->setInt(8, bYear);
		prep_stmt->setString(9, country);
		prep_stmt->setInt(10, KTN);
		prep_stmt->execute();

		

		//find an empty seat
		prep_stmt = con->prepareStatement("SELECT row FROM flight NATURAL JOIN seats WHERE flight_ID = (?) AND type = (?)");
		prep_stmt->setInt(1, flightNum);
		prep_stmt->setString(2, seatType);
		res1 = prep_stmt->executeQuery();
		string seatNumber;
		bool seatFound;
		while (res1->next())
		{
			seatFound = false;
			if (seatType == "F")
			{
				string seatTest = res1->getString(1);
				string seats[4] = { seatTest + "A", seatTest + "B", seatTest + "C", seatTest + "D" };

				for (int i = 0; i < 4; i++)
				{
					seatFound = false;
					prep_stmt = con->prepareStatement("SELECT seat_no FROM boarding_pass WHERE flight_ID = (?)");
					prep_stmt->setInt(1, flightNum);
					res1 = prep_stmt->executeQuery();
					while (res1->next())
					{
						if (res1->getString(1) == seats[i])
						{
							seatFound = true;
						}
					}
					if (seatFound == false)
					{
						seatNumber = seats[i];
						break;
					}
				}
			}
			else
			{
				string seatTest = res1->getString(1);
				string seats[6] = { seatTest + "A", seatTest + "B", seatTest + "C", seatTest + "D", seatTest + "E", seatTest + "F" };

				for (int i = 0; i < 6; i++)
				{
					seatFound = false;
					prep_stmt = con->prepareStatement("SELECT seat_no FROM boarding_pass WHERE flight_ID = (?)");
					prep_stmt->setInt(1, flightNum);
					res1 = prep_stmt->executeQuery();
					while (res1->next())
					{
						if (res1->getString(1) == seats[i])
						{
							seatFound = true;
						}
					}
					if (seatFound == false)
					{
						seatNumber = seats[i];
						break;
					}
				}
			}
			//if at the end, seatFound is still set to true, that means, no free seats in the user-specified class and flight cannot be booked
			if (seatFound == true)
			{
				cout << "We're sorry, but the seat you were looking for is not available, you might be able to get a seat in a different class or on different flight." << endl;
				return;
			}
		}
		//boarding pass is inserted for the user
		int bID = -1;
		res = stmt->executeQuery("SELECT boarding_ID FROM boarding_pass");
		while (res->next())
		{
			if (bID < res->getInt(1))
				bID = res->getInt(1);
		}
		bID++;
		cout << "Your seat is: " << seatNumber << endl;
		prep_stmt = con->prepareStatement("INSERT INTO boarding_pass (boarding_ID, price, seat_no, flight_ID, customer_ID) VALUES (?,?,?,?,?)");
		prep_stmt->setInt(1, bID);
		prep_stmt->setDouble(2, price);
		prep_stmt->setString(3, seatNumber);
		prep_stmt->setInt(4, flightNum);
		prep_stmt->setInt(5, cID);
		prep_stmt->execute();
		//extras are noted
		prep_stmt = con->prepareStatement("INSERT INTO extras (boarding_ID,TSA_precheck,preBoard,elite_status) VALUES (?,?,?,?)");
		prep_stmt->setInt(1, bID);

		if (KTN == -1)
			prep_stmt->setBoolean(2, false);
		else
			prep_stmt->setBoolean(2, true);
		prep_stmt->setBoolean(3, preB);
		prep_stmt->setBoolean(4, eliteS);
		prep_stmt->execute();
		//lets the user enter in multiple emails
		int emails;
		string email;
		cout << "How many emails would you like to have associated: ";
		cin >> emails;
		for (int i = 0; i < emails; i++)
		{
			cout << "Enter email " << i+1 << ": ";
			cin >> email;
			prep_stmt = con->prepareStatement("INSERT INTO customer_emails (customer_ID, email) VALUES (?,?)");
			prep_stmt->setInt(1, cID);
			prep_stmt->setString(2, email);
			prep_stmt->execute();
		}
		cout << "Flight is booked, have a good day!" << endl;
		return;
	}
	else
	{//if user decides to quit back to main menu and not book a ticket
		cout << "Sorry to hear that, you will now be taken back to the main menu" << endl;
		return;
	}
}

void cancelTicket()
{
	int boardingID, customerID;
	char ans;
	cout << "Do you know the boarding ID? (y/n): ";
	cin >> ans;
	while (ans != 'y' && ans != 'n')
	{ //input validation
		cout << "Invalid input, please try again" << endl;
		cout << "Do you know the boarding ID? (y/n): ";
		cin >> ans;
	}
	if (ans == 'y')
	{ //delete boarding pass/ticked using the unique boarding ID
		cout << "What is the boarding ID: ";
		cin >> boardingID;
		prep_stmt = con->prepareStatement("DELETE FROM boarding_pass WHERE boarding_ID = (?)");
		prep_stmt->setInt(1, boardingID);
		res = prep_stmt->executeQuery();
		cout << "Ticket has been cancelled" << endl;
	}
	else
	{
		cout << "Do you konw the customer ID? (y/n): ";
		cin >> ans;
		while (ans != 'y' && ans != 'n')
		{ //input validation
			cout << "Invalid input, please try again" << endl;
			cout << "Do you know the customer ID? (y/n): ";
			cin >> ans;
		}
		if (ans == 'y')
		{
			cout << "What is the customer ID: ";
			cin >> customerID;
			prep_stmt = con->prepareStatement("SELECT * FROM boarding_pass WHERE customer_ID = (?)");
			prep_stmt->setInt(1, customerID);
			res = prep_stmt->executeQuery();
			while (res->next())
			{ //finds all boarding passes with a matching customer ID
				cout << "Boarding ID: " << res->getString(1) << ", Price: $" << res->getDouble(2) << ", Seat Number: " << res->getString(3) << ", Flight Number: " << res->getInt(4) << endl;
			}
			cout << "Enter the Boarding ID of the ticket that you want cancelled: ";
			cin >> boardingID;
			

			bool flag = false;
			res = stmt->executeQuery("SELECT boarding_ID FROM boarding_pass");
			while (res->next()) {
				if (res->getInt(1) == boardingID)
					flag = true;
			}
			while (flag == false)
			{ //makes sure the user enters in a valid boarding pass ID to delete
				flag = true;
				cout << "You have entered an invalid boarding id, please try again" << endl;
				cout << "Enter the Boarding ID of the ticket that you want cancelled: ";
				cin >> boardingID;

				res = stmt->executeQuery("SELECT boarding_ID FROM boarding_pass");
				while (res->next()) {
					if (res->getInt(1) == boardingID)
						flag = true;
				}
			}



			prep_stmt = con->prepareStatement("DELETE FROM boarding_pass WHERE boarding_ID = (?)");
			prep_stmt->setInt(1, boardingID);
			res = prep_stmt->executeQuery();
			cout << "Ticket has been cancelled" << endl;
		}
		else
		{
			cout << "This is the last thing that can be tried. Do you know the customer name? (y/n): ";
			cin >> ans;
			while (ans != 'y' && ans != 'n')
			{ //input validation
				cout << "Invalid input, please try again" << endl;
				cout << "Do you know the customer name? (y/n): ";
				cin >> ans;
			}
			if (ans == 'y')
			{ //allows user to type in any combinations of first, middle, and/or last names to look up the appropriate flight to be deleted
				string fName, mName, lName;
				cout << "If you don't know all the names (first/middle/last) please type in '-1'" << endl;
				cout << "What is the customer's first name: ";
				cin >> fName;
				cout << "What is the customer's middle name: ";
				cin >> mName;
				cout << "What is the customer's last name: ";
				cin >> lName;
				if (fName != "-1" && mName != "-1" && lName != "-1") //know everything
				{
					prep_stmt = con->prepareStatement("SELECT * FROM boarding_pass NATURAL JOIN customer WHERE firstName = (?) and middleName = (?) and lastName = (?)");
					prep_stmt->setString(1, fName);
					prep_stmt->setString(2, mName);
					prep_stmt->setString(3, lName);
					res = prep_stmt->executeQuery();
				}
				else if (fName != "-1" && mName != "-1" && lName == "-1") //don't know last name
				{
					prep_stmt = con->prepareStatement("SELECT * FROM boarding_pass NATURAL JOIN customer WHERE firstName = (?) and middleName = (?)");
					prep_stmt->setString(1, fName);
					prep_stmt->setString(2, mName);
					res = prep_stmt->executeQuery();
				}
				else if (fName != "-1" && mName == "-1" && lName != "-1") //don't know middle name
				{
					prep_stmt = con->prepareStatement("SELECT * FROM boarding_pass NATURAL JOIN customer WHERE firstName = (?) and lastName = (?)");
					prep_stmt->setString(1, fName);
					prep_stmt->setString(2, lName);
					res = prep_stmt->executeQuery();
				}
				else if (fName == "-1" && mName != "-1" && lName != "-1") //don't konw last name
				{
					prep_stmt = con->prepareStatement("SELECT * FROM boarding_pass NATURAL JOIN customer WHERE firstName = (?) and middleName = (?)");
					prep_stmt->setString(1, fName);
					prep_stmt->setString(2, mName);
					res = prep_stmt->executeQuery();
				}
				else if (fName != "-1" && mName == "-1" && lName == "-1") //only know first name
				{
					prep_stmt = con->prepareStatement("SELECT * FROM boarding_pass NATURAL JOIN customer WHERE firstName = (?) and middleName = (?) and lastName = (?)");
					prep_stmt->setString(1, fName);
					res = prep_stmt->executeQuery();
				}
				else if (fName == "-1" && mName != "-1" && lName == "-1") //only know middle name
				{
					prep_stmt = con->prepareStatement("SELECT * FROM boarding_pass NATURAL JOIN customer WHERE middleName = (?)");
					prep_stmt->setString(1, mName);
					res = prep_stmt->executeQuery();
				}
				else if (fName == "-1" && mName == "-1" && lName != "-1") //only know last name
				{
					prep_stmt = con->prepareStatement("SELECT * FROM boarding_pass NATURAL JOIN customer WHERE lastName = (?)");
					prep_stmt->setString(1, lName);
					res = prep_stmt->executeQuery();
				}
				else //know nothing (literally useless)
				{
					cout << "This is unhelpful, nothing can be done at this point, sorry" << endl;
					return;
				}
				while (res->next()) //pulls up with appropriate boarding passes
				{
					cout << "Customer ID: " << res->getInt(1) << ", Boarding ID: " << res->getInt(2) <<", Price: $" << res->getDouble(3) << ", Seat Number: " << res->getString(4) << ", Flight Number: " << res->getInt(5) << endl;
				}
				cout << "Enter the boarding_ID of the boarding pass you wish to cancel: ";
				cin >> boardingID;
				
				bool flag = false;
				res = stmt->executeQuery("SELECT boarding_ID FROM boarding_pass");
				while (res->next()) {
					if (res->getInt(1) == boardingID)
						flag = true; //input validation
				}
				while (flag == false)
				{
					flag = true;
					cout << "You have entered an invalid boarding id, please try again" << endl;
					cout << "Enter the Boarding ID of the ticket that you want cancelled: ";
					cin >> boardingID;

					res = stmt->executeQuery("SELECT boarding_ID FROM boarding_pass");
					while (res->next()) {
						if (res->getInt(1) == boardingID)
							flag = true;
					}
				}

				prep_stmt = con->prepareStatement("DELETE FROM boarding_pass WHERE boarding_ID = (?)");
				prep_stmt->setInt(1, boardingID);
				res = prep_stmt->executeQuery();
				cout << "Ticket has been cancelled" << endl;
			} //delete boarding pass
			else
			{ //when a boarding pass cannot be looked up
				cout << "Sorry, there is nothing that can be done at this point" << endl;
				return;
			}
		}
	}
}

void display()
{
	int flightID;
	bool flag = false;
	do
	{ //asks the user for a flight ID
		flag = false;
		cout << "Enter a flight ID: ";
		cin >> flightID;
		res = stmt->executeQuery("SELECT flight_ID FROM flight");
		while (res->next())
		{ //input validation
			if (res->getInt(1) == flightID)
				flag = true;
		}
		if (flag == false)
			cout << "Not a valid flight ID, please try again" << endl;
	} while (flag==false);
	
	
	prep_stmt = con->prepareStatement("SELECT customer_ID,firstName,middleName,lastName,flight_ID,dep_dest,dept_time,arrive_dest,arrive_time,seat_no from (customer NATURAL JOIN boarding_pass) JOIN flight USING (flight_ID) WHERE flight_ID = (?)");
	prep_stmt->setInt(1, flightID); //selects all the appropriate information on each boarding pass for the user's desired flight number
	res1=prep_stmt->executeQuery();
	while (res1->next()) {
		cout << "Customer ID: " << res1->getInt(1) << endl;
		cout << "Name: " << res1->getString(2) << " ";
		cout << res1->getString(3) << " " << res1->getString(4);
		cout << endl << "This customer is on the following flight: ";
		cout << "Flight ID: " << res1->getInt(5) << endl;
		cout << "Departure Airport: " << res1->getString(6);
		cout << ", Departure Time: " << res1->getString(7) << endl;
		cout << "Arrival Airport: " << res1->getString(8);
		cout << ", Arrival Time: " << res1->getString(9) << endl;
		cout << "On seat: " << res1->getString(10) << endl;
	}
}
