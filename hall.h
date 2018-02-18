/*
Cameron Merrick
hall.h // proj2
date
*/

#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <iostream>
#include <cstdlib>

struct Seat{
	string ID;	//either "---" for empty or has the custID
	double cost	//revenue gained from seat (based on discount or not)
};

using namespace std;

struct Request {		
	string custID;		//Data struct to package
	int tix_requested;	//user info for Reservations
	string price_range;
	int day;
	bool gets_discount;
	int start_row, start_col;
};

struct Cancel {			//Data struct to package
	string custID;		//the user input information
	int day;			//for cancels
};

class Hall{

	public:
		
		Hall();	//Constructor - initializes the chart on each day
		void request(Request r);
		void cancel(Cancel c);
		void print();
		
	private:
		Seat grid[MAX_ROWS][MAX_COLS];	//Seating chart
		
		//Helper functions
		void request(Request);
		
		bool check_dup(Reqest);	//Checks for duplicate custID's on same day    
		bool check_num_requested(Request);	//Checks # tix requested
		string lo_cont(Request);	//Checks request for continuous seats
		void fill_seats(Request, int, int);
		void print_receipt(Request);
		double calc_cust_cost(Request);
		void print_seat_codes(Request);
				
		
};

#endif

		
