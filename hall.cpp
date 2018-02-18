//NOTES:
//	MAKE TOTAL REVENUE WORK THROUGH AN EXTRA STRUCT
//	MEMBER FOR EACH CUSTID TO ACCOUNT FOR DISCOUNTS
//	cin string for day, compare to any, then covert to ints
//check lo_cont for skip col 4 -> 16 if condition is == or >


	
	


#include "hall.h"

const int MAX_COLS = 20;
const int MAX_ROWS = 14;
const int HI_TIX_PRICE = 200;
const int LO_TIX_PRICE = 150;
const int HI_PRICE_ROWS = 8;
const int HI_PRICE_COLS_LEFT = 4;
const int HI_PRICE_COLS_RIGHT = 16;
const int LO_RO_TOP = 0;
const int LO_RO_MID = 8;
const int LO_LEFT_COL_LEFT = 0;
const int LO_LEFT_COL_RIGHT = 4;
const int LO_RIGHT_COL_LEFT = 16;
const int LO_RIGHT_COL_RIGHT = 20;
const int LO_ROW_BOTTOM = 14;
const double DISCOUNT = 0.9;

//Default constructor
//purpose: initialize the class and data
//arguments: none
//rets: none
//effects: prepares the class's seating chart + instantiates the class
Hall::Hall()
{
	//Initialize entire chart
	for (int i = 0; i < MAX_ROWS; i++){
		for (int k = 0; k < MAX_COLS; k++){
			grid[i][k].ID = "---";
			grid[i][k].cost = 0;
			grid[i][k].disc = false;
		}
	}
}


//request(Request r)
//purpose: Separate request into price range dividers
//arguments:the user inputed struct
//returns:0
//effects: a level of data sorting
void Hall::request(Request r)
{
	if (r.price_range == "lo"){
		lo_price_func(r);
	}
	else if (r.price_range == "hi"){
		hi_price_func(r);
	}
	else if (r.price_range == "any"){
		any_price_func(r);
	}
}


//lo_price_func()
//purp: hub to call helper functions for lo range requests
//args: Request struct
//rets: none
//effects: Begins seating client or tells them if it's not possible
void Hall::lo_price_func(Request r)
{
	bool dups = check_dup(r);
	bool over_limit = check_num_requested(r);
	string cont = lo_cont(r);
	
	if(dups == false && over_limit == false && cont == "success"){
		r.gets_discount = false;
		fill_seats(r);
		}
	else if (dups == false && over_limit == false && any_lo_seats(r)){
		give discount
		fill seats
	}
}

//check_dup()
//purp: checks specific day's chart for another reservation from client
//args: Request struct
//rets: bool
//effects: Enforces one of the rules of no dub reservations per day
bool Hall::check_dup(Reqest r)
{
	bool val = false;
	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (chart[i][j].ID == r.custID){
				cout << "No " 
				<< r.custID 
				<< " already-reserved\n";
				val = true;
			}
		}
	}
	return val;
}

bool Hall::check_num_requested(Request r)
{
	bool too_many = false;
	
	if (r.tix_requested > 12 || r.tix_requested < 1){
		cout << "No " << r_person.custID << " bad-number\n";
		too_many = true;
	}
	
	return too_many;
}

//lo_cont()
//purpose:checks for continuous seats for a reservation
//args: the request struct
//rets: string "success" or "failure"
//effects: Applies discount for customer if appropriate
string Hall::lo_cont(Request r)
{
	int row = 0, col = 0, seats_found = 0;
		
	if(r.tix_requested > 4){ 				//immediate jump to lower rows
		row = 8;							//if tix requested > 4
	}
	do {
		if (col == 4 && row < 8){	//If test reaches col 4 below row 8
			col = 16;				//jump to other side of hi price range
			seats_found = 0;		//reset counter. (skips premium seat block)
		}		
		if(col == 20){
			row++;
			col = 0;
			seats_found = 0;
		}	
		if(row > 13)				//if end of array reached within while loop
			return "failure";
		else if(grid[row][col].ID = "---")
			seats_found++;
		else
			seats_found = 0;
		j++;
	} while(seats_found < r.tix_requested);
	r.start_row = row;
	r.start_col = col;
	
	fill_seats(r, row, col);	//Seat the customer
	return "success";
}

void Hall::fill_seats(Request r, int row, int col)
{
	for(int k = r.start_col-r.tix_requested; k < r.start_col; k++){
		grid[r.start_row][k].ID = r.custID;
		if(r.gets_discount == true)
			grid[r.start_row][k].cost = (0.9*LO_TIX_PRICE);
		else
			grid[r.start_row][k].cost = (LO_TIX_PRICE);
	}
	
	print_receipt(r);
}

void Hall::print_receipt(Request r)
{
	double cust_cost = calc_cust_cost(r);
	
	cout << "OK " << r.custID << " " << cust_cost << " " << r.tix_requested
		 << " " << r.day << " " << print_seat_codes(r);	
}

double Hall::calc_cust_cost(Request r)
{
	double sum = 0;
	
	for(int k = r.start_col-r.tix_requested; k < r.start_col; k++){
		sum = sum + grid[r.start_row][k].cost;
	}
	
	return sum;
}

void Hall::print_seat_codes(Request r_person)
{
	char rowletter;
	
	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (grid[i][j].ID == r_person.custID){
				rowletter = 'A' + i;
				cout << rowletter << j+1 << " ";
			}
		}
	}
	cout << endl;
}



/*
bool Hall::is_continuous_lo(Request r)
{
	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (chart[i][j] == "---" 
			   && (i < LO_RO_MID 
			   && (j < LO_LEFT_COL_RIGHT
			   || j < LO_RIGHT_COL_LEFT))){
				counter++;
			}
			
			else if (chart[i][j] == "---" && i >= LO_RO_MID){
				counter++;
			}
			else if (chart[i][j] != "---"){
				counter = 0;
			}
			else if (i > LO_RO_MID && (j > LO_LEFT_COL_RIGHT || j > LO_RIGHT_COL_LEFT))
				counter = 0;
				
			if (counter >= r_person.tix_requested){
				infostruct.row = i;
				infostruct.col = j;
				infostruct.success = true;
			}
		}
		counter = 0;
	}


/*
void Hall::hi_price_func(Request r)
{	
	if(!is_continuous_hi)
		r.gets_discount = true;
	
	if(is_available(r))
		insert(r);
}


/*void cancel()
	purpose: cancels reservations
	args: A struct with user info for cancellation
	rets: none
	effects: Cancels request
	notes:none

void Hall::cancel(Cancel c)
{
	bool success = false;

	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (grid[i][j] == c.custID){
				grid[i][j] = "---";
				success = true;
			}
		}
	}

	if (success == true){
		cout << "OK " << c_person1.custID << " cancelled\n";
	}
	else{
		cout << "NO " << c_person1.custID << " no-reservation\n";
	}

}

void Hall::print()
{
	//double revenue;
	
	for (int i = 0; i < MAX_ROWS; i++){
		for (int k = 0; k < MAX_COLS; k++){
			cout << grid[i][k] << " ";
		}
		cout << endl;
		
	}
	
	//revenue = total_revenue();
	//cout << "Revenue: " << revenue << ".00" << endl;
	
}
//lo_price_func
//purpose: Calls helper functions to determine availability of request
//			and ultimately insert the customer into the seats
//args: The Request struct
//rets: none
//effects: Last barrier before customer actually is inserted