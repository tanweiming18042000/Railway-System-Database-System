#define _CRT_SECURE_NO_WARNINGS	// to let local time usable
#include <iostream>
#include <conio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <windows.h>
#include <iomanip>
#include <mysql.h>
#pragma comment(lib, "libmysql")


using namespace std;

// global variable
int qstate;
MYSQL* conn;
MYSQL_RES* res;
MYSQL_ROW row;
string adminID;
//string password;

class db_response
{
public:
	static void ConnectionFunction()
	{
		conn = mysql_init(0);
		if (conn)
		{
			cout << "Database Connected" << endl;
			system("cls");
		}
		else
		{
			cout << "Failed to connect" << mysql_errno(conn) << endl;
		}
		conn = mysql_real_connect(conn, "localhost", "root", "", "railway system database system", 3306, NULL, 0);
		if (conn)
		{
			cout << "Database connected to MySQL" << conn << endl;
			cout << "Press any key to continue..." << endl;
			//_getch();
			system("cls");
		}
		else
		{
			cout << "Failed to connect to MySQL" << mysql_errno(conn) << endl;
		}
	}
};

//declare function
void customerReceipt(string, string, string, string, string, string, string, string, string, string, double);
string userMenu();
char adminValid(char);
void adminMenu();
void controlMenu();
void updateMenu();
void customerValid();
string customerMenu();
void viewTrip(string local, int age);
void purchaseTicket(string IC, int age, string local);
void displayTripForPurchase(vector<double>, char, int, int , int);
void delPurchaseTicket(string IC);
void reportTicketList();

int main()
{
	system("cls");
	system("title Railway System Database System");
	db_response::ConnectionFunction();

	//need to insert administrative member if there is none
	string position = userMenu();
	char adminPass = 'n';
	do
	{
		if (position == "1")
		{
			do
			{
				char pass = adminValid(adminPass);
				if (pass == 'y')	// id and password accepted
				{
					adminMenu();
				}
				else if (pass == 'n')	// id and password unaccepted
				{
					adminPass = 'y';
				}	
			} while (adminPass == 'y');
		}
		else if (position == "2")
		{
			customerValid();
		}
		else if (position == "3")
		{
			cout << "Thank you!" << endl;
			break;
		}
		else
		{
			cout << "Wrong input! Try Again!" << endl;
		}
	} while (position != "1" && position != "2" && position != "3");
}

void customerReceipt(string IC, string purchaseTicketID, string purchaseTripID, string purchaseRailwayID, string purchaseDepartStation, string purchaseDestinatedStation, string purchaseSeat, string tripDate, string purchaseTimeDepart, string purchaseTimeArrive, double finalPrice)
{
	string curAdmin;
	// retrieve adminID from administrator table 
	string retrieveAdmin = "Select admin_ID from customer where customer_IC = '" + IC + "'";
	const char* ra = retrieveAdmin.c_str();
	qstate = mysql_query(conn, ra);

	if (!qstate) // query can proceed 
	{
		res = mysql_store_result(conn);
		if (res->row_count == 1)
		{
			while (row = mysql_fetch_row(res))
			{
				curAdmin = row[0];
			}
		}
	}

	cout << "***************************************************************" << endl;
	cout << "|                             Receipt                         |" << endl;
	cout << "***************************************************************" << endl;
	cout << "   IC            : " << setw(20) << left <<  IC <<   setw(15) << left << "Ticket ID: " << purchaseTicketID << endl;
	cout << "   Trip ID       : " << setw(20) << left << purchaseTripID << endl;
	cout << "   Railway ID    : " << setw(20) << left << purchaseRailwayID << setw(15) << left << "Seat: " << purchaseSeat << endl;
	cout << "   Date          : " << tripDate << endl;
	cout << "   Depart Station: " << purchaseDepartStation << " --> " << purchaseDestinatedStation << endl;
	cout << "   Depart Time   : " << setw(20) << purchaseTimeDepart << setw(15) << left << "Arrive Time: " << purchaseTimeArrive << endl;
	cout << "   Price         : RM " << setw(17) << left << finalPrice << setw(15) << left << "Admin ID: " << curAdmin << endl;
	cout << "***************************************************************" << endl;
}

string userMenu()
{
	string position = "";
	do
	{
		system("cls");
		cout << "******************************" << endl;
		cout << "|          Position          |" << endl;
		cout << "******************************" << endl;
		cout << "|        1. Admin            |" << endl;
		cout << "|        2. Customer         |" << endl;
		cout << "|        3. Exit             |" << endl;
		cout << "******************************" << endl;
		cout << "\nEnter your position numbering: ";
		cin >> position;
		cin.ignore(80, '\n');

		if (position != "1" && position != "2" && position != "3")
		{
			cout << "Invalid input\nEnter any key to try again..." << endl;
			_getch();
		}

	} while (position != "1" && position != "2" && position != "3");

	return position;
}

char adminValid(char adminPass)
{
	//string adminID;
	string password = "";

	// admin validation
	cout << "Admin ID: ";
	cin >> adminID;

	cout << "Password: ";
	cin >> password;

	//check from the database
	string checkAdmin = "Select * from Administrator where Admin_ID = '" + adminID + "' and Admin_Password = '" + password + "'";
	const char* adCheck = checkAdmin.c_str();
	qstate = mysql_query(conn, adCheck);

	if (!qstate) // query can proceed 
	{
		res = mysql_store_result(conn);
		if (res->row_count == 1)
		{
			while (row = mysql_fetch_row(res))
			{
				adminID = row[0];
				return ('y');
			}
		}
		else
		{
			adminPass = 'n';
			cout << "\nNo such Admin ID and Password being found!Try again? (y/n): ";
			cin >> adminPass;
			adminPass = tolower(adminPass);

			if(adminPass == 'n')
			{
				main();
			}
			else if (adminPass == 'y')
			{
				return ('n');
			}
		}
	}
}



void adminMenu()
{
	string adminChoice;
	do
	{
		//display adminMenu
		system("cls");

		cout << "**********************************************" << endl;
		cout << "|                 Admin Menu                 |" << endl;
		cout << "**********************************************" << endl;
		cout << "|        1. Admin Control                    |" << endl;
		cout << "|        2. Display purchase history         |" << endl;
		cout << "|        3. Generate new report              |" << endl;
		cout << "|        4. Exit                             |" << endl;
		cout << "**********************************************" << endl;
		
		cout << "\nEnter the admin option: ";
		cin >> adminChoice;
		cin.ignore(80, '\n');

		if (adminChoice == "1")
		{
			system("cls");
			controlMenu();
		}
		else if (adminChoice == "2")
		{
			system("cls");

			vector<string> purchaseTripID;
			vector<string> futurePurchaseTripID;
			vector<string> purchaseTripDate;
			string tripIDRange = "('";
			int year, month, day;
			string curDate, curMonth, curDay;

			// get date and only shows ticket that is in the future
			time_t now = time(0);
			tm* ltm = localtime(&now);

			year = 1900 + ltm->tm_year;
			month = 1 + ltm->tm_mon;
			day = ltm->tm_mday;

			if (month < 10)
			{
				curMonth = "0" + to_string(month);
			}
			else
			{
				curMonth = to_string(month);
			}

			if (day < 10)
			{
				curDay = "0" + to_string(day);
			}
			else
			{
				curDay = to_string(day);
			}

			curDate = to_string(year) + "-" + curMonth + "-" + curDay;

			// store the purchase trip ID into vector
			string retrieveTripID = "select DISTINCT Trip_ID from ticket";
			const char* rti = retrieveTripID.c_str();
			qstate = mysql_query(conn, rti);

			if (!qstate)
			{
				res = mysql_store_result(conn);
				if (res->row_count >= 1)
				{
					while ((row = mysql_fetch_row(res)))
					{
						purchaseTripID.push_back(row[0]);
					}
				}
			}
			
			// retrieve the trip ID's trip_date from trip table and compare to the curTime
			for (int i = 0; i < purchaseTripID.size(); i++)
			{
				string curTripID = purchaseTripID[i];
				string retrieveTripDate = "select trip_date from trip where trip_ID = '" + curTripID + "'";
				const char* rtd = retrieveTripDate.c_str();
				qstate = mysql_query(conn, rtd);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (res->row_count == 1)
					{
						//compare time then store into futurePurchaseTripID
						row = mysql_fetch_row(res);
						if (row[0] >= curDate)
						{
							futurePurchaseTripID.push_back(curTripID);
						}
					}
				}
			}
			
			// create a string range for the sql statement
			if (futurePurchaseTripID.size() > 0)
			{
				int curSize = 0;
				for(int i = 0; i < futurePurchaseTripID.size(); i++)
				{
					curSize++;
					tripIDRange = tripIDRange + futurePurchaseTripID[i] + "'";
					if (curSize != futurePurchaseTripID.size())
					{
						tripIDRange = tripIDRange + ", '";
					}
					else
					{
						tripIDRange = tripIDRange + ")";
					}
				}
			}
			
			// print ticket for display
			cout << "**********************************************" << endl;
			cout << "|               Purchase History              |" << endl;
			cout << "**********************************************" << endl;

			string viewPurchaseHistory = "select Ticket_ID, Trip_ID, Seat_Number, Date_Purchase, Final_Price from ticket where Trip_ID in " + tripIDRange;
			const char* vph = viewPurchaseHistory.c_str();
			qstate = mysql_query(conn, vph);

			if (!qstate)
			{
				res = mysql_store_result(conn);
				if (res->row_count >= 1)
				{
					cout << setw(15) << left << "Ticket ID" << setw(15) << left << "Trip ID" << setw(15)
						<< left << "Seat Number" << setw(15) << left << "Date Purchase" << setw(15) << left << "Price (RM)"
						<< endl;
					while ((row = mysql_fetch_row(res)))
					{
						cout << setw(15) << left << row[0] << setw(15) << left << row[1] << setw(15)
							<< left << row[2] << setw(15) << left << row[3] << setw(15) << left << row[4]
							<< endl;
					}
				}
				else
				{
					cout << "There are no ticket being purchased yet." << endl;
				}
			}
			cout << "Press any key to back to Admin Menu..." << endl;
			_getch();
			return adminMenu();
		}
		else if (adminChoice == "3")	// generate new report
		{
			string generateAgain;
			do
			{
				generateAgain = "n";
				vector<string> ticketTripDate;
				vector<string> uniqueDate;
				vector<string> uniqueYear;	// to check see the input year exist or not, if not will let user insert year again
				vector<int> uniqueYearInt;	// convert the year to int
				vector<string> ticketPrice;
				vector<double> ticketDouPrice;
				string reportType;
				int year = 0;
				int startMonth = 0, endMonth = 0;
				string startMonthStr, endMonthStr;
				int individualMonth = 0;
				string individualMonthStr;
				int numberOfWeek = 5;	// because all months got 5 weeks, except feb (which 4 weeks during normal year and 5 weeks during leap year) 
				int ticketNum = 0;
				double totalPrice = 0;
				int arrMon[12] = { 0 };
				string arrMonStr[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
				int arrWeek[5] = { 0 };
				char uniqueYes = 'n';	// to validate whether the input year is inside the ticket table
				vector<string> reportIDVec;
				string newReportID;
				string newReportType;
				string newRypeRange;
				string exportYes = "n";
				string exportName;

				// use the tripID from ticket to retrieve the tripDate list, during the sql, use ORDER BY Trip_date, store inside a vector
				string retrieveTicketTripDate = "select Trip_Date, final_Price from ticket natural join trip ORDER BY Trip_Date ASC";
				const char* rttd = retrieveTicketTripDate.c_str();
				qstate = mysql_query(conn, rttd);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (res->row_count >= 1)
					{
						while (row = mysql_fetch_row(res))
						{
							ticketTripDate.push_back(row[0]);
							ticketPrice.push_back(row[1]);
						}
					}
					else
					{
						cout << "There are no ticket being purchased yet, press any key to back to admin menu...";
						_getch();
						return adminMenu();
					}
				}

				// get the uniqueYear
				for (int i = 0; i < ticketTripDate.size(); i++)
				{
					uniqueDate.push_back(ticketTripDate[i]);
				}

				sort(uniqueDate.begin(), uniqueDate.end());
				uniqueDate.erase(unique(uniqueDate.begin(), uniqueDate.end()), uniqueDate.end());

				// get the first 4 element as year and store inside the uniqueYear
				for (int i = 0; i < uniqueDate.size(); i++)
				{
					string yearUnique = uniqueDate[i].substr(0, 4);
					stringstream gook(yearUnique);
					int curUniqueYear = 0;
					gook >> curUniqueYear;

					uniqueYearInt.push_back(curUniqueYear);
				}

				// make price vector double
				for (int i = 0; i < ticketPrice.size(); i++)
				{
					double curPrice = stod(ticketPrice[i]);
					ticketDouPrice.push_back(curPrice);
				}

				// ask for year base, months base or Per month base
				// after choose 1, let choose year, display the total ticket, year, total, then make point graph about the 12 month
				// after choose 2, let choose year and 2 month range, display the total ticket, month range, total, then make point graph about the months in range
				// after choose 3, let choose year and 1 month, display the total ticket, month, total, then make point graph about the 4 weeks
				// Save the results into txt file 
				do
				{
					system("cls");
					cout << "**********************************************" << endl;
					cout << "|                 Report Type                |" << endl;
					cout << "**********************************************" << endl;
					cout << "|        1. Yearly Report                    |" << endl;
					cout << "|        2. Months Report                    |" << endl;
					cout << "|        3. Monthly Report                   |" << endl;
					cout << "|        4. Exit                             |" << endl;
					cout << "**********************************************" << endl;


					cout << "Enter the report type: ";
					cin >> reportType;
					cin.ignore(20, '\n');

					if (reportType == "1" || reportType == "2" || reportType == "3")
					{
						reportTicketList();
					}

					if (reportType == "1")	// Year
					{
						newReportType = "Year";
						do
						{
							cout << "Enter the year: ";
							cin >> year;

							for (int i = 0; i < uniqueYearInt.size(); i++)
							{
								if (year == uniqueYearInt[i])
								{
									uniqueYes = 'y';
									break;
								}
							}

							if (uniqueYes == 'n')
							{
								cout << "No such year! Try Again!" << endl;
							}
						} while (uniqueYes == 'n');

						// use the vector and retrieve the first 4 substring from it, see year, if yes ticketNum++, totalPrice add, and see the month, then only add to month
						for (int i = 0; i < ticketTripDate.size(); i++)
						{
							string yearSub = ticketTripDate[i].substr(0, 4);
							stringstream geek(yearSub);
							int curYearSub = 0;
							geek >> curYearSub;
							if (curYearSub == year)
							{
								ticketNum++;
								totalPrice += ticketDouPrice[i];
								// month, take substr(5, 2)
								string monthSub = ticketTripDate[i].substr(5, 2);
								stringstream gek(monthSub);
								int curMonthSub = 0;
								gek >> curMonthSub;
								arrMon[curMonthSub - 1]++;
							}
						}


						// display report first, then the graph
						system("cls");
						cout << "**********************************************" << endl;
						cout << "|                 Annual Report              |" << endl;
						cout << "**********************************************" << endl;
						cout << endl;
						cout << setw(20) << left << "Year" << setw(5) << right << ": " << year << endl;
						cout << setw(20) << left << "Total ticket sales" << setw(5) << right << ": " << ticketNum << endl;
						cout << setw(20) << left << "Total price" << setw(7) << right << ": RM" << totalPrice << endl;

						for (int i = 0; i < 3; i++)
						{
							cout << endl;
						}

						cout << "************************" << endl;
						cout << "|         Graph        |" << endl;
						cout << "************************" << endl;
						cout << endl;
						cout << setw(8) << right << "Month" << endl;
						cout << setw(7) << right << "/ \\" << endl;
						for (int i = 0; i < 12; i++)
						{
							cout << setw(5) << left << arrMonStr[i] << left << "|";
							for (int j = 0; j < arrMon[i]; j++)
							{
								cout << "*";
							}
							if (arrMon[i] != 0)
							{
								cout << " " << arrMon[i] << endl;
							}
							else
							{
								cout << endl;
							}
						}

						// find the max of the montharray to print the underline
						cout << setw(6) << right;
						int* maxNum;
						maxNum = max_element(arrMon, arrMon + 12);
						for (int i = 0; i < *maxNum + 5; i++)
						{
							cout << "-";
						}
						cout << ">" << endl;

						// retrieve the reportID and ascend it
						string retrieveReportID = "select report_ID from report ORDER BY report_ID";
						const char* rri = retrieveReportID.c_str();
						qstate = mysql_query(conn, rri);

						if (!qstate)
						{
							res = mysql_store_result(conn);
							if (res->row_count >= 1)
							{
								while ((row = mysql_fetch_row(res)))
								{
									reportIDVec.push_back(row[0]);
								}
							}
						}

						int curNum;

						if (reportIDVec.size() != 0)
						{
							string curReportID = reportIDVec.back();
							curReportID = curReportID.substr(1, 3);
							stringstream gek(curReportID);
							gek >> curNum;
						}
						else
						{
							curNum = 0;
						}
						curNum++;

						if (curNum >= 100)
						{
							newReportID = "R" + to_string(curNum);
						}
						else if (curNum >= 10)
						{
							newReportID = "R0" + to_string(curNum);
						}
						else
						{
							newReportID = "R00" + to_string(curNum);
						}

						// insert the reportID, adminID, Report_Type, Type_Range, Total_Ticket, total_sales into the report table 
						string insertReportRow = "insert into report VALUES('" + newReportID + "', '" + adminID + "', '" + newReportType + "', '" + to_string(year) + "', '" + to_string(ticketNum) + "', '" + to_string(totalPrice) + "')";
						const char* irr = insertReportRow.c_str();
						qstate = mysql_query(conn, irr);

						
						for (int i = 0; i < 3; i++)
						{
							cout << endl;
						}

						do
						{
							cout << "Do you want to export the report? (Y/N): ";
							cin >> exportYes;

							if (exportYes != "Y" && exportYes != "y" && exportYes != "N" && exportYes != "n")
							{
								cout << "Wrong input! Try again!" << endl;
							}
						} while (exportYes != "Y" && exportYes != "y" && exportYes != "N" && exportYes != "n");

						// get txt file name, open it and export it
						if (exportYes == "Y" || exportYes == "y")
						{
							cout << "Enter the name for the txt file: ";
							cin.ignore(80, '\n');
							getline(cin, exportName);

							ofstream fout;
							fout.open(exportName + ".txt");


							fout << "**********************************************" << endl;
							fout << "|                 Annual Report              |" << endl;
							fout << "**********************************************" << endl;
							fout << endl;
							fout << setw(20) << left << "Year" << setw(5) << right << ": " << year << endl;
							fout << setw(20) << left << "Total ticket sales" << setw(5) << right << ": " << ticketNum << endl;
							fout << setw(20) << left << "Total price" << setw(7) << right << ": RM" << totalPrice << endl;

							for (int i = 0; i < 3; i++)
							{
								fout << endl;
							}

							fout << "************************" << endl;
							fout << "|         Graph        |" << endl;
							fout << "************************" << endl;
							fout << endl;
							fout << setw(8) << right << "Month" << endl;
							fout << setw(7) << right << "/ \\" << endl;
							for (int i = 0; i < 12; i++)
							{
								fout << setw(5) << left << arrMonStr[i] << left << "|";
								for (int j = 0; j < arrMon[i]; j++)
								{
									fout << "*";
								}
								if (arrMon[i] != 0)
								{
									fout << " " << arrMon[i] << endl;
								}
								else
								{
									fout << endl;
								}
							}

							// find the max of the montharray to print the underline
							fout << setw(6) << right;
							int* maxNum;
							maxNum = max_element(arrMon, arrMon + 12);
							for (int i = 0; i < *maxNum + 5; i++)
							{
								fout << "-";
							}
							fout << ">" << endl;

							fout.close();
						}
						
						

						for (int i = 0; i < 3; i++)
						{
							cout << endl;
						}

						do
						{
							cout << "Do you want to generate another report? (Y/N): ";
							cin >> generateAgain;

							if (generateAgain == "Y" || generateAgain == "y")
							{
								generateAgain = "y";
							}
							else if (generateAgain == "N" || generateAgain == "n")
							{
								cout << "Enter any key to back to admin menu..." << endl;
								_getch();
								return adminMenu();
							}
							else
							{
								cout << "Wrong input! Try again!" << endl;
							}
						} while (generateAgain != "Y" && generateAgain != "y" && generateAgain != "N" && generateAgain != "n");
					}
					else if (reportType == "2")	// Months
					{
						newReportType = "Months";
						do
						{
							cout << "Enter the year: ";
							cin >> year;

							for (int i = 0; i < uniqueYearInt.size(); i++)
							{
								if (year == uniqueYearInt[i])
								{
									uniqueYes = 'y';
									break;
								}
							}

							if (uniqueYes == 'n')
							{
								cout << "No such year! Try Again!" << endl;
							}
						} while (uniqueYes == 'n');

						do
						{
							do
							{
								cout << "Enter the starting month (1 - 12): ";
								cin >> startMonth;

								if (startMonth < 1 && startMonth > 12)
								{
									cout << "Start month can only between 1- 12! Try again!" << endl;
								}
							} while (startMonth < 1 && startMonth > 12);

							do
							{
								cout << "Enter the last month (1 - 12): ";
								cin >> endMonth;


								if (endMonth < 1 && endMonth > 12)
								{
									cout << "Last month can only between 1- 12! Try again!" << endl;
								}
							} while (endMonth < 1 && endMonth > 12);

							if (startMonth >= endMonth)
							{
								cout << "The starting month must be smaller than last month! Try again!" << endl;
							}
						} while (startMonth >= endMonth);

						// check the year and month is in range of (startMonth, endMonth), if yes, the arrMonth is added and totalTicket, totalPrice is added
						for (int i = 0; i < ticketTripDate.size(); i++)
						{
							string yearSub = ticketTripDate[i].substr(0, 4);
							stringstream geek(yearSub);
							int curYearSub = 0;
							geek >> curYearSub;

							if (curYearSub == year)
							{
								// month, take substr(5, 2)
								string monthSub = ticketTripDate[i].substr(5, 2);
								stringstream gek(monthSub);
								int curMonthSub = 0;
								gek >> curMonthSub;

								// if curMonthSub >= startMonth && curMonthSub <= endMonth, then only add
								if (curMonthSub >= startMonth && curMonthSub <= endMonth)
								{
									ticketNum++;
									totalPrice += ticketDouPrice[i];
									arrMon[curMonthSub - 1]++;
								}
							}
						}

						// display report first, then the graph
						system("cls");
						cout << "**********************************************" << endl;
						cout << "|                 Months Report              |" << endl;
						cout << "**********************************************" << endl;
						cout << endl;
						cout << setw(20) << left << "Start Month" << setw(5) << right << ": " << arrMonStr[startMonth - 1] << endl;
						cout << setw(20) << left << "End Month" << setw(5) << right << ": " << arrMonStr[endMonth - 1] << endl;
						cout << setw(20) << left << "Total ticket sales" << setw(5) << right << ": " << ticketNum << endl;
						cout << setw(20) << left << "Total price" << setw(7) << right << ": RM" << totalPrice << endl;

						for (int i = 0; i < 3; i++)
						{
							cout << endl;
						}

						cout << "************************" << endl;
						cout << "|         Graph        |" << endl;
						cout << "************************" << endl;
						cout << endl;
						cout << setw(8) << right << "Month" << endl;
						cout << setw(7) << right << "/ \\" << endl;
						for (int i = startMonth - 1; i < endMonth; i++)
						{
							cout << setw(5) << left << arrMonStr[i] << left << "|";
							for (int j = 0; j < arrMon[i]; j++)
							{
								cout << "*";
							}
							if (arrMon[i] != 0)
							{
								cout << " " << arrMon[i] << endl;
							}
							else
							{
								cout << endl;
							}
						}

						// find the max of the montharray to print the underline
						cout << setw(6) << right;
						int* maxNum;
						maxNum = max_element(arrMon, arrMon + 12);
						for (int i = 0; i < *maxNum + 5; i++)
						{
							cout << "-";
						}
						cout << ">" << endl;

						// retrieve the reportID and ascend it
						string retrieveReportID = "select report_ID from report ORDER BY report_ID";
						const char* rri = retrieveReportID.c_str();
						qstate = mysql_query(conn, rri);

						if (!qstate)
						{
							res = mysql_store_result(conn);
							if (res->row_count >= 1)
							{
								while ((row = mysql_fetch_row(res)))
								{
									reportIDVec.push_back(row[0]);
								}
							}
						}

						int curNum;

						if (reportIDVec.size() != 0)
						{
							string curReportID = reportIDVec.back();
							curReportID = curReportID.substr(1, 3);
							stringstream gek(curReportID);
							gek >> curNum;
						}
						else
						{
							curNum = 0;
						}
						curNum++;

						if (curNum >= 100)
						{
							newReportID = "R" + to_string(curNum);
						}
						else if (curNum >= 10)
						{
							newReportID = "R0" + to_string(curNum);
						}
						else
						{
							newReportID = "R00" + to_string(curNum);
						}

						if (startMonth < 10)
						{
							startMonthStr = "0" + to_string(startMonth);
						}
						else
						{
							startMonthStr = to_string(startMonth);
						}

						if (endMonth < 10)
						{
							endMonthStr = "0" + to_string(endMonth);
						}
						else
						{
							endMonthStr = to_string(endMonth);
						}

						string newTypeRange = startMonthStr + "->" + endMonthStr + ", " + to_string(year);

						// insert the reportID, adminID, Report_Type, Type_Range, Total_Ticket, total_sales into the report table 
						string insertReportRow = "insert into report VALUES('" + newReportID + "', '" + adminID + "', '" + newReportType + "', '" + newTypeRange + "', '" + to_string(ticketNum) + "', '" + to_string(totalPrice) + "')";
						const char* irr = insertReportRow.c_str();
						qstate = mysql_query(conn, irr);

						// export the file
						do
						{
							cout << "Do you want to export the report? (Y/N): ";
							cin >> exportYes;
							cin.ignore(20, '\n');

							if (exportYes != "Y" && exportYes != "y" && exportYes != "N" && exportYes != "n")
							{
								cout << "Wrong input! Try again!" << endl;
							}
						} while (exportYes != "Y" && exportYes != "y" && exportYes != "N" && exportYes != "n");

						if (exportYes == "Y" || exportYes == "y")
						{
							cout << "Enter the name for the txt file: ";
							cin.ignore(80, '\n');
							getline(cin, exportName);

							ofstream fout;
							fout.open(exportName + ".txt");

							fout << "**********************************************" << endl;
							fout << "|                 Months Report              |" << endl;
							fout << "**********************************************" << endl;
							fout << endl;
							fout << setw(20) << left << "Start Month" << setw(5) << right << ": " << arrMonStr[startMonth - 1] << endl;
							fout << setw(20) << left << "End Month" << setw(5) << right << ": " << arrMonStr[endMonth - 1] << endl;
							fout << setw(20) << left << "Total ticket sales" << setw(5) << right << ": " << ticketNum << endl;
							fout << setw(20) << left << "Total price" << setw(7) << right << ": RM" << totalPrice << endl;

							for (int i = 0; i < 3; i++)
							{
								fout << endl;
							}

							fout << "************************" << endl;
							fout << "|         Graph        |" << endl;
							fout << "************************" << endl;
							fout << endl;
							fout << setw(8) << right << "Month" << endl;
							fout << setw(7) << right << "/ \\" << endl;
							for (int i = startMonth - 1; i < endMonth; i++)
							{
								fout << setw(5) << left << arrMonStr[i] << left << "|";
								for (int j = 0; j < arrMon[i]; j++)
								{
									fout << "*";
								}
								if (arrMon[i] != 0)
								{
									fout << " " << arrMon[i] << endl;
								}
								else
								{
									fout << endl;
								}
							}

							// find the max of the montharray to print the underline
							fout << setw(6) << right;
							int* maxNum;
							maxNum = max_element(arrMon, arrMon + 12);
							for (int i = 0; i < *maxNum + 5; i++)
							{
								fout << "-";
							}
							fout << ">" << endl;
							fout.close();
						}
						
						
						for (int i = 0; i < 3; i++)
						{
							cout << endl;
						}

						do
						{
							cout << "Do you want to generate another report? (Y/N): ";
							cin >> generateAgain;
							cin.ignore(20, '\n');

							if (generateAgain == "Y" || generateAgain == "y")
							{
								generateAgain = "y";
							}
							else if (generateAgain == "N" || generateAgain == "n")
							{
								cout << "Enter any key to back to admin menu..." << endl;
								_getch();
								return adminMenu();
							}
							else
							{
								cout << "Wrong input! Try again!" << endl;
							}
						} while (generateAgain != "Y" && generateAgain != "y" && generateAgain != "N" && generateAgain != "n");
					}
					else if (reportType == "3")	// Per Month
					{
						newReportType = "Month";
						do
						{
							cout << "Enter the year: ";
							cin >> year;

							for (int i = 0; i < uniqueYearInt.size(); i++)
							{
								if (year == uniqueYearInt[i])
								{
									uniqueYes = 'y';
									break;
								}
							}

							if (uniqueYes == 'n')
							{
								cout << "No such year! Try Again!" << endl;
							}
						} while (uniqueYes == 'n');

						
						do
						{
							cout << "Enter the month (1 - 12): ";
							cin >> individualMonth;

							if (individualMonth < 1 || individualMonth > 12)
							{
								cout << "No such month! Try again!" << endl;
							}
						} while (individualMonth < 1 || individualMonth > 12);

						// check the year and month, if yes, the arrMonth is added and totalTicket, totalPrice is added
						for (int i = 0; i < ticketTripDate.size(); i++)
						{
							string yearSub = ticketTripDate[i].substr(0, 4);
							stringstream geek(yearSub);
							int curYearSub = 0;
							geek >> curYearSub;

							if (curYearSub == year)
							{
								// month, take substr(5, 2)
								string monthSub = ticketTripDate[i].substr(5, 2);
								stringstream gek(monthSub);
								int curMonthSub = 0;
								gek >> curMonthSub;

								string daySub = ticketTripDate[i].substr(8, 2);
								stringstream geok(daySub);
								int curDaySub = 0;
								geok >> curDaySub;

								// if curMonthSub == individual month then only add
								if (curMonthSub == individualMonth)
								{
									ticketNum++;
									totalPrice += ticketDouPrice[i];

									// if curDaySub <= 7, week 1
									// curDaySub > 7, <= 14, week 2
									// curDaySub > 14, <= 21, week 3
									// curDaySub > 21, <= 28, week 4
									// else, week 5
									if (curDaySub <= 7)
									{
										arrWeek[0]++;
									}
									else if (curDaySub > 7 && curDaySub <= 14)
									{
										arrWeek[1]++;
									}
									else if (curDaySub > 14 && curDaySub <= 21)
									{
										arrWeek[2]++;
									}
									else if (curDaySub > 21 && curDaySub <= 28)
									{
										arrWeek[3]++;
									}
									else
									{
										arrWeek[4]++;
									}
								}
							}
						}

						// display report first, then the graph
						system("cls");
						cout << "**********************************************" << endl;
						cout << "|                Monthly Report              |" << endl;
						cout << "**********************************************" << endl;
						cout << endl;
						cout << setw(20) << left << "Month" << setw(5) << right << ": " << arrMonStr[individualMonth - 1] << endl;
						cout << setw(20) << left << "Total ticket sales" << setw(5) << right << ": " << ticketNum << endl;
						cout << setw(20) << left << "Total price" << setw(7) << right << ": RM" << totalPrice << endl;

						for (int i = 0; i < 3; i++)
						{
							cout << endl;
						}

						// calculate the number of weeks
						if (individualMonth == 2)	// feb
						{
							if (year % 4 == 0)	// leap year
							{
								numberOfWeek = 5;
							}
							else
							{
								numberOfWeek = 4;
							}
						}

						cout << "************************" << endl;
						cout << "|         Graph        |" << endl;
						cout << "************************" << endl;
						cout << endl;
						cout << setw(9) << right << "Weeks" << endl;
						cout << setw(8) << right << "/ \\" << endl;
						for (int i = 0; i < numberOfWeek; i++)
						{
							cout << setw(5) << left << "Week " << i + 1 << left << "|";
							for (int j = 0; j < arrWeek[i]; j++)
							{
								cout << "*";
							}
							if (arrWeek[i] != 0)
							{
								cout << " " << arrWeek[i] << endl;
							}
							else
							{
								cout << endl;
							}
						}

						// find the max of the montharray to print the underline
						cout << setw(7) << right;
						int* maxNum;
						maxNum = max_element(arrWeek, arrWeek + 5);
						for (int i = 0; i < *maxNum + 5; i++)
						{
							cout << "-";
						}
						cout << ">" << endl;


						// retrieve the reportID and ascend it
						string retrieveReportID = "select report_ID from report ORDER BY report_ID";
						const char* rri = retrieveReportID.c_str();
						qstate = mysql_query(conn, rri);

						if (!qstate)
						{
							res = mysql_store_result(conn);
							if (res->row_count >= 1)
							{
								while ((row = mysql_fetch_row(res)))
								{
									reportIDVec.push_back(row[0]);
								}
							}
						}

						int curNum;

						if (reportIDVec.size() != 0)
						{
							string curReportID = reportIDVec.back();
							curReportID = curReportID.substr(1, 3);
							stringstream gek(curReportID);
							gek >> curNum;
						}
						else
						{
							curNum = 0;
						}
						curNum++;

						if (curNum >= 100)
						{
							newReportID = "R" + to_string(curNum);
						}
						else if (curNum >= 10)
						{
							newReportID = "R0" + to_string(curNum);
						}
						else
						{
							newReportID = "R00" + to_string(curNum);
						}

						if (individualMonth < 10)
						{
							individualMonthStr = "0" + to_string(individualMonth);
						}
						else
						{
							individualMonthStr = to_string(individualMonth);
						}

						string newTypeRange = individualMonthStr + ", " + to_string(year);

						// insert the reportID, adminID, Report_Type, Type_Range, Total_Ticket, total_sales into the report table 
						string insertReportRow = "insert into report VALUES('" + newReportID + "', '" + adminID + "', '" + newReportType + "', '" + newTypeRange + "', '" + to_string(ticketNum) + "', '" + to_string(totalPrice) + "')";
						const char* irr = insertReportRow.c_str();
						qstate = mysql_query(conn, irr);

						// export the file
						do
						{
							cout << "Do you want to export the report? (Y/N): ";
							cin >> exportYes;

							if (exportYes != "Y" && exportYes != "y" && exportYes != "N" && exportYes != "n")
							{
								cout << "Wrong input! Try again!" << endl;
							}
						} while (exportYes != "Y" && exportYes != "y" && exportYes != "N" && exportYes != "n");

						if (exportYes == "Y" || exportYes == "y")
						{
							cout << "Enter the name for the txt file: ";
							cin.ignore(80, '\n');
							getline(cin, exportName);

							ofstream fout;
							fout.open(exportName + ".txt");
							fout << "**********************************************" << endl;
							fout << "|                Monthly Report              |" << endl;
							fout << "**********************************************" << endl;
							fout << endl;
							fout << setw(20) << left << "Month" << setw(5) << right << ": " << arrMonStr[individualMonth - 1] << endl;
							fout << setw(20) << left << "Total ticket sales" << setw(5) << right << ": " << ticketNum << endl;
							fout << setw(20) << left << "Total price" << setw(7) << right << ": RM" << totalPrice << endl;

							for (int i = 0; i < 3; i++)
							{
								fout << endl;
							}

							fout << "************************" << endl;
							fout << "|         Graph        |" << endl;
							fout << "************************" << endl;
							fout << endl;
							fout << setw(9) << right << "Weeks" << endl;
							fout << setw(8) << right << "/ \\" << endl;
							for (int i = 0; i < numberOfWeek; i++)
							{
								fout << setw(5) << left << "Week " << i + 1 << left << "|";
								for (int j = 0; j < arrWeek[i]; j++)
								{
									fout << "*";
								}
								if (arrWeek[i] != 0)
								{
									fout << " " << arrWeek[i] << endl;
								}
								else
								{
									fout << endl;
								}
							}

							// find the max of the montharray to print the underline
							fout << setw(7) << right;
							int* maxNum;
							maxNum = max_element(arrWeek, arrWeek + 5);
							for (int i = 0; i < *maxNum + 5; i++)
							{
								fout << "-";
							}
							fout << ">" << endl;

							fout.close();
						}


						for (int i = 0; i < 3; i++)
						{
							cout << endl;
						}

						do
						{
							cout << "Do you want to generate another report? (Y/N): ";
							cin >> generateAgain;

							if (generateAgain == "Y" || generateAgain == "y")
							{
								generateAgain = "y";
							}
							else if (generateAgain == "N" || generateAgain == "n")
							{
								cout << "Enter any key to back to admin menu..." << endl;
								_getch();
								return adminMenu();
							}
							else
							{
								cout << "Wrong input! Try again!" << endl;
							}
						} while (generateAgain != "Y" && generateAgain != "y" && generateAgain != "N" && generateAgain != "n");
					}
					else if (reportType == "4")
					{
						cout << "Enter any key to back to admin menu..." << endl;
						_getch();
						return adminMenu();
					}
					else
					{
						cout << "Wrong input! Enter any key to try Again..." << endl;
						_getch();
						system("cls");
					}
				} while (reportType != "1" && reportType != "2" && reportType != "3" && reportType != "4");
			}while (generateAgain == "Y" || generateAgain == "y");
		}
		else if (adminChoice == "4")
		{
			main();
		}
		else
		{
			cout << "Wrong input! Press any key to try again..." << endl;
			_getch();
		}
	} while (adminChoice != "1" && adminChoice != "2" && adminChoice != "3" && adminChoice != "4");
}


void controlMenu()
{
	string controlChoice;
	do
	{
		do
		{
			system("cls");

			cout << "**********************************************" << endl;
			cout << "|                Control Menu                |" << endl;
			cout << "**********************************************" << endl;
			cout << "|        1. Update                           |" << endl;
			cout << "|        2. Delete purchase history          |" << endl;
			cout << "|        3. Delete trip                      |" << endl;
			cout << "|        4. Delete railway transport         |" << endl;
			cout << "|        5. Delete driver                    |" << endl;
			cout << "|        6. Exit                             |" << endl;
			cout << "**********************************************" << endl;
		

			cout << "\nEnter the control option: ";
			cin >> controlChoice;
			cin.ignore(20, '\n');

			if (controlChoice != "1" && controlChoice != "2" && controlChoice != "3" && controlChoice != "4" && controlChoice != "5" && controlChoice != "6")
			{
				cout << "Wrong input! Enter any key to try again..." << endl;
				_getch();
			}
		} while (controlChoice != "1" && controlChoice != "2" && controlChoice != "3" && controlChoice != "4" && controlChoice != "5" && controlChoice != "6");

		if (controlChoice == "1")
		{
			system("cls");
			return updateMenu();
		}
		else if (controlChoice == "2")
		{
			string delAgain;
			string delRecord;
			string delTicket;
			char matchTicket = 'n';
			vector<string> purchaseTripID;
			vector<string> futurePurchaseTripID;
			vector<string> purchaseTripDate;
			vector<string> purchaseTicketID;
			string tripIDRange = "('";
			int year, month, day;
			string curDate, curMonth, curDay;

			do
			{
				// use the tripID in TICKET table to retrieve date in TRIP table
					//display all the stations
				do
				{
					system("cls");

					// get date and only shows ticket that is in the future
					time_t now = time(0);
					tm* ltm = localtime(&now);

					year = 1900 + ltm->tm_year;
					month = 1 + ltm->tm_mon;
					day = ltm->tm_mday;

					if (month < 10)
					{
						curMonth = "0" + to_string(month);
					}
					else
					{
						curMonth = to_string(month);
					}

					if (day < 10)
					{
						curDay = "0" + to_string(day);
					}
					else
					{
						curDay = to_string(day);
					}

					curDate = to_string(year) + "-" + curMonth + "-" + curDay;

					// store the purchase trip ID into vector
					string retrieveTripID = "select DISTINCT Trip_ID from ticket";
					const char* rti = retrieveTripID.c_str();
					qstate = mysql_query(conn, rti);

					if (!qstate)
					{
						res = mysql_store_result(conn);
						if (res->row_count >= 1)
						{
							while ((row = mysql_fetch_row(res)))
							{
								purchaseTripID.push_back(row[0]);
							}
						}
					}

					// retrieve the trip ID's trip_date from trip table and compare to the curTime
					for (int i = 0; i < purchaseTripID.size(); i++)
					{
						string curTripID = purchaseTripID[i];
						string retrieveTripDate = "select trip_date from trip where trip_ID = '" + curTripID + "'";
						const char* rtd = retrieveTripDate.c_str();
						qstate = mysql_query(conn, rtd);

						if (!qstate)
						{
							res = mysql_store_result(conn);
							if (res->row_count == 1)
							{
								//compare time then store into futurePurchaseTripID
								row = mysql_fetch_row(res);
								if (row[0] >= curDate)
								{
									futurePurchaseTripID.push_back(curTripID);
								}
							}
						}
					}

					// create a string range for the sql statement
					if (futurePurchaseTripID.size() > 0)
					{
						int curSize = 0;
						for (int i = 0; i < futurePurchaseTripID.size(); i++)
						{
							curSize++;
							tripIDRange = tripIDRange + futurePurchaseTripID[i] + "'";
							if (curSize != futurePurchaseTripID.size())
							{
								tripIDRange = tripIDRange + ", '";
							}
							else
							{
								tripIDRange = tripIDRange + ")";
							}
						}
					}

					// print ticket for display
					cout << "**********************************************" << endl;
					cout << "|               Purchase History             |" << endl;
					cout << "**********************************************" << endl;

					string viewPurchaseHistory = "select Ticket_ID, Trip_ID, Seat_Number, Date_Purchase, Final_Price from ticket where Trip_ID in " + tripIDRange;
					const char* vph = viewPurchaseHistory.c_str();
					qstate = mysql_query(conn, vph);

					if (!qstate)
					{
						res = mysql_store_result(conn);
						if (res->row_count >= 1)
						{
							cout << setw(15) << left << "Ticket ID" << setw(15) << left << "Trip ID" << setw(15)
								<< left << "Seat Number" << setw(15) << left << "Date Purchase" << setw(15) << left << "Price (RM)"
								<< endl;
							while ((row = mysql_fetch_row(res)))
							{
								cout << setw(15) << left << row[0] << setw(15) << left << row[1] << setw(15)
									<< left << row[2] << setw(15) << left << row[3] << setw(15) << left << row[4]
									<< endl;
								purchaseTicketID.push_back(row[0]);
							}
						}
						else
						{
							cout << "There are no ticket being purchased yet." << endl;
							cout << "Press any key to back to Control Menu..." << endl;
							_getch();
							return controlMenu();
						}
					}


					for (int i = 0; i < 3; i++)
					{
						cout << endl;
					}


					cout << "Do you want to delete any purchase record? (y/n): ";
					cin >> delRecord;
					cin.ignore(80, '\n');

					// ask the date to delete
					if (delRecord == "Y" || delRecord == "y")
					{
						do
						{
							cout << "Enter the Ticket ID to delete: ";
							cin >> delTicket;

							// validate the delTicket
							for (int i = 0; i < purchaseTicketID.size(); i++)
							{
								if (delTicket == purchaseTicketID[i])
								{
									matchTicket = 'y';
									break;
								}
							}

							if (matchTicket == 'n')
							{
								cout << "No such Ticket ID, Try Again!" << endl;
							}
						} while (matchTicket == 'n');

						// delete from Ticket table
						string delTicketID = "delete from Ticket where ticket_ID = '" + delTicket + "'";
						const char* dti = delTicketID.c_str();
						qstate = mysql_query(conn, dti);

					}
					else if (delRecord == "N" || delRecord == "n")
					{
						cout << "Enter any key to back to control menu..." << endl;
						_getch();
						return controlMenu();
					}
					else
					{
						cout << "Wrong input! Try again!" << endl;
					}
				} while (delRecord != "Y" && delRecord != "y" && delRecord != "N" && delRecord != "n");

				cout << "Do you want to delete another record? (y/n): ";
				cin >> delAgain;
				cin.ignore(80, '\n');

				if (delAgain != "Y" && delAgain != "y" && delAgain != "N" && delAgain != "n")
				{
					cout << "Wrong input! Try again!" << endl;
				}

			} while (delAgain == "Y" || delAgain == "y");
			return controlMenu();
		}
		else if (controlChoice == "3")
		{
			string delAgain;
			do
			{
				string delTrip;
				string tripID;
				vector<string> listTripID;
				char matchTrip = 'n';

				system("cls");

				cout << "**********************************************" << endl;
				cout << "|                 Trip List                  |" << endl;
				cout << "**********************************************" << endl;

				string viewTripList = "select * from trip";
				const char* vtr = viewTripList.c_str();
				qstate = mysql_query(conn, vtr);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (res->row_count >= 1)
					{
						cout << setw(15) << left << "Trip ID" << setw(15) << left << "Railway ID" << setw(20)
							<< left << "Departed Station" << setw(20) << left << "Destinate Station" << setw(15) << left << "Trip Date" << setw(15) << left << "Time Depart"
							<< setw(15) << left << "Time Arrive" << setw(20) << left << "Duration (minutes)" << setw(15) << left << "Price (RM)" << endl;
						while ((row = mysql_fetch_row(res)))
						{
							cout << setw(15) << left << row[0] << setw(15) << left << row[1] << setw(20)
								<< left << row[2] << setw(20) << left << row[3] << setw(15) << left << row[4] << setw(15) << left << row[5]
								<< setw(15) << left << row[6] << setw(15) << left << row[7] << setw(20) << left << row[8] << endl;
							listTripID.push_back(row[0]);
						}
					}
					else
					{
						cout << "There are no trip inserted yet." << endl;
						cout << "Enter to back to control menu..." << endl;
						_getch();
						return controlMenu();
					}

					for (int i = 0; i < 3; i++)
					{
						cout << endl;
					}

					do
					{
						cout << "Do you want to delete any trip? (y/n): ";
						cin >> delTrip;
						cin.ignore(80, '\n');
						

						if (delTrip == "Y" || delTrip == "y")
						{
							cout << endl;
							do
							{
								cout << "Enter the Trip ID to delete: ";
								cin >> tripID;

								for (int i = 0; i < listTripID.size(); i++)
								{
									if (tripID == listTripID[i])
									{
										matchTrip = 'y';
										break;
									}
								}

								if (matchTrip == 'n')
								{
									cout << "No such Trip ID! Try Again!" << endl;
								}
							} while (matchTrip == 'n');

							string delTripID = "delete from trip where trip_ID = '" + tripID + "'";
							const char* dti = delTripID.c_str();
							qstate = mysql_query(conn, dti);

						}
						else if (delTrip == "N" || delTrip == "n")
						{
							cout << "Enter any key to back to control menu..." << endl;
							_getch();
							return controlMenu();
						}
						else
						{
							cout << "Wrong input!" << endl;

						}
					} while (delTrip != "y" && delTrip != "Y" && delTrip != "n" && delTrip != "N");

					do
					{
						cout << "Do you want to delete another trip? (y/n) : ";
						cin >> delAgain;
						cin.ignore(80, '\n');
						if (delAgain == "N" || delAgain == "n")
						{
							return controlMenu();
						}
					} while (delAgain != "Y" && delAgain != "y" && delAgain != "N" && delAgain != "n");
				}
			} while (delAgain == "Y" || delAgain == "y");
		}
		else if (controlChoice == "4")	// delete railway transport
		{
			string delAgain;
			do
			{
				string delTransport;
				string transportID;
				vector<string> railwayIDVec;

				system("cls");

				// store the railway ID into vector for validation
				string selectRailwayID = "select railway_ID from railway_transport";
				const char* sri = selectRailwayID.c_str();
				qstate = mysql_query(conn, sri);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (res->row_count >= 1)
					{
						while (row = mysql_fetch_row(res))
						{
							railwayIDVec.push_back(row[0]);
						}
					}
				}

				cout << "**********************************************" << endl;
				cout << "|              Railway Transport             |" << endl;
				cout << "**********************************************" << endl;

				string viewTransport = "select * from railway_transport";
				const char* vt = viewTransport.c_str();
				qstate = mysql_query(conn, vt);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (res->row_count >= 1)
					{
						cout << setw(15) << left << "Railway_ID" << setw(15) << left << "Code" << setw(15) << left << "Capacity" << endl;
						while ((row = mysql_fetch_row(res)))
						{
							cout << setw(15) << left << row[0] << setw(15) << left << row[1] << setw(15) << left << row[2] << endl;
						}
					}
					else
					{
						cout << "There are no railway transport inserted yet." << endl;
						cout << "Enter a key to back to control menu...";
						_getch();
						return controlMenu();
					}
				}

				for (int i = 0; i < 3; i++)
				{
					cout << endl;
				}

				do
				{
					cout << "Do you want to delete any railway transport? (y/n): ";
					cin >> delTransport;
					cin.ignore(80, '\n');

					if (delTransport == "Y" || delTransport == "y")
					{
						do
						{
							cout << endl;
							cout << "Enter the Railway ID to delete: ";
							cin >> transportID;

							// check validation
							int railwayIDCount = 0;
							// if found tiok, then can go delete
							for (int i = 0; i < railwayIDVec.size(); i++)
							{
								if (transportID == railwayIDVec[i])
								{
									break;
								}
								else
								{
									railwayIDCount++;
								}
							}

							if (railwayIDCount < railwayIDVec.size())
							{
								string delTransportID = "delete from railway_transport where railway_ID = '" + transportID + "'";
								const char* dti = delTransportID.c_str();
								qstate = mysql_query(conn, dti);
								res = mysql_store_result(conn);

								delTransport = "y";
							}
							else if (railwayIDCount == railwayIDVec.size())
							{
								cout << "Couldn't find the Railway ID..." << endl;
								cout << "Enter to insert another Railway ID...";
								_getch();
								delTransport = "n";
							}
						} while (delTransport == "N" || delTransport == "n");
					}
					else if (delTransport == "N" || delTransport == "n")
					{
						cout << "Enter any key to back to control menu..." << endl;
						_getch();
						return controlMenu();
					}
					else
					{
						cout << "Wrong input!" << endl;

					}
				} while (delTransport != "Y" && delTransport != "y" && delTransport != "N" && delTransport != "n");

				do
				{
					cout << "Do you want to delete another railway transport? (y/n) : ";
					cin >> delAgain;
					cin.ignore(80, '\n');

					if (delAgain == "N" && delAgain == "n")
					{
						return controlMenu();
					}
				} while (delAgain != "Y" && delAgain != "y" && delAgain != "N" && delAgain != "n");

			} while (delAgain == "Y" || delAgain == "y");
		}
		else if (controlChoice == "5")
		{
			string delAgain;
			do
			{
				string delDriver;
				string driverID;
				vector<string> driverIDVec;

				system("cls");

				// store the railway ID into vector for validation
				string selectDriverID = "select driver_ID from railway_driver";
				const char* sdi = selectDriverID.c_str();
				qstate = mysql_query(conn, sdi);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (res->row_count >= 1)
					{
						while (row = mysql_fetch_row(res))
						{
							driverIDVec.push_back(row[0]);
						}
					}
				}


				cout << "**********************************************" << endl;
				cout << "|                Railway Driver              |" << endl;
				cout << "**********************************************" << endl;

				string viewDriver = "select * from railway_driver";
				const char* vd = viewDriver.c_str();
				qstate = mysql_query(conn, vd);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (res->row_count >= 1)
					{
						cout << setw(15) << left << "Driver ID" << setw(15) << left << "Railway ID" << setw(30) << left << "Driver Name" << setw(20) << left << "Driver IC" << endl;
						while ((row = mysql_fetch_row(res)))
						{
							cout << setw(15) << left << row[0] << setw(15) << left << row[1] << setw(30) << left << row[2] << setw(20) << left << row[3] << endl;
						}
					}
					else
					{
						cout << "There are no railway driver inserted yet." << endl;
						cout << "Enter a key to back to control menu...";
						_getch();
						return controlMenu();
					}
				}

				for (int i = 0; i < 3; i++)
				{
					cout << endl;
				}

				do
				{
					cout << "Do you want to delete any railway driver? (y/n): ";
					cin >> delDriver;
					cin.ignore(80, '\n');

					if (delDriver == "Y" || delDriver == "y")
					{
						do
						{
							cout << endl;
							cout << "Enter the Driver ID to delete: ";
							cin >> driverID;

							// check validation
							int driverIDCount = 0;
							// if found tiok, then can go delete
							for (int i = 0; i < driverIDVec.size(); i++)
							{
								if (driverID == driverIDVec[i])
								{
									break;
								}
								else
								{
									driverIDCount++;
								}
							}

							if (driverIDCount < driverIDVec.size())
							{
								string delDriverID = "delete from railway_driver where driver_ID = '" + driverID + "'";
								const char* ddi = delDriverID.c_str();
								qstate = mysql_query(conn, ddi);
								res = mysql_store_result(conn);

								delDriver = 'y';
							}
							else if (driverIDCount == driverIDVec.size())
							{
								cout << "Couldn't find the Driver ID..." << endl;
								cout << "Enter to insert another Driver ID...";
								_getch();
								delDriver = "n";
							}
						} while (delDriver == "N" || delDriver == "n");
					}
					else if (delDriver == "N" || delDriver == "n")
					{
						cout << "Enter any key to back to control menu..." << endl;
						_getch();
						return controlMenu();
					}
					else
					{
						cout << "Wrong input!" << endl;

					}
				} while (delDriver != "Y" && delDriver != "y"  && delDriver != "N" && delDriver != "n");

				do
				{
					cout << "Do you want to delete another railway driver? (y/n) : ";
					cin >> delAgain;
					cin.ignore(80, '\n');

					if (delAgain == "N" || delAgain == "n")
					{
						return controlMenu();
					}
				} while (delAgain != "Y" && delAgain != "y" && delAgain != "N" && delAgain != "n");

			} while (delAgain == "Y" || delAgain == "y");
		}
		else if (controlChoice == "6")
		{
			system("cls");
			return adminMenu();
		}
		else
		{
			cout << "Wrong input! Try again!" << endl;
			_getch();
		}
	}while (controlChoice != "1" && controlChoice != "2" && controlChoice != "3" && controlChoice != "4" && controlChoice != "5" && controlChoice != "6");
}



void updateMenu()
{
	string updateChoice;

	do
	{
		char again;
		do
		{
			system("cls");


			cout << "**********************************************" << endl;
			cout << "|                Update Menu                 |" << endl;
			cout << "**********************************************" << endl;
			cout << "|        1. Insert trip                      |" << endl;
			cout << "|        2. Base price                       |" << endl;
			cout << "|        3. Privilage price                  |" << endl;
			cout << "|        4. Senior discount                  |" << endl;
			cout << "|        5. Insert railway transport         |" << endl;
			cout << "|        6. Insert railway driver            |" << endl;
			cout << "|        7. Exit                             |" << endl;
			cout << "**********************************************" << endl;

			cout << "\nEnter the update option: ";
			cin >> updateChoice;
			cin.ignore(80, '\n');

			if (updateChoice != "1" && updateChoice != "2" && updateChoice != "3" && updateChoice != "4" && updateChoice != "5" && updateChoice != "6" && updateChoice != "7" && updateChoice != "8")
			{
				cout << "Wrong input! Enter any key to try again..." << endl;
				_getch();
			}
		} while (updateChoice != "1" && updateChoice != "2" && updateChoice != "3" && updateChoice != "4" && updateChoice != "5" && updateChoice != "6" && updateChoice != "7" && updateChoice != "8");
		// set one station as the main station, then set other station distance from it
		// can locate direction, whether it is left or right from main station
		// insert station, railwayID and set the available seats, station to station, distance
		if (updateChoice == "1")
		{
			string insertYes = "n";
			do
			{
				string tripID = "";
				string railwayID = "";
				string departedStation = "";
				string destinatedStation = "";
				string tripDate = "";	// 2019-04-05
				string hour = "";
				string minute = "";
				string timeDepart = "";	// 00:00:00
				string timeArrive = "";
				int duration = 0;
				double basePrice = 0;
				double privilagePrice = 0;
				double seniorDiscount = 0;
				double price;

				//display all the stations
				system("cls");

				cout << "**********************************************" << endl;
				cout << "|                 Trip List                  |" << endl;
				cout << "**********************************************" << endl;

				// retrieve base price, privilage price and 
				// senior discount from price_update table
				string selectPriceList = "select Base_Price from Price_Update";
				const char* spl = selectPriceList.c_str();
				qstate = mysql_query(conn, spl);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (res->row_count == 1)
					{
						row = mysql_fetch_row(res);
						basePrice = stod(row[0]);
					}
					else
					{
						cout << "There are no base price inserted yet." << endl;
					}
				}

				string viewTripList = "select * from trip";
				const char* vtr = viewTripList.c_str();
				qstate = mysql_query(conn, vtr);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (res->row_count >= 1)
					{
						cout << setw(15) << left << "Trip ID" << setw(15) << left << "Railway ID" << setw(20)
							<< left << "Departed Station" << setw(20) << left << "Destinate Station" << setw(15) << left << "Trip Date" << setw(15) << left << "Time Depart"
							<< setw(15) << left << "Time Arrive" << setw(20) << left << "Duration (minutes)" << setw(15) << left << "Price (RM)" << endl;
						while ((row = mysql_fetch_row(res)))
						{
							cout << setw(15) << left << row[0] << setw(15) << left << row[1] << setw(20)
								<< left << row[2] << setw(20) << left << row[3] << setw(15) << left << row[4] << setw(15) << left << row[5]
								<< setw(15) << left << row[6] << setw(20) << left << row[7] << setw(15) << left << row[8] << endl;
						}
					}
					else
					{
						cout << "There are no trip inserted yet." << endl;
					}
				}

				for (int i = 0; i < 3; i++)
				{
					cout << endl;
				}

				if (insertYes == "N" || insertYes == "n")
				{
					do
					{
						cout << "Do you want to insert any trip? (y/n): ";
						cin >> insertYes;
						cin.ignore(80, '\n');

						if (insertYes == "N" || insertYes == "n")
						{
							cout << "Enter any key to back to update menu..." << endl;
							_getch();
							return updateMenu();
						}
						else if (insertYes != "Y" && insertYes != "y" && insertYes != "N" && insertYes != "n")
						{
							cout << "Wrong input! Try again!" << endl;
						}
					} while (insertYes != "Y" && insertYes != "y" && insertYes != "N" && insertYes != "n");
				}


				for (int i = 0; i < 3; i++)
				{
					cout << endl;
				}

				// ask to insert trip
				cout << "Enter the trip ID: ";
				cin >> tripID;
				cout << "Enter the railway ID: ";
				cin >> railwayID;
				cout << "Enter the departed station: ";
				cin >> departedStation;
				cout << "Enter the destinated station: ";
				cin >> destinatedStation;
				cout << "Enter the date (YYYY-MM-DD): ";
				cin >> tripDate;
				cout << "Enter the depart time (Hour): ";
				cin >> hour;
				cout << "Enter the depart time (Minute): ";
				cin >> minute;
				timeDepart = hour + ":" + minute + ":00";
				cout << "Enter the duration of the trip: ";
				cin >> duration;

				int hourArrive = 0;
				int minuteArrive = 0;
				stringstream geek(hour);
				geek >> hourArrive;
				stringstream gek(minute);
				gek >> minuteArrive;

				minuteArrive += duration;

				if (minuteArrive >= 60)
				{
					int tempHour = minuteArrive / 60;
					minuteArrive = minuteArrive % 60;
					hourArrive += tempHour;
				}

				hour = to_string(hourArrive);
				minute = to_string(minuteArrive);
				timeArrive = hour + ":" + minute + ":00";

				// price formula = base price * (duration/10);
				price = basePrice * (duration / 10);

				// insert into database
				string insertTripList = "insert into trip VALUES ('" + tripID + "', '" + railwayID + "', '" + departedStation + "', '" + destinatedStation + "', '" + tripDate + "', '" + timeDepart + "', '" + timeArrive + "', " + to_string(duration) + ", " + to_string(price) + ")";
				const char* itl = insertTripList.c_str();
				qstate = mysql_query(conn, itl);

				if (qstate)
				{
					cout << "Error with foreign key" << endl;
				}

				do
				{
					cout << "Do you want to insert another trip? (y/n) : ";
					cin >> again;
					again = tolower(again);
					if (again == 'n')
					{
						return updateMenu();
					}

				} while (again != 'y' && again != 'n');
			} while (again == 'y');
		}
		else if (updateChoice == "2")
		{
			string baseInsert;
			double basePrice;
			vector<string> tripID;
			vector<int> tripDuration;
			vector<double> tripPrice;

			// display the base price
			system("cls");

			cout << "**********************************************" << endl;
			cout << "|                 Base Price                 |" << endl;
			cout << "**********************************************" << endl;

			string viewBasePrice = "select Base_Price from Price_Update";
			const char* vbp = viewBasePrice.c_str();
			qstate = mysql_query(conn, vbp);

			if (!qstate)
			{
				res = mysql_store_result(conn);
				if (res->row_count == 1)
				{
					while ((row = mysql_fetch_row(res)))
					{
						cout << "\n\nThe base price is: RM" << row[0] << " per 10 minutes." << endl;
						do
						{
							cout << "\nDo you want to update base price? (y/n): ";
							cin >> baseInsert;
							cin.ignore(80, '\n');

							if (baseInsert == "Y" || baseInsert == "y")
							{
								cout << "Enter the base price per 10 minutes: RM";
								cin >> basePrice;
								cout << endl;
								cout << "The base price has been updated as RM" << basePrice << "." << endl;

								// insert into PRICE_UPDATE table
								string insertBasePrice = "update Price_Update set Base_Price = '" + to_string(basePrice) + "'" + " where Price_Update_ID = '" + to_string(1) + "'";
								const char* ibp = insertBasePrice.c_str();
								qstate = mysql_query(conn, ibp);

								if (qstate)
								{
									cout << "Error with inserting!" << endl;
								}

								// update the price of all the trip that Admin set
								// // get duration into vector
								// update the price = (duration / 10) * basePrice
								string retrieveTripDuration = "select trip_ID, duration from trip";
								const char* rtd = retrieveTripDuration.c_str();
								qstate = mysql_query(conn, rtd);

								if (!qstate)
								{
									res = mysql_store_result(conn);
									if (res->row_count >= 1)
									{
										while (row = mysql_fetch_row(res))
										{
											tripID.push_back(row[0]);
											int curDuration = 0;
											stringstream geek(row[1]);
											geek >> curDuration;

											tripDuration.push_back(curDuration);
										}
									}
								}

								// calculate the basePrice for each row
								for (int i = 0; i < tripDuration.size(); i++)
								{
									double curPrice = (tripDuration[i] / 10) * basePrice;
									tripPrice.push_back(curPrice);
								}

								for (int i = 0; i < tripID.size(); i++)
								{
									string updatePrice = "update Trip SET Price = '" + to_string(tripPrice[i]) + "' where trip_ID = '" + tripID[i] + "'";
									const char* up = updatePrice.c_str();
									qstate = mysql_query(conn, up);

									if (!qstate)
									{
										res = mysql_store_result(conn);
									}
								}

								cout << "\nPress any key to back to Update Menu..." << endl;
								_getch();
								return updateMenu();

							}
							else if (baseInsert == "N" || baseInsert == "n")
							{
								return updateMenu();
							}
							else
							{
								cout << "Wrong input! Try again!" << endl;

							}
						} while (baseInsert != "Y" && baseInsert != "y" && baseInsert != "N" && baseInsert != "n");
					}
				}
				else
				{
					cout << "\n\nThere are no base price inserted yet." << endl;
					do
					{
						cout << "\nDo you want to insert base price? (y/n): ";
						cin >> baseInsert;
						cin.ignore(80, '\n');

						if (baseInsert == "Y" || baseInsert == "y")
						{
							cout << "Enter the base price per 10 minutes: RM";
							cin >> basePrice;
							cout << endl;
							cout << "The base price has been updated as RM" << basePrice << "." << endl;

							// insert into PRICE_UPDATE table
							string insertBasePrice = "insert into Price_Update (Price_Update_ID, Base_Price) VALUES ('" + to_string(1) + "', '" + to_string(basePrice) + "')";
							const char* ibp = insertBasePrice.c_str();
							qstate = mysql_query(conn, ibp);

							if (qstate)
							{
								cout << "Error with inserting!" << endl;
							}
							cout << "\nPress any key to back to Update Menu..." << endl;
							_getch();
							return updateMenu();
						}
						else if (baseInsert == "N" || baseInsert == "n")
						{
							return updateMenu();
						}
						else
						{
							cout << "Wrong input! Try again!" << endl;

						}
					} while (baseInsert != "Y" && baseInsert != "y" && baseInsert != "N" && baseInsert != "n");
				}
			}
		}
		else if (updateChoice == "3")
		{
			string privilageInsert;
			double privilagePrice;

			// display the base price
			system("cls");

			cout << "**********************************************" << endl;
			cout << "|              Privilage Price                |" << endl;
			cout << "**********************************************" << endl;

			string viewPrivilagePrice = "select Privilage_Price from Price_Update";
			const char* vpp = viewPrivilagePrice.c_str();
			qstate = mysql_query(conn, vpp);

			if (!qstate)
			{
				res = mysql_store_result(conn);
				if (res->row_count == 1)
				{
					while ((row = mysql_fetch_row(res)))
					{
						cout << "\n\nThe privilage price is: RM" << row[0] << " per 10 minutes." << endl;
						do
						{
							cout << "\nDo you want to update privilage price? (y/n): ";
							cin >> privilageInsert;
							cin.ignore(80, '\n');

							if (privilageInsert == "Y" || privilageInsert == "y")
							{
								cout << "Enter the privilage price per 10 minutes: RM";
								cin >> privilagePrice;
								cout << endl;
								cout << "The privilage price has been updated as RM" << privilagePrice << "." << endl;

								// insert into PRICE_UPDATE table
								string insertPrivilagePrice = "update Price_Update set Privilage_Price = '" + to_string(privilagePrice) + "'" + " where Price_Update_ID = '" + to_string(1) + "'";
								const char* ipp = insertPrivilagePrice.c_str();
								qstate = mysql_query(conn, ipp);

								if (qstate)
								{
									cout << "Error with inserting!" << endl;
								}
								cout << "\nPress any key to back to Update Menu..." << endl;
								_getch();
								return updateMenu();
							}
							else if (privilageInsert == "N" || privilageInsert == "n")
							{
								return updateMenu();
							}
							else
							{
								cout << "Wrong input! Try again!" << endl;

							}
						} while (privilageInsert != "Y" && privilageInsert != "y" && privilageInsert != "N" && privilageInsert != "n");
					}
				}
				else
				{
					cout << "\n\nThere are no privilage price inserted yet." << endl;
					do
					{
						cout << "\nDo you want to insert privilage price? (y/n): ";
						cin >> privilageInsert;
						cin.ignore(80, '\n');

						if (privilageInsert == "Y" || privilageInsert == "y")
						{
							cout << "Enter the privilage price per 10 minutes: RM";
							cin >> privilagePrice;
							cout << endl;
							cout << "The privilage price has been updated as RM" << privilagePrice << "." << endl;

							// insert into PRICE_UPDATE table
							string insertPrivilagePrice = "insert into Price_Update (Price_Update_ID, Privilage_Price) VALUES ('" + to_string(1) + "', '" + to_string(privilagePrice) + "')";
							const char* ipp = insertPrivilagePrice.c_str();
							qstate = mysql_query(conn, ipp);

							if (qstate)
							{
								cout << "Error with inserting!" << endl;
							}
							cout << "\nPress any key to back to Update Menu..." << endl;
							_getch();
							return updateMenu();
						}
						else if (privilageInsert == "N" || privilageInsert == "n")
						{
							return updateMenu();
						}
						else
						{
							cout << "Wrong input! Try again!" << endl;

						}
					} while (privilageInsert != "Y" && privilageInsert != "y" && privilageInsert != "N" && privilageInsert != "n");
				}
			}
		}
		else if (updateChoice == "4")
		{
			string seniorInsert;
			double seniorDiscount;

			// display the base price
			system("cls");

			cout << "**********************************************" << endl;
			cout << "|                Senior Discount              |" << endl;
			cout << "**********************************************" << endl;

			string viewSeniorDiscount = "select Senior_Discount from Price_Update";
			const char* vsd = viewSeniorDiscount.c_str();
			qstate = mysql_query(conn, vsd);

			if (!qstate)
			{
				res = mysql_store_result(conn);
				if (res->row_count == 1)
				{
					while ((row = mysql_fetch_row(res)))
					{
						cout << "\n\nThe senior discount is: " << row[0] << "%" << endl;
						do
						{
							cout << "\nDo you want to update senior discount? (y/n): ";
							cin >> seniorInsert;
							cin.ignore(80, '\n');

							if (seniorInsert == "Y" || seniorInsert == "y")
							{
								cout << "Enter the senior discount per 10 minutes (%): ";
								cin >> seniorDiscount;
								cout << endl;
								cout << "The senior discount has been updated as " << seniorDiscount << "%." << endl;

								// insert into PRICE_UPDATE table
								string insertSeniorDiscount = "update Price_Update set Senior_Discount = '" + to_string(seniorDiscount) + "'" + " where Price_Update_ID = '" + to_string(1) + "'";
								const char* isd = insertSeniorDiscount.c_str();
								qstate = mysql_query(conn, isd);

								if (qstate)
								{
									cout << "Error with inserting!" << endl;
								}
								cout << "\nPress any key to back to Update Menu..." << endl;
								_getch();
								return updateMenu();
							}
							else if (seniorInsert == "N" || seniorInsert == "n")
							{
								return updateMenu();
							}
							else
							{
								cout << "Wrong input! Try again!" << endl;

							}
						} while (seniorInsert != "Y" && seniorInsert != "y" && seniorInsert != "N" && seniorInsert != "n");
					}
				}
				else
				{
					cout << "\n\nThere are no senior discount inserted yet." << endl;
					do
					{
						cout << "\nDo you want to insert senior discount? (y/n): ";
						cin >> seniorInsert;
						cin.ignore(80, '\n');

						if (seniorInsert == "Y" || seniorInsert == "y")
						{
							cout << "Enter the senior discount per 10 minutes (%): ";
							cin >> seniorDiscount;
							cout << endl;
							cout << "The senior discount has been updated as " << seniorDiscount << "%." << endl;

							// insert into PRICE_UPDATE table
							string insertSeniorDiscount = "insert into Price_Update (Price_Update_ID, Senior_Discount) VALUES ('" + to_string(1) + "', '" + to_string(seniorDiscount) + "')";
							const char* isd = insertSeniorDiscount.c_str();
							qstate = mysql_query(conn, isd);

							if (qstate)
							{
								cout << "Error with inserting!" << endl;
							}
							cout << "\nPress any key to back to Update Menu..." << endl;
							_getch();
							return updateMenu();
						}
						else if (seniorInsert == "N" || seniorInsert == "n")
						{
							return updateMenu();
						}
						else
						{
							cout << "Wrong input! Try again!" << endl;

						}
					} while (seniorInsert != "Y" && seniorInsert != "y" && seniorInsert != "N" && seniorInsert != "n");
				}
			}
		}
		else if (updateChoice == "5")
		{
			string again;
			do
			{
				string transportID;
				char transportCode;
				int transportCapacity;
				string insertTransport;

				// display the railway transport
				system("cls");

				cout << "**********************************************" << endl;
				cout << "|              Railway Transport             |" << endl;
				cout << "**********************************************" << endl;

				string viewTransport = "select * from railway_transport";
				const char* vt = viewTransport.c_str();
				qstate = mysql_query(conn, vt);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (res->row_count >= 1)
					{
						cout << setw(15) << left << "Railway_ID" << setw(10) << left << "Code" << setw(15) << left << "Capacity" << endl;
						while ((row = mysql_fetch_row(res)))
						{
							cout << setw(15) << left << row[0] << setw(10) << left << row[1] << setw(15) << left << row[2] << endl;
						}
					}
					else
					{
						cout << "There are no railway transport inserted yet." << endl;
					}
				}

				for (int i = 0; i < 3; i++)
				{
					cout << endl;
				}

				do
				{
					cout << "Do you want to insert any railway transport? (Y/N): ";
					cin >> insertTransport;
					cin.ignore(80, '\n');

					if (insertTransport == "Y" || insertTransport == "y")
					{
						cout << "Enter the Railway ID: ";
						cin >> transportID;
						cout << "Enter the Code: ";
						cin >> transportCode;
						cout << "Enter the capacity: ";
						cin >> transportCapacity;

						// insert into database
						string insertRailwayTransport = "insert into railway_transport VALUES ('" + transportID + "', '" + transportCode + "', '" + to_string(transportCapacity) + "')";
						const char* irt = insertRailwayTransport.c_str();
						qstate = mysql_query(conn, irt);

						if (qstate)
						{
							cout << "Error with foreign key" << endl;
						}
					}
					else if (insertTransport == "N" || insertTransport == "n")
					{
						cout << "Enter any key to return to update menu..." << endl;
						_getch();
						return updateMenu();
					}
					else
					{
						cout << "Wrong input! Try again!" << endl;
					}
				} while (insertTransport != "Y" && insertTransport != "y" && insertTransport != "N" && insertTransport != "n");

				do
				{
					cout << "Do you want to insert another railway transport? (y/n) : ";
					cin >> again;
					cin.ignore(80, '\n');

					if (again == "N" || again == "n")
					{
						return updateMenu();
					}

				} while (again != "Y" && again != "y" && again != "N" && again != "n");
			} while (again == "Y" || again == "y");
		}
		else if (updateChoice == "6")
		{
			string again;
			do
			{
				string driverID;
				string transportID;
				string driverName;
				string driverIC;
				// create a vector to store the railway ID for validation
				vector<string> transportVec;
				int validateSize;
				string insertDriver;

				// display the base price
				system("cls");

				// store the railway ID into the vector
				string retrieveDriver = "select Railway_ID from railway_transport";
				const char* rd = retrieveDriver.c_str();
				qstate = mysql_query(conn, rd);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (res->row_count >= 1)
					{
						while (row = mysql_fetch_row(res))
						{
							transportVec.push_back(row[0]);
						}
					}
					else
					{
						cout << "No railway transport inserted yet!" << endl;
						_getch();
						return updateMenu();
					}
				}
				else
				{
					cout << "Error with railway table!" << endl;
					_getch();
					return updateMenu();
				}

				cout << "**********************************************" << endl;
				cout << "|                Railway Driver              |" << endl;
				cout << "**********************************************" << endl;

				string viewDriver = "select * from railway_driver";
				const char* vd = viewDriver.c_str();
				qstate = mysql_query(conn, vd);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (res->row_count >= 1)
					{
						cout << setw(15) << left << "Driver ID" << setw(15) << left << "Railway ID" << setw(30) << left << "Driver Name" << setw(20) << left << "Driver IC" << endl;
						while ((row = mysql_fetch_row(res)))
						{
							cout << setw(15) << left << row[0] << setw(15) << left << row[1] << setw(30) << left << row[2] << setw(20) << left << row[3] << endl;
						}
					}
					else
					{
						cout << "There are no railway driver inserted yet." << endl;
					}
				}

				for (int i = 0; i < 3; i++)
				{
					cout << endl;
				}

				do
				{
					cout << "Do you want to insert any driver? (Y/N): ";
					cin >> insertDriver;
					cin.ignore(80, '\n');

					if (insertDriver != "Y" && insertDriver != "y" && insertDriver != "N" && insertDriver != "n")
					{
						cout << "Wrong input! Try again!" << endl;
					}
				} while (insertDriver != "Y" && insertDriver != "y" && insertDriver != "N" && insertDriver != "n");

				if (insertDriver == "Y" || insertDriver == "y")
				{
					cout << "Enter the Driver ID: ";
					cin >> driverID;
					// validate railway ID
					do
					{
						validateSize = 0;
						cout << "Enter the Railway ID: ";
						cin >> transportID;
						for (int i = 0; i < transportVec.size(); i++)
						{
							if (transportID == transportVec[i])
							{
								break;
							}
							else
							{
								validateSize++;
							}
						}

						if (validateSize == transportVec.size())
						{
							cout << "No match Railway ID! Try again!" << endl;
						}
					} while (validateSize == transportVec.size());

					cout << "Enter the Driver name: ";
					cin >> driverName;
					cout << "Enter the Driver IC: ";
					cin >> driverIC;

					// insert into database
					string insertRailwayDriver = "insert into railway_driver VALUES ('" + driverID + "', '" + transportID + "', '" + driverName + "', '" + driverIC + "')";
					const char* ird = insertRailwayDriver.c_str();
					qstate = mysql_query(conn, ird);

					if (qstate)
					{
						cout << "Error with foreign key" << endl;
					}
				}
				else if (insertDriver == "N" || insertDriver == "n")
				{
					cout << "Enter any key to back to update menu..." << endl;
					_getch();
					return updateMenu();
				}

				do
				{
					cout << "Do you want to insert another railway driver? (y/n) : ";
					cin >> again;
					cin.ignore(80, '\n');

					if (again == "N" || again == "n")
					{
						return updateMenu();
					}

				} while (again != "Y" && again != "y" && again != "N" && again != "n");
			} while (again == "Y" || again == "y");
		}
		else if (updateChoice == "7")
		{
			return controlMenu();
		}
		else
		{
			cout << "Wrong input! Try again!" << endl;
			_getch();
		}
	}while (updateChoice != "1" && updateChoice != "2" && updateChoice != "3" && updateChoice != "4" && updateChoice != "5" && updateChoice != "6" && updateChoice != "7" && updateChoice != "8");
 }


 void customerValid()
 {
	 string local;
	 char localDatabase;
	 string localIC;
	 string foreignIC;
	 int age;
	 int ageDifferentiator;	// to determine 2000s or before 2000s
	 string customerCh;
	 

	 do
	 {
		 cout << "Are you local resident? (y/n): ";
		 cin >> local;
		 cin.ignore(80, '\n');

		 if (local != "Y" && local != "y" && local != "N" && local != "n")
		 {
			 cout << "Wrong input! Try again!" << endl;
		 }
	 } while (local != "Y" && local != "y" && local != "N" && local != "n");

	 do
	 {
		 // if local, take IC, use the front 2 digit to get age
		 if (local == "Y" || local == "y")
		 {
			 localDatabase = '1';
			 cout << "Enter your IC number (000000070000): ";
			 cin >> localIC;

			 string curString;
			 curString = localIC.substr(0, 2);

			 stringstream geek(curString);
			 geek >> age;

			 // take current year - age = age
			 time_t now = time(0);
			 tm* ltm = localtime(&now);
			 int curYear = 1900 + ltm->tm_year;	// 2021
			 ageDifferentiator = curYear % 100;	// 21

			 if (age > ageDifferentiator)	// born before 200s
			 {
				 age = curYear - (1900 + age);
			 }
			 else	// born after and on 2000s
			 {
				 age = ageDifferentiator - age;
			 }
		 }
		 else if (local == "N" || local == "n")
		 {
			 localDatabase = '0';
			 cout << "Enter your IC number (format without "<< "'-'" << " ): ";
			 cin >> foreignIC;

			 cout << "Enter your age: ";
			 cin >> age;
		 }
		 else
		 {
			 cout << "Wrong input! Try again!" << endl;
		 }
	 } while (local != "Y" && local != "y" && local != "N" && local != "n");


	 string loop = "y";
	 do
	 {
		 customerCh = customerMenu();
		 if (customerCh == "1")
		 {
			 if (local == "Y" || local == "y")
			 {
				 viewTrip(local, age);
			 }
			 else if (local == "N" || local == "n")
			 {
				 viewTrip(local, age);
			 }
		 }
		 else if (customerCh == "2")
		 {
			 if (local == "Y" || local == "y")
			 {
				 purchaseTicket(localIC, age, local);
			 }
			 else if (local == "N" || local == "n")
			 {
				 purchaseTicket(foreignIC, age, local);
			 }
		 }
		 else if (customerCh == "3")
		 {
			 if (local == "Y" || local == "y")
			 {
				 delPurchaseTicket(localIC);
			 }
			 else if (local == "N" || local == "n")
			 {
				 delPurchaseTicket(foreignIC);
			 }
		 }
		 else if (customerCh == "4")
		 {
			 main();
		 }

	 } while (loop == "y");

 }


 string customerMenu()
 {
	 string customerChoice;

	 system("cls");

	 cout << "**********************************************" << endl;
	 cout << "|                Customer Menu               |" << endl;
	 cout << "**********************************************" << endl;
	 cout << "|        1. View Available Trip              |" << endl;
	 cout << "|        2. Purchase Ticket                  |" << endl;
	 cout << "|        3. View and Delete Ticket           |" << endl;
	 cout << "|        4. Exit                             |" << endl;
	 cout << "**********************************************" << endl;

	 cout << "Enter the customer option: ";
	 cin >> customerChoice;

	 return customerChoice;
 }


 void viewTrip(string local, int age)
 {
	 double basePrice;
	 double privilagePrice;
	 char privilageYes = 'n';
	 double seniorDiscount;
	 char seniorYes = 'n';
	 vector<string> ticketVec;
	 int ticketNum;
	 vector<string> tripVec;
	 vector<int> durationVec;
	 vector<double> priceVec;
	 int priceCounter = 0;
	 int totalTrip = 0;
	 int priceCount = 0;
	 int tripRotate = 0;
	 char nextPage;
	 // take current time take and compare the trip_date to the current time, if greater, only shows
	 int year, month, day;
	 string curDate, curMonth, curDay;
	 string again;


	 // retrieve base price, privilage price and senior price
	 string retrievePrice = "select base_price, privilage_price, senior_discount from price_update";
	 const char* rp = retrievePrice.c_str();
	 qstate = mysql_query(conn, rp);
	 if (!qstate)
	 {
		 res = mysql_store_result(conn);
		 if (res->row_count == 1)
		 {
			 while (row = mysql_fetch_row(res))
			 {
				 basePrice = stod(row[0]);
				 privilagePrice = stod(row[1]);
				 seniorDiscount = stod(row[2]);
			 }
		 }
	 }


	 // get duration vector so that can show different price
	 string retrieveIDDuration = "select trip_ID, duration from trip";
	 const char* rdd = retrieveIDDuration.c_str();
	 qstate = mysql_query(conn, rdd);

	 if (!qstate)
	 {
		 res = mysql_store_result(conn);
		 if (res->row_count >= 1)
		 {
			 while ((row = mysql_fetch_row(res)))
			 {
				 tripVec.push_back(row[0]);
				 int curDuration = 0;
				 stringstream geek(row[1]);
				 geek >> curDuration;
				 durationVec.push_back(curDuration);
			 }
		 }
	 }

	 // the ticketID is refer to ticket table, retrieve the latest ticket ID then add 1.
	 string retrieveTicketID = "select ticket_ID from ticket ORDER BY Ticket_ID";
	 const char* rti = retrieveTicketID.c_str();
	 qstate = mysql_query(conn, rti);

	 if (!qstate)
	 {
		 res = mysql_store_result(conn);
		 if (res->row_count >= 1)
		 {
			 while ((row = mysql_fetch_row(res)))
			 {
				 ticketVec.push_back(row[0]);
			 }
		 }
	 }


	 ticketNum = ticketVec.size();

	 // show trip, can press 'y' to see next, or 'n' quit
	// show different interface for normal, privilage and senior customer
	// show privilage or normal first (use ticketNum to determine), then only see age whether is senior or not
	// but the privilage also depends on the number of ticket purchase on that day
	 if (local == "Y" || local == "y")
	 {
		 if (ticketNum < 5)	// privilage price
		 {
			 privilageYes = 'y';
			 if (age >= 60)	// senior
			 {
				 seniorYes = 'y';
				 for (int i = 0; i < durationVec.size(); i++)
				 {
					 double curPrice = (durationVec[i] / 10) * privilagePrice * ((100 - seniorDiscount) / 100);
					 priceVec.push_back(curPrice);
				 }
			 }
			 else
			 {
				 for (int i = 0; i < durationVec.size(); i++)
				 {
					 double curPrice = (durationVec[i] / 10) * privilagePrice;
					 priceVec.push_back(curPrice);
				 }
			 }
		 }
		 else	// normal price
		 {
			 if (age >= 60)	// senior
			 {
				 seniorYes = 'y';
				 for (int i = 0; i < durationVec.size(); i++)
				 {
					 double curPrice = (durationVec[i] / 10) * basePrice * ((100 - seniorDiscount) / 100);
					 priceVec.push_back(curPrice);
				 }
			 }
			 else
			 {
				 for (int i = 0; i < durationVec.size(); i++)
				 {
					 double curPrice = (durationVec[i] / 10) * basePrice;
					 priceVec.push_back(curPrice);
				 }
			 }
		 }
	 }
	 else if (local == "N" || local == "n")
	 {
		 for (int i = 0; i < durationVec.size(); i++)
		 {
			 double curPrice = (durationVec[i] / 10) * basePrice;
			 priceVec.push_back(curPrice);
		 }
	 }

	 totalTrip = tripVec.size();

	 system("cls");

	 cout << "**********************************************" << endl;
	 cout << "|                 Trip List                  |" << endl;
	 cout << "**********************************************" << endl;

	 string viewTripList = "select * from trip";
	 const char* vtr = viewTripList.c_str();
	 qstate = mysql_query(conn, vtr);

	 if (!qstate)
	 {
		 res = mysql_store_result(conn);
		 if (res->row_count >= 1)
		 {
			 cout << setw(15) << left << "Trip ID" << setw(15) << left << "Railway ID" << setw(20)
				 << left << "Departed Station" << setw(20) << left << "Destinate Station" << setw(15) << left << "Trip Date" << setw(15) << left << "Time Depart"
				 << setw(15) << left << "Time Arrive" << setw(20) << left << "Duration (minutes)" << setw(15) << left << "Price (RM)" << endl;
			 while ((row = mysql_fetch_row(res)))
			 {
				 cout << setw(15) << left << row[0] << setw(15) << left << row[1] << setw(20)
					 << left << row[2] << setw(20) << left << row[3] << setw(15) << left << row[4] << setw(15) << left << row[5]
					 << setw(15) << left << row[6] << setw(20) << left << row[7] << setw(15) << left << priceVec[priceCount] << endl;
				 priceCount++;
				 tripRotate++;

				 // if show 10 trip, stop and ask to press 'y' for next, or 'n' to exit
				 if (tripRotate % 10 == 0)
				 {
					 cout << "Enter Y for next page or N to back to admin customer menu? (Y/N): ";
					 cin >> nextPage;
					 nextPage = tolower(nextPage);

					 if (nextPage == 'y')
					 {
						 system("cls");

						 cout << "**********************************************" << endl;
						 cout << "|                 Trip List                  |" << endl;
						 cout << "**********************************************" << endl;
						 cout << setw(15) << left << "Trip ID" << setw(15) << left << "Railway ID" << setw(20)
							 << left << "Departed Station" << setw(20) << left << "Destinate Station" << setw(15) << left << "Trip Date" << setw(15) << left << "Time Depart"
							 << setw(15) << left << "Time Arrive" << setw(20) << left << "Duration (minutes)" << setw(15) << left << "Price (RM)" << endl;
					 }
					 else if (nextPage == 'n')
					 {
						 break;
					 }

				 }
				 else if (tripRotate == totalTrip)
				 {
					 cout << "All trip has been shown." << endl;
					 cout << "Enter Y to view again or N to exit (Y/N): ";
					 cin >> nextPage;

					 if (nextPage == 'y')
					 {
						 return viewTrip(local, age);
					 }
					 else if (nextPage == 'n')
					 {
						 break;
					 }
				 }
			 }
		 }
		 else
		 {
			 cout << "There are no trip inserted yet." << endl;
		 }

		 for (int i = 0; i < 3; i++)
		 {
			 cout << endl;
		 }
	 }

 }



 // only shows future trips
 void displayTripForPurchase(vector<double> priceVec, char nextPage, int totalTrip, int priceCounter = 0, int tripRotate = 0)
 {
	 // take current time take and compare the trip_date to the current time, if greater, only shows
	 int year, month, day;
	 string curMonth, curDay;

	 system("cls");

	 cout << "**********************************************" << endl;
	 cout << "|                 Trip List                  |" << endl;
	 cout << "**********************************************" << endl;

	 string viewTripList = "select * from trip";
	 const char* vtr = viewTripList.c_str();
	 qstate = mysql_query(conn, vtr);

	 if (!qstate)
	 {
		 res = mysql_store_result(conn);
		 if (res->row_count >= 1)
		 {
			 cout << setw(15) << left << "Trip ID" << setw(15) << left << "Railway ID" << setw(20)
				 << left << "Departed Station" << setw(20) << left << "Destinate Station" << setw(15) << left << "Trip Date" << setw(15) << left << "Time Depart"
				 << setw(15) << left << "Time Arrive" << setw(20) << left << "Duration (minutes)" << setw(15) << left << "Price (RM)" << endl;
			 while ((row = mysql_fetch_row(res)))
			 {
				 cout << setw(15) << left << row[0] << setw(15) << left << row[1] << setw(20)
					 << left << row[2] << setw(20) << left << row[3] << setw(15) << left << row[4] << setw(15) << left << row[5]
					 << setw(15) << left << row[6] << setw(20) << left << row[7] << setw(15) << left << priceVec[priceCounter] << endl;
				 priceCounter++;
				 tripRotate++;
				 // if show 10 trip, stop and ask to press 'y' for next, or 'n' to exit
				 if (tripRotate % 10 == 0)
				 {
					 cout << "Enter Y for next page or N to purchase ticket (Y/N): ";
					 cin >> nextPage;
					 nextPage = tolower(nextPage);

					 if (nextPage == 'y')
					 {
						 system("cls");

						 cout << "**********************************************" << endl;
						 cout << "|                 Trip List                  |" << endl;
						 cout << "**********************************************" << endl;
						 cout << setw(15) << left << "Trip ID" << setw(15) << left << "Railway ID" << setw(20)
							 << left << "Departed Station" << setw(20) << left << "Destinate Station" << setw(15) << left << "Trip Date" << setw(15) << left << "Time Depart"
							 << setw(15) << left << "Time Arrive" << setw(20) << left << "Duration (minutes)" << setw(15) << left << "Price (RM)" << endl;
					 }
					 else if (nextPage == 'n')
					 {
						 break;
					 }

				 }
				 else if (tripRotate == totalTrip)
				 {
					 cout << "All trip has been shown." << endl;
					 cout << "Enter Y to view again or N to purchase ticket (Y/N): ";
					 cin >> nextPage;

					 if (nextPage == 'y')
					 {
						 return displayTripForPurchase(priceVec, nextPage, totalTrip, priceCounter = 0, tripRotate = 0);
					 }
					 else if (nextPage == 'n')
					 {
						 break;
					 }
				 }
			 }
		 }
		 else
		 {
			 cout << "There are no trip inserted yet." << endl;
		 }

		 for (int i = 0; i < 3; i++)
		 {
			 cout << endl;
		 }
	 }
 }

 void purchaseTicket(string IC, int age, string local)
 {
	 string purchaseAgain;
	 do
	 {
		 system("cls");

		 vector<string> ticketVec;	
		 vector<string> tripVec;	
		 vector<double> durationVec;
		 vector<double> priceVec;
		 vector<string> seatNumberVec;
		 int ticketNum;
		 string adminID;
		 double basePrice;
		 double privilagePrice;
		 char privilageYes = 'n';
		 double seniorDiscount;
		 char seniorYes = 'n';
		 int priceCounter = 0;
		 int totalTrip = 0;
		 int tripRotate = 0;
		 char nextPage = 'n';
		 string purchaseTripID;
		 string tripDate;
		 string purchaseRailwayID;
		 string railwayCode;
		 int railwayCapacity;
		 int railwayRow;
		 int railwayRowTotal;
		 int seatNum;
		 string purchaseSeat;
		 string purchaseTicketID;
		 string purchaseDate;
		 int priceDuration;
		 string purchaseDepartStation;
		 string purchaseDestinatedStation;
		 string purchaseTimeDepart;
		 string purchaseTimeArrive;
		 double finalPrice;
		 char tripExist = 'n';

		 // for the adminID of the customer table, create a new column for administrator table called char "Today", if "Today" == 'y', adminID in customer table will become him
		 string retrieveAdminID = "select Admin_ID from administrator where Admin_In_Charge = 'y'";
		 const char* iai = retrieveAdminID.c_str();
		 qstate = mysql_query(conn, iai);
		 if (!qstate)
		 {
			 res = mysql_store_result(conn);
			 if (res->row_count == 1)
			 {
				 while (row = mysql_fetch_row(res))
				 {
					 adminID = row[0];
				 }
			 }
		 }

		 // retrieve base price, privilage price and senior price
		 string retrievePrice = "select base_price, privilage_price, senior_discount from price_update";
		 const char* rp = retrievePrice.c_str();
		 qstate = mysql_query(conn, rp);
		 if (!qstate)
		 {
			 res = mysql_store_result(conn);
			 if (res->row_count == 1)
			 {
				 while (row = mysql_fetch_row(res))
				 {
					 basePrice = stod(row[0]);
					 privilagePrice = stod(row[1]);
					 seniorDiscount = stod(row[2]);
				 }
			 }
		 }


		 // insert IC, adminID, age and local into customer table 
		 string insertCustomer = "insert into customer VALUES('" + IC + "', '" + adminID + "', '" + to_string(age) + "', '" + local + "')";
		 const char* ic = insertCustomer.c_str();
		 qstate = mysql_query(conn, ic);


		 // the ticketID is refer to ticket table, retrieve the latest ticket ID then add 1.
		 string retrieveTicketID = "select ticket_ID from ticket ORDER BY Ticket_ID";
		 const char* rti = retrieveTicketID.c_str();
		 qstate = mysql_query(conn, rti);

		 if (!qstate)
		 {
			 res = mysql_store_result(conn);
			 if (res->row_count >= 1)
			 {
				 while ((row = mysql_fetch_row(res)))
				 {
					 ticketVec.push_back(row[0]);
				 }
			 }
		 }

		 ticketNum = ticketVec.size();

		 // get duration vector so that can show different price
		 string retrieveIDDuration = "select trip_ID, duration from trip";
		 const char* rdd = retrieveIDDuration.c_str();
		 qstate = mysql_query(conn, rdd);

		 if (!qstate)
		 {
			 res = mysql_store_result(conn);
			 if (res->row_count >= 1)
			 {
				 while ((row = mysql_fetch_row(res)))
				 {
					 tripVec.push_back(row[0]);
					 int curDuration = 0;
					 stringstream geek(row[1]);
					 geek >> curDuration;
					 durationVec.push_back(curDuration);
				 }
			 }
		 }



		 // show trip, same as view trip, customer can press 'y' to see next, or 'n' to buy the ticket 
		 // show different interface for normal, privilage and senior customer
		 // show privilage or normal first (use ticketNum to determine), then only see age whether is senior or not
		 // but the privilage also depends on the number of ticket purchase on that day
		 if (local == "Y" || local == "y")
		 {
			 if (ticketNum < 5)	// privilage price
			 {
				 privilageYes = 'y';
				 if (age >= 60)	// senior
				 {
					 seniorYes = 'y';
					 for (int i = 0; i < durationVec.size(); i++)
					 {
						 double curPrice = (durationVec[i] / 10) * privilagePrice * ((100 - seniorDiscount)/100);
						 priceVec.push_back(curPrice);
					 }
				 }
				 else
				 {
					 for (int i = 0; i < durationVec.size(); i++)
					 {
						 double curPrice = (durationVec[i] / 10) * privilagePrice;
						 priceVec.push_back(curPrice);
					 }
				 }
			 }
			 else	// normal price
			 {
				 if (age >= 60)	// senior
				 {
					 seniorYes = 'y';
					 for (int i = 0; i < durationVec.size(); i++)
					 {
						 double curPrice = (durationVec[i] / 10) * basePrice * ((100 - seniorDiscount)/100);
						 priceVec.push_back(curPrice);
					 }
				 }
				 else
				 {
					 for (int i = 0; i < durationVec.size(); i++)
					 {
						 double curPrice = (durationVec[i] / 10) * basePrice;
						 priceVec.push_back(curPrice);
					 }
				 }
			 }
		 }
		 else if (local == "N" || local == "n")
		 {
			 for (int i = 0; i < durationVec.size(); i++)
			 {
				 double curPrice = (durationVec[i] / 10) * basePrice;
				 priceVec.push_back(curPrice);
			 }
		 }

		 totalTrip = tripVec.size();

		 displayTripForPurchase(priceVec, nextPage, totalTrip, priceCounter = 0, tripRotate = 0);


		 do
		 {
			 cout << "Enter the Trip ID to purchase: ";
			 cin >> purchaseTripID;

			 for (int i = 0; i < tripVec.size(); i++)
			 {
				 if (purchaseTripID == tripVec[i])
				 {
					 tripExist = 'y';
					 break;
				 }
			 }

			 if (tripExist == 'n')
			 {
				 cout << "No such trip ID! Try again!" << endl;
			 }
		 } while (tripExist == 'n');

		 // calculate final price
		 // get the duration from the trip ID
		 string retrieveTripDuration = "select duration, trip_date from trip where trip_ID = '" + purchaseTripID + "'";
		 const char* rtd = retrieveTripDuration.c_str();
		 qstate = mysql_query(conn, rtd);

		 if (!qstate)
		 {
			 res = mysql_store_result(conn);
			 if (res->row_count == 1)
			 {
				 while (row = mysql_fetch_row(res))
				 {
					 stringstream geek(row[0]);
					 geek >> priceDuration;
					 tripDate = row[1];
				 }
			 }
		 }

		 if (privilageYes == 'y')
		 {
			 if (seniorYes == 'y')
			 {
				 finalPrice = privilagePrice * (priceDuration / 10) * ((100 - seniorDiscount)/100);
			 }
			 else
			 {
				 finalPrice = privilagePrice * (priceDuration / 10);
			 }
		 }
		 else
		 {
			 if (seniorYes == 'y')
			 {
				 finalPrice = basePrice * (priceDuration / 10) * (100 - seniorDiscount)/100;
			 }
			 else
			 {
				 finalPrice = basePrice * (priceDuration / 10);
			 }
		 }

		 // get seat number vector for display
		 string retrieveSeat = "select seat_number from ticket where trip_ID = '" + purchaseTripID + "'";
		 const char* rs = retrieveSeat.c_str();
		 qstate = mysql_query(conn, rs);

		 if (!qstate)
		 {
			 res = mysql_store_result(conn);
			 if (res->row_count >= 1)
			 {
				 while ((row = mysql_fetch_row(res)))
				 {
					 seatNumberVec.push_back(row[0]);
				 }
			 }
		 }


		 // after choosing the trip ID, show customer the train model (use the Trip ID to retrieve Railway ID from Trip table and use 
		 // the Railway_ID to retrieve the Code and Capacity from Railway_Transport table). 
		 // When after display all, compare to the seat number vector, if there same, change the seat number to x in display
		 // Compare the user chose Seat number with the seat number vector, if same, say already got people choose
		 string retrieveRailwayID = "select railway_ID, departed_station, destinated_station, time_depart, time_arrive from trip where trip_ID = '" + purchaseTripID + "'";
		 const char* rri = retrieveRailwayID.c_str();
		 qstate = mysql_query(conn, rri);

		 if (!qstate)
		 {
			 res = mysql_store_result(conn);
			 if (res->row_count == 1)
			 {
				 while ((row = mysql_fetch_row(res)))
				 {
					 purchaseRailwayID = row[0];
					 purchaseDepartStation = row[1];
					 purchaseDestinatedStation = row[2];
					 purchaseTimeDepart = row[3];
					 purchaseTimeArrive = row[4];
				 }
			 }
		 }

		 string retrieveCodeAndCap = "select code, capacity from railway_transport where railway_ID = '" + purchaseRailwayID + "'";
		 const char* rcc = retrieveCodeAndCap.c_str();
		 qstate = mysql_query(conn, rcc);

		 if (!qstate)
		 {
			 res = mysql_store_result(conn);
			 if (res->row_count == 1)
			 {
				 while ((row = mysql_fetch_row(res)))
				 {
					 railwayCode = row[0];
					 stringstream geek(row[1]);
					 geek >> railwayCapacity;
				 }
			 }
		 }
		 
		 // ----- ----- 
		 //| A1  | A5  |
		 //| A2  |     | 
		 //|     
		 //| A3  |     |
		 //| A4  |     |
		 // ----- -----
		 // row = (railwayCapacity / 4) + 1	= (50 / 4) + 1 = 13
		 // first and final row total length = (row + 1) + (5 * row) = (13 + 1) + (5 * 13) = 79
		 // first and final row formula = row * (" " + 5*"-") + " " = 
		 // middle part formula  = 
		 system("cls");
		 railwayRow = (railwayCapacity / 4) + 1;
		 railwayRowTotal = (railwayRow + 1) + (5 * railwayRow);

		 cout << "----------------" << endl;
		 cout << "      " + purchaseRailwayID + "      " << endl;
		 cout << "----------------" << endl;
		 cout << endl;

		 // first row
		 for (int i = 0; i < railwayRowTotal; i++)
		 {
			 if (i % 6 == 0)
			 {
				 cout << " ";
			 }
			 else
			 {
				 cout << "-";
			 }
		 }
		 cout << endl;

		 // second row
		 seatNum = 1;
		 for (int i = 0; i < railwayRow; i++)
		 {
			 string seat = railwayCode + to_string(seatNum);
			 if (seatNum <= railwayCapacity)
			 {
				 for (int j = 0; j < seatNumberVec.size(); j++)
				 {
					 if (seat == seatNumberVec[j])
					 {
						 seat = "x";
					 }
				 }
			 }
			 else
			 {
				 seat = "   ";
			 }
			 cout << "| " << setw(3) << seat << " ";
			 seatNum = seatNum + 4;
		 }
		 cout << "|" << endl;

		 // third row
		 seatNum = 2;
		 for (int i = 0; i < railwayRow; i++)
		 {
			 string seat = railwayCode + to_string(seatNum);
			 if (seatNum <= railwayCapacity)
			 {
				 for (int j = 0; j < seatNumberVec.size(); j++)
				 {
					 if (seat == seatNumberVec[j])
					 {
						 seat = "x";
					 }
				 }
			 }
			 else
			 {
				 seat = "   ";
			 }
			 cout << "| " << setw(3) << seat << " ";
			 seatNum = seatNum + 4;
		 }
		 cout << "|" << endl;

		 // fourth row
		 cout << "|";
		 for (int i = 0; i < railwayRowTotal - 2; i++)
		 {
			 cout << " ";
		 }
		 cout << "|" << endl;


		 // fifth row
		 seatNum = 3;
		 for (int i = 0; i < railwayRow; i++)
		 {
			 string seat = railwayCode + to_string(seatNum);
			 if (seatNum <= railwayCapacity)
			 {
				 for (int j = 0; j < seatNumberVec.size(); j++)
				 {
					 if (seat == seatNumberVec[j])
					 {
						 seat = "x";
					 }
				 }
			 }
			 else
			 {
				 seat = "   ";
			 }
			 cout << "| " << setw(3) << seat << " ";
			 seatNum = seatNum + 4;
		 }
		 cout << "|" << endl;

		 // sixth row
		 seatNum = 4;
		 for (int i = 0; i < railwayRow; i++)
		 {
			 string seat = railwayCode + to_string(seatNum);
			 if (seatNum <= railwayCapacity)
			 {
				 for (int j = 0; j < seatNumberVec.size(); j++)
				 {
					 if (seat == seatNumberVec[j])
					 {
						 seat = "x";
					 }
				 }
			 }
			 else
			 {
				 seat = "   ";
			 }
			 cout << "| " << setw(3) << seat << " ";
			 seatNum = seatNum + 4;
		 }
		 cout << "|" << endl;

		 // final row
		 for (int i = 0; i < railwayRowTotal; i++)
		 {
			 if (i % 6 == 0)
			 {
				 cout << " ";
			 }
			 else
			 {
				 cout << "-";
			 }
		 }

		 // when they purchase ticket,  record the purchase real date, record ticketID (ticketID use the ticketVec last one and add 1), so that can be used during the delete ticket
		 for (int i = 0; i < 3; i++)
		 {
			 cout << endl;
		 }

		 do
		 {
			 cout << "Enter the seat number: ";
			 cin >> purchaseSeat;

			 for (int i = 0; i < seatNumberVec.size(); i++)
			 {
				 if (purchaseSeat == seatNumberVec[i])
				 {
					 cout << "The seat is not available!" << endl;
					 purchaseSeat = "0";
					 break;
				 }
			 }
		 } while (purchaseSeat == "0");


		 int curNum;

		 if (ticketVec.size() != 0)
		 {
			 string curTicketID = ticketVec.back();
			 curTicketID = curTicketID.substr(2, 3);
			 stringstream gek(curTicketID);
			 gek >> curNum;
		 }
		 else
		 {
			 curNum = 0;
		 }
		 curNum++;

		 if (curNum >= 100)
		 {
			 purchaseTicketID = "TI" + to_string(curNum);
		 }
		 else if (curNum >= 10)
		 {
			 purchaseTicketID = "TI0" + to_string(curNum);
		 }
		 else
		 {
			 purchaseTicketID = "TI00" + to_string(curNum);
		 }

		 time_t now = time(0);
		 tm* ltm = localtime(&now);

		 int year = 1900 + ltm->tm_year;
		 int month = 1 + ltm->tm_mon;
		 int day = ltm->tm_mday;
		 purchaseDate = to_string(year) + "-" + to_string(month) + "-" + to_string(day);



		 string insertTicket = "insert into ticket VALUES('" + IC + "', '" + purchaseTripID + "', '" + purchaseTicketID + "', '" + purchaseSeat + "', '" + purchaseDate + "', '" + to_string(finalPrice) + "')";
		 const char* it = insertTicket.c_str();
		 qstate = mysql_query(conn, it);

		 // receipt
		 system("cls");
		 customerReceipt(IC, purchaseTicketID, purchaseTripID, purchaseRailwayID, purchaseDepartStation, purchaseDestinatedStation, purchaseSeat, tripDate, purchaseTimeDepart, purchaseTimeArrive, finalPrice);

		 // ask if want to purchase another ticket
		 do
		 {
			 cout << "Do you want to purchase another ticket? (y/n): ";
			 cin >> purchaseAgain;
			 cin.ignore(80, '\n');

			 if (purchaseAgain != "Y" && purchaseAgain != "y" && purchaseAgain != "N" && purchaseAgain != "n")
			 {
				 cout << "Wrong input! Try again!" << endl;
			 }
		 } while (purchaseAgain != "Y" && purchaseAgain != "y" && purchaseAgain != "N" && purchaseAgain != "n");

	 }while (purchaseAgain == "Y" || purchaseAgain == "y");
 }

 void delPurchaseTicket(string IC)
 {
	 string delAgain;
	 do
	 {
		 int year, month, day;
		 string curDate, curMonth, curDay;
		 vector<string> ticketIDVec;
		 string delYes;
		 string delContinue;
		 string delTicketID;
		 string delTicketDate;
		 string delDateDiff;
		 int delDateDiffInt;
		 char ticketValid = 'n';
		 vector<string> ticketIDVecValid;
		 char delCustomer = 'n';


		 // get date and only shows ticket that is in the future
		 time_t now = time(0);
		 tm* ltm = localtime(&now);

		 year = 1900 + ltm->tm_year;
		 month = 1 + ltm->tm_mon;
		 day = ltm->tm_mday;

		 if (month < 10)
		 {
			 curMonth = "0" + to_string(month);
		 }
		 else
		 {
			 curMonth = to_string(month);
		 }

		 if (day < 10)
		 {
			 curDay = "0" + to_string(day);
		 }
		 else
		 {
			 curDay = to_string(day);
		 }

		 curDate = to_string(year) + "-" + curMonth + "-" + curDay;


		 system("cls");
		 // store the ticket ID for validation purpose
		 string retrieveTicketID = "select Ticket_ID from ticket natural join trip where customer_IC = '" + IC + "' AND trip_date >= '" + curDate + "'";
		 const char* rti = retrieveTicketID.c_str();
		 qstate = mysql_query(conn, rti);
		 if (!qstate)
		 {
			 res = mysql_store_result(conn);
			 if (res->row_count >= 1)
			 {
				 while ((row = mysql_fetch_row(res)))
				 {
					 ticketIDVec.push_back(row[0]);
				 }
			 }
			 else
			 {
				 cout << "You have not purchased any ticket! Enter any key to back to customer menu..." << endl;
				 _getch();
				 break;
			 }
		 }

		 cout << "**********************************************" << endl;
		 cout << "|                 Ticket Info                |" << endl;
		 cout << "**********************************************" << endl;
		 string selectTicketInfo = "select Ticket_ID, Trip_ID, Seat_Number, trip_date, Final_Price from ticket natural join trip where customer_IC = '" + IC + "' AND trip_date >= '" + curDate + "'";
		 const char* sti = selectTicketInfo.c_str();
		 qstate = mysql_query(conn, sti);
		 if (!qstate)
		 {
			 res = mysql_store_result(conn);
			 if (res->row_count >= 1)
			 {
				 cout << setw(15) << left << "Ticket ID" << setw(15) << left << "Trip ID" << setw(15) << left
					 << "Seat Number" << setw(15) << left << "Trip Date" << setw(20) << left << "Final Price (RM)" << endl;
				 while (row = mysql_fetch_row(res))
				 {
					 cout << setw(15) << left << row[0] << setw(15) << left << row[1] << setw(15) << left << row[2] << setw(15) << left << row[3] << setw(20) << left << row[4] << endl;
				 }
			 }
		 }

		 for (int i = 0; i < 3; i++)
		 {
			 cout << endl;
		 }

		 do
		 {
			 cout << "Do you want to delete any ticket? (y/n): ";
			 cin >> delYes;
			 cin.ignore(80, '\n');

			 if (delYes == "N" || delYes == "n")
			 {
				 delContinue = "n";
			 }
			 else if (delYes == "Y" || delYes == "y")
			 {
				 delContinue = "y";
			 }
			 else
			 {
				 cout << "Wrong Input! Try Again!" << endl;
			 }
		 } while (delYes != "Y" && delYes != "y" && delYes != "N" && delYes != "n");

		 for (int i = 0; i < 3; i++)
		 {
			 cout << endl;
		 }

		 if (delContinue == "Y" || delContinue == "y")
		 {
			 do
			 {
				 cout << "Enter the Ticket ID to delete (at least 2 days before the trip date): ";
				 cin >> delTicketID;

				 for (int i = 0; i < ticketIDVec.size(); i++)
				 {
					 if (delTicketID == ticketIDVec[i])
					 {
						 ticketValid = 'y';

						 // check if the ticket is at least 2 days in advance 
						 // take the trip_date of the ticket ID and compare to curDate by using the sql code below
						 // SELECT DATEDIFF(day, '2017/08/25', '2011/08/25') AS DateDiff;
						 string retrieveDateValidate = "select trip_date from trip where trip_ID = (select trip_ID from ticket where ticket_ID = '" + delTicketID + "')";
						 const char* rdv = retrieveDateValidate.c_str();
						 qstate = mysql_query(conn, rdv);
						 if (!qstate)
						 {
							 res = mysql_store_result(conn);
							 if (res->row_count == 1)
							 {
								 row = mysql_fetch_row(res);
								 delTicketDate = row[0];
							 }
						 }

						 string retrieveDateDiff = "select DATEDIFF('" + delTicketDate + "', '" + curDate + "')";
						 const char* rdd = retrieveDateDiff.c_str();
						 qstate = mysql_query(conn, rdd);
						 if (!qstate)
						 {
							 res = mysql_store_result(conn);
							 if (res->row_count == 1)
							 {
								 row = mysql_fetch_row(res);
								 delDateDiff = row[0];
							 }
						 }

						 stringstream gk(delDateDiff);
						 gk >> delDateDiffInt;

						 if (delDateDiffInt >= 2)
						 {
							 string deleteTicket = "delete from ticket where ticket_ID = '" + delTicketID + "')";
							 const char* dt = deleteTicket.c_str();
							 qstate = mysql_query(conn, dt);
						 }
						 else
						 {
							 cout << "Cannot delete the ticket as today is less than 2 days away from the trip date.\n" << endl;
							 ticketValid = 'N';
						 }
						 break;
					 }
					 else
					 {
						 ticketValid = 'n';
					 }
				 }

				 if (ticketValid == 'n')
				 {
					 cout << "No such Ticket ID! Try Again!" << endl;
				 }
			 } while (ticketValid == 'n' || ticketValid == 'N');


			 string deleteTicketID = "delete from ticket where ticket_ID = '" + delTicketID + "'";
			 const char* dti = deleteTicketID.c_str();
			 qstate = mysql_query(conn, dti);
			 res = mysql_store_result(conn);

			 cout << "\n" << endl;

			 // check the ticket ID again, if no more, then will delete the customer and quit back to customer menu
			 string retrieveTicketIDValid = "select Ticket_ID from ticket where customer_IC = '" + IC + "'";
			 const char* rtiv = retrieveTicketIDValid.c_str();
			 qstate = mysql_query(conn, rtiv);
			 if (!qstate)
			 {
				 res = mysql_store_result(conn);
				 if (res->row_count >= 1)
				 {
					 while ((row = mysql_fetch_row(res)))
					 {
						 ticketIDVecValid.push_back(row[0]);
					 }
				 }
				 else	// no more ticket ID, delete the customer and back straight back to customer menu
				 {
					 string deleteCustomer = "delete from customer where customer_IC = '" + IC + "'";
					 const char* dc = deleteCustomer.c_str();
					 qstate = mysql_query(conn, dc);
					 res = mysql_store_result(conn);
					 delCustomer = 'y';
					 delAgain = "n";
				 }
			 }

			do
			{
				cout << "Do you want to delete another ticket? (y/n): ";
				cin >> delAgain;
				cin.ignore(80, '\n');

				if (delAgain != "Y" && delAgain != "y" && delAgain != "N" && delAgain != "n")
				{
					cout << "Wrong Input! Try Again!" << endl;
				}
				else if (delAgain == "Y" || delAgain == "y")
				{
					return delPurchaseTicket(IC);
				}
				else if (delAgain == "N" || delAgain == "n")
				{
					cout << "Enter any key to back to customer menu...";
					_getch();
				}
				
			} while (delAgain != "Y" && delAgain != "y" && delAgain != "N" && delAgain != "n");
			



		 }
		 else
		 {
			 cout << "Enter any key to back to customer menu...";
			 _getch();
		 }
	 }while (delAgain == "Y" || delAgain == "y");
	 
 }


 void reportTicketList()
 {
	 int ticketRotate = 0;
	 int totalTicket = 0;
	 string nextPage;
	 system("cls");

	 // retrieve the size of the trip then use for the next page stuff
	 string retrieveTotalTicket = "select ticket_ID from ticket";
	 const char* rtt = retrieveTotalTicket.c_str();
	 qstate = mysql_query(conn, rtt);

	 if (!qstate)
	 {
		 res = mysql_store_result(conn);
		 if (res->row_count >= 1)
		 {
			 while (row = mysql_fetch_row(res))
			 {
				 totalTicket++;
			 }
		 }
	 }

	 cout << "**********************************************" << endl;
	 cout << "|                 Ticket List                |" << endl;
	 cout << "**********************************************" << endl;

	 string retrieveReport = "select Ticket_ID, Trip_ID, Seat_Number, Trip_Date, final_price from ticket natural join trip";
	 const char* rr = retrieveReport.c_str();
	 qstate = mysql_query(conn, rr);

	 if (!qstate)
	 {
		 res = mysql_store_result(conn);
		 if (res->row_count >= 1)
		 {
			 cout << setw(15) << left << "Ticket ID" << setw(15) << left << "Trip ID" << setw(15) << left << "Seat Number" << setw(15) << left << "Trip Date"
				 << setw(20) << left << "Final Price (RM)" << endl;
			 while (row = mysql_fetch_row(res))
			 {
				 cout << setw(15) << left << row[0] << setw(15) << left << row[1] << setw(15) << left << row[2] << setw(15) << left << row[3]
					 << setw(20) << left << row[4] << endl;

				 ticketRotate++;
				 if (ticketRotate % 10 == 0)
				 {
					 cout << "Enter Y for next page or N to choose proceed (Y/N): ";
					 cin >> nextPage;
					 cin.ignore(80, '\n');

					 if (nextPage == "Y" || nextPage == "y")
					 {
						 system("cls");
						 cout << "**********************************************" << endl;
						 cout << "|                 Ticket List                |" << endl;
						 cout << "**********************************************" << endl;
						 cout << setw(15) << left << "Ticket ID" << setw(15) << left << "Trip ID" << setw(15) << left << "Seat Number" << setw(15) << left << "Trip Date"
							 << setw(20) << left << "Final Price (RM)" << endl;
					 }
					 else if (nextPage == "N" || nextPage == "n")
					 {
						 break;
					 }
				 }
				 else if (ticketRotate == totalTicket)
				 {
					 cout << "All ticket has been shown." << endl;
					 cout << "Enter Y to view again or N to proceed (Y/N): ";
					 cin >> nextPage;

					 if (nextPage == "Y" || nextPage == "y")
					 {
						 return reportTicketList();
					 }
					 else if (nextPage == "N" || nextPage == "n")
					 {
						 break;
					 }
				 }
			 }
		 }
		 else
		 {
			 cout << "There are no ticket purchased yet." << endl;
			 for (int i = 0; i < 3; i++)
			 {
				 cout << endl;
			 }
			 cout << "Enter anything to back to admin menu..." << endl;
			 _getch();
			 return adminMenu();
		 }
	 }

 }