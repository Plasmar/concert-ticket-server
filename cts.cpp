#include <iostream>

using namespace std;
//
// cts.cpp
// comp11(1630) -- proj1
// Cameron Merrick
// March 12, 2013
//
// A concert ticket server to handle user requests
// for requesting and purchasing a ticket for a concert
// as well as cancelling reservations, and printing
// the seating chart for the user to view.
// The server also handles price discrimination for 
// parties that cannot be sat in a continuous row, as well 
// as for user defined price ranges.
//
//

struct Request {		//Data struct to package
	string rq_type;		//the user input information
	string custID;		//for requests
	int tix_requested;
	string price_range;
};

struct Cancel {			//Data struct to package
	string rq_type;		//the user input information
	string custID;		//for cancels
};

struct Continuous{		//Data structure to be used in the functions
	bool success;		//that check the availability of continuous
	int row;		//seats, and also provides row and collumn
	int col;		//of the last available continuous seat
};

const int MAX_COLS = 20;	//constants to be used for
const int MAX_ROWS = 14;	//arrays and general numbers
const int HI_TIX_PRICE = 200;	//used throughout the program
const int LO_TIX_PRICE = 150;
const int HI_PRICE_ROWS = 8;
const int HI_PRICE_COLS_LEFT = 4;
const int HI_PRICE_COLS_RIGHT = 16;
const int LO_RO_TOP = 0;	//The constants below here are representing
const int LO_RO_MID = 8;	//boundries (left and right wing and the 
const int LO_LEFT_COL_LEFT = 0;	//bottom rectangle) of the low price area.    
const int LO_LEFT_COL_RIGHT = 4;
const int LO_RIGHT_COL_LEFT = 16;
const int LO_RIGHT_COL_RIGHT = 20;
const int LO_ROW_BOTTOM = 14;
const double DISCOUNT = 0.9;



//Function declarations (without definitions)
//that will be invoked throughout the program
void input_handler(string chart[][MAX_COLS]);
void request_func(string chart[][MAX_COLS], Request);
void cancel_func(string chart[][MAX_COLS], Cancel);
void print_func(string chart[][MAX_COLS]);
void lo_price_func(string chart[][MAX_COLS], Request);
void hi_price_func(string chart[][MAX_COLS], Request);
void any_price_func(string chart[][MAX_COLS], Request);
bool is_available_lo(string chart[][MAX_COLS], Request);
bool is_available_hi(string chart[][MAX_COLS], Request);
bool is_available_any(string chart[][MAX_COLS], Request);
bool lo_seats_available(string chart[][MAX_COLS], Request);
bool hi_seats_available(string chart[][MAX_COLS], Request);
bool any_seats_available(string chart[][MAX_COLS], Request);
Continuous is_continuous_lo(string chart[][MAX_COLS], Request);
Continuous is_continuous_hi(string chart[][MAX_COLS], Request);
Continuous is_continuous_any(string chart[][MAX_COLS], Request);
void print_receipt_fullprice_lo(string chart[][MAX_COLS], Request, Continuous);
void print_receipt_discount_lo(string chart[][MAX_COLS], Request, Continuous);
void print_receipt_fullprice_hi(string chart[][MAX_COLS], Request, Continuous);
void print_receipt_discount_hi(string chart[][MAX_COLS], Request, Continuous);
void print_receipt_fullprice_any(string chart[][MAX_COLS], Request, Continuous);
void print_receipt_discount_any(string chart[][MAX_COLS], Request, Continuous);
double calc_cust_cost(string chart[][MAX_COLS], Request, Continuous);
double calc_cost_any(string chart[][MAX_COLS], Request, Continuous);
void assigned_seats(string chart[][MAX_COLS], Request);
int total_revenue(string chart[][MAX_COLS]);

/*main
	purpose: initialize seating array and 
		invoke the input_handler function.
	arguments: none
	returns: 0
	effects: sets up the array to be used throughout
		the whole program.
	notes: none
*/
int main()
{
	//initialize seating array to be used throughout
	//the program
	
	string grid[MAX_ROWS][MAX_COLS];
	
	for (int i = 0; i < MAX_ROWS; i++){
		for (int k = 0; k < MAX_COLS; k++){
			grid[i][k] = "---";
		}
	}
	
	
	
	input_handler(grid); //invoke the input handler function
	
	return 0;
	
}

/*void input_handler
	purpose: read in user input for menu, determine
		structure type to be used, and pass the 
		struct and seating array to the appropriate
		function. Also prints error messages.
	arguments: a grid array
	returns: 0
	effects: behaves as the looping menu that invokes
		the appropriate actions and functions based
		on the input received.
	notes: none
*/
void input_handler(string chart[][MAX_COLS])
{
	string type;	//declaring variables for user input
	string custID;	//that will be inserted into the appropriate
	int num_tix;	//data structure
	string location;
	
	Request r_person;  //declaring a variable of the request struct
	Cancel c_person;   //declaring a variable of the cancel struct

	do {
		cin >> type;
		if (type == "RQ"){
			cin >> custID >> num_tix >> location;
			
			r_person.rq_type = type;
			r_person.custID = custID;
			r_person.tix_requested = num_tix;
			r_person.price_range = location;
			
			request_func(chart, r_person);
		}
		
		else if (type == "CA"){
			cin >> custID;
			c_person.custID = custID;
			cancel_func(chart, c_person);
		}
		
		else if (type == "PR"){
			print_func(chart);
		}
	} while (type != "QU");
}


/*void request_func
	purpose: takes in  the input struct and grid array and 
		determines which of the functions (hi, lo, any)
		to invoke based on reading the members of the 
		input struct.
	arguments: the grid array, and the user inputed struct
	returns:0
	effects: completes another level of data sorting to finally
		invoke the functions that will output useful info
		and make significant calculations
	notes: none
*/
void request_func(string chart[][MAX_COLS], Request r_person1)
{
	if (r_person1.price_range == "lo"){
		lo_price_func(chart, r_person1);
	}
	else if (r_person1.price_range == "hi"){
		hi_price_func(chart, r_person1);
	}
	else if (r_person1.price_range == "any"){
		any_price_func(chart, r_person1);
	}
}

/*void cancel
	purpose: to remove the reservations from the seating
		array of a user inputed custID.
	arguments: the grid array, and the user inputed struct.
	returns: 0
	effects: creates more open in the seating array for other 
		users to purchase later on.
	notes: none
*/
void cancel_func(string chart[][MAX_COLS], Cancel c_person1)
{
	bool success = false;

	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (chart[i][j] == c_person1.custID){
				chart[i][j] = "---";
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


/*
void print_func
	purpose: prints the row of the 2D array with the custID names
		in it to confirm the purchase to the customer.  Also
		invokes a function to compute the final price for the 
		customer.
	arguments: the grid array
	returns: none
	effects: with this function is working properly, we will be able to see
		the updated array.
	notes: this function will compute the revenue as well, after printing
		out the array, so that the output is how it appears
		on the instructions.
*/
void print_func(string chart[][MAX_COLS])
{
	double revenue;
	
	for (int i = 0; i < MAX_ROWS; i++){
		for (int k = 0; k < MAX_COLS; k++){
			cout << chart[i][k] << " ";
		}
		cout << endl;
		
	}
	
	revenue = total_revenue(chart);
	cout << "Revenue: " << revenue << ".00" << endl;
	
}

int total_revenue(string chart[][MAX_COLS])
{
	int num_high_seats = 0;
	int num_lo_seats = 0;
	int rev;
	
	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (i < HI_PRICE_ROWS
			   && (j > HI_PRICE_COLS_LEFT
			   && j < HI_PRICE_COLS_RIGHT)){
				if (chart[i][j] != "---"){
					num_high_seats++;
				}
			   }
			   else{
				   if (chart[i][j] != "---"){
					   num_lo_seats++;
				   }
			   }
		}
	}
	
	rev = (num_high_seats * HI_TIX_PRICE) + (num_lo_seats * LO_TIX_PRICE);
	return rev;
}


/*
lo_price_func
	purp: For LO price requests: invoke funcs to determine 
		conditions of availability 
		and whether or not the party can be seated continuously
		and then move on to functions to update the array and print
		the receipt based on said conditions.
	Args: the seating chart, the request struct
	Returns: void
	Effects: Invokes functions that do the legwork of determining
		availaibility and thus overall success of the customer's
		request.
	Notes:
*/
void lo_price_func(string chart[][MAX_COLS], Request r_person_lo)
{
	bool available = is_available_lo(chart, r_person_lo);
	Continuous info = is_continuous_lo(chart, r_person_lo);

	if (( available == true) 
	   && (info.success == true )) {
		print_receipt_fullprice_lo(chart, r_person_lo, info);
		}
		
	else if (( available == true) 
		&& (info.success == false )) {
		print_receipt_discount_lo(chart, r_person_lo, info);
		}
	
	
	
}

/*
hi_price_func
	purp: For HI price requests: invoke funcs to determine 
		conditions of availability 
		and whether or not the party can be seated continuously
		and then move on to functions to update the array and print
		the receipt based on said conditions.
	Args: the seating chart, the request struct
	Returns: void
	Effects: Invokes functions that do the legwork of determining
		availaibility and thus overall success of the customer's
		request.
	Notes:
*/
void hi_price_func(string chart[][MAX_COLS], Request r_person_hi)
{
	bool available = is_available_hi(chart, r_person_hi);
	Continuous info = is_continuous_hi(chart, r_person_hi);

	if (( available == true) 
	   && (info.success == true )) {
	   print_receipt_fullprice_hi(chart, r_person_hi, info);
	}
		
	else if (( available == true) 
		&& (info.success == false )) {
		print_receipt_discount_hi(chart, r_person_hi, info);
	}
	
}

/*
any_price_func
	purp: For ANY price requests: invoke funcs to determine 
		conditions of availability 
		and whether or not the party can be seated continuously
		and then move on to functions to update the array and print
		the receipt based on said conditions.
	Args: the seating chart, the request struct
	Returns: void
	Effects: Invokes functions that do the legwork of determining
		availaibility and thus overall success of the customer's
		request.
	Notes:
*/
void any_price_func(string chart[][MAX_COLS], Request r_person_any)
{
	bool available = is_available_any(chart, r_person_any);
	Continuous info = is_continuous_any(chart, r_person_any);
	
	if (( available == true)
	   && (info.success == true)) {
		print_receipt_fullprice_any(chart, r_person_any, info);
	}
	   
	else if (( available == true)
		&& (info.success == false)) {
			print_receipt_discount_any(chart, r_person_any, info);
	}
	
}

/*
bool is_available_lo
	purp: For LO requests: Determines availability of seats requested
	Args: the seating chart, the request struct
	Returns: bool
	Effects: Determines whether or not the seating request can ultimately
		be possible
	Notes: *Invokes another function (lo_seats_available) used to traverse
		the array for the low section to count if the number of seats
		requested is available
*/
bool is_available_lo(string chart[][MAX_COLS], Request r_person)
{
	bool val = true;
	
	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (chart[i][j] == r_person.custID){
				cout << "No " 
				<< r_person.custID 
				<< " already-reserved\n";
				val = false;
				return val;
			}	
		}
	}
	
	if (r_person.tix_requested > 12 || r_person.tix_requested < 1){
		cout << "No " << r_person.custID << " bad-number\n";
		val = false;
	}
	else if (lo_seats_available(chart, r_person) == false){
		cout << "No " << r_person.custID << " not-available\n";
	}
	
	return val;
}


/*
bool is_available_hi
	purp: For HI requests: Determines availability of seats requested
	Args: the seating chart, the request struct
	Returns: bool
	Effects: Determines whether or not the seating request can ultimately
		be possible
	Notes: *Invokes another function (hi_seats_available) used to traverse
		the array for the high section to count if the number of seats
		requested is available
*/
bool is_available_hi(string chart[][MAX_COLS], Request r_person)
{
	bool val = true;
	
	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (chart[i][j] == r_person.custID){
				cout << "No "
				<< r_person.custID
				<< " already-reserved\n";
				val = false;
				return val;
			}	
		}
	}
	
	if (r_person.tix_requested > 12 || r_person.tix_requested < 1){
		cout << "No " << r_person.custID << " bad-number\n";
		val = false;
	}
	else if (hi_seats_available(chart, r_person) == false){
		cout << "No " << r_person.custID << " not-available\n";
	}
	
	return val;
}


/*
bool is_available_any
	purp: For ANY requests: Determines availability of seats requested
	Args: the seating chart, the request struct
	Returns: bool
	Effects: Determines whether or not the seating request can ultimately
		be possible
	Notes: *Invokes another function (any_seats_available) used to traverse
		the array for either section to count if the number of seats
		requested is available
*/
bool is_available_any(string chart[][MAX_COLS], Request r_person)
{
	bool val = true;
	
	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (chart[i][j] == r_person.custID){
				cout << "No "
				<< r_person.custID
				<< " already-reserved\n";
				val = false;
				return val;
			}	
		}
	}
	
	if (r_person.tix_requested > 12 || r_person.tix_requested < 1){
		cout << "No " << r_person.custID << " bad-number\n";
		val = false;
	}
	else if (any_seats_available(chart, r_person) == false){
		cout << "No " << r_person.custID << " not-available\n";
	}
	
	return val;
}


/*
bool lo_seats_available
	purp: Traverse the array's low section to deterimine if the amount
		of seats requested can be met.
	Returns: bool
	Args: the array, the request struct
	Effects: Loops across the 2d Array with a counter for available seats
	Notes: Uses constants to describe the borders of the price sections
*/
bool lo_seats_available(string chart[][MAX_COLS], Request r_person)
{
	bool val = true;
	int seats_open;

	for (int i = LO_RO_TOP; i < LO_RO_MID; i++){
		for (int j = LO_LEFT_COL_LEFT; j < LO_LEFT_COL_RIGHT; j++){
			if (chart[i][j] == "---"){
				seats_open++;
			}
		}
	}
	for (int i = LO_RO_TOP; i < LO_RO_MID; i++){
		for (int j = LO_RIGHT_COL_LEFT; j < LO_RIGHT_COL_RIGHT; j++){
			if (chart[i][j] == "---"){
				seats_open++;
			}
		}
	}
	for (int i = LO_RO_MID; i < LO_ROW_BOTTOM; i++){
		for (int j = LO_RO_TOP; j < LO_RIGHT_COL_RIGHT; j++){
			if (chart[i][j] == "---"){
				seats_open++;
			}
		}
	}
	if (seats_open < r_person.tix_requested){
		val = false;
	}	
	return val;
}


/*
bool hi_seats_available
	purp: Traverse the array's high section to deterimine if the amount
		of seats requested can be met.
	Returns: bool
	Args: the array, the request struct
	Effects: Loops across the 2d Array with a counter for available seats
	Notes: Uses constants to describe the borders of the price sections
*/
bool hi_seats_available(string chart[][MAX_COLS], Request r_person)
{
	bool val = true;
	int seats_open;
	
	for (int i = 0; i < HI_PRICE_ROWS; i++){
		for (int j = HI_PRICE_COLS_LEFT; j < HI_PRICE_COLS_RIGHT; j++){
			if (chart[i][j] == "---"){
				seats_open++;
			}
		}
	}
	
	if (seats_open < r_person.tix_requested){
		val = false;
	}
	
	return val;
}

/*
bool any_seats_available
	purp: Traverse both of the array's sections to deterimine if the amount
		of seats requested can be met.
	Returns: bool
	Args: the array, the request struct
	Effects: Loops across the 2d Array with a counter for available seats
	Notes: Uses constants to describe the borders of the price sections
*/
bool any_seats_available(string chart[][MAX_COLS], Request r_person)
{
	bool val = true;
	int seats_open;
	
	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (chart[i][j] == "---"){
				seats_open++;
			}
		}
	}
	
	if (seats_open < r_person.tix_requested){
		val = false;
	}
	
	return val;
}

/*
Continous is_continuous_lo
	purp: Traverse the 2D array's low section and increment a counter while
		on a given row if continuous open seats are found, and compares
		that to how many seats the user requested.
	Returns: a variable of the data structure 'Continuous'
	Args: the array, the request struct
	Effects: The return of the struct is advantageous because it provides
		success or failure on continuous availability as well as the
		last seat's row and col to be used by later funcs.
	Notes: The function returns the struct to also be able to hold row and
		col numbers for the last seat so that the function that
		customer receipt can be determined later.
*/
Continuous is_continuous_lo(string chart[][MAX_COLS], Request r_person)
{
	Continuous infostruct;
	infostruct.success = false;
	infostruct.row = 0;
	infostruct.col = 0;
	int counter = 0;
	
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
			if (counter >= r_person.tix_requested){
				infostruct.row = i;
				infostruct.col = j;
				infostruct.success = true;
			}
		}
		counter = 0;
	}
	
	return infostruct; 
	
}	

/*
Continous is_continuous_hi
	purp: Traverse the 2D array's high section and increment counter while
		on a given row if continuous open seats are found, and compares 
		that to how many seats the user requested.
	Returns: a variable of the data structure 'Continuous'
	Args: the array, the request struct
	Effects: The return of the struct is advantageous because it provides
		success or failure on continuous availability as well as the
		last seat's row and col to be used by later funcs.
	Notes: The function returns the struct to also be able to hold row and
		col numbers for the last seat so that the function that
		customer receipt can be determined later.
*/
Continuous is_continuous_hi(string chart[][MAX_COLS], Request r_person)
{
	Continuous infostruct;
	infostruct.success = false;
	infostruct.row = 0;
	infostruct.col = 0;
	int counter = 0;
	
	for (int i = 0; i < HI_PRICE_ROWS; i++){
		for (int j = HI_PRICE_COLS_LEFT; j < HI_PRICE_COLS_RIGHT; j++){
			if (chart[i][j] == "---") {
				counter++;
			}
			
			else if (chart[i][j] != "---"){
				counter = 0;
			}
			if (counter >= r_person.tix_requested){
				infostruct.row = i;
				infostruct.col = j;
				infostruct.success = true;
			}
		}
		counter = 0;
	}
	
	return infostruct; 
}

/*
Continous is_continuous_any
	purp: Traverses both of the 2D array's sections and increment 
		a counter while on a given row if continuous open seats 
		are found, and compares that to how many seats the 
		user requested.
	Returns: a variable of the data structure 'Continuous'
	Args: the array, the request struct
	Effects: The return of the struct is advantageous because it provides
		success or failure on continuous availability as well as the
		last seat's row and col to be used by later funcs.
	Notes: The function returns the struct to also be able to hold row and
		col numbers for the last seat so that the function that
		customer receipt can be determined later.
*/
Continuous is_continuous_any(string chart[][MAX_COLS], Request r_person)
{
	Continuous infostruct;
	infostruct.success = false;
	infostruct.row = 0;
	infostruct.col = 0;
	int counter = 0;
	
	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (chart[i][j] == "---") {
				counter++;
			}
			
			else if (chart[i][j] != "---"){
				counter = 0;
			}
			if (counter >= r_person.tix_requested){
				infostruct.row = i;
				infostruct.col = j;
				infostruct.success = true;
			}
		}
		counter = 0;
	}
	
	return infostruct; 
}

/*
void print_receipt_fullprice_lo
	purp: For continuous: update the array with the custID, invoke a func 
		to calc the cost for the customer, and print out the 
		output to the user using another helper function 
		for the seat codes.
	Returns: void
	Args: the array, the request struct, and the Continuous info struct
	Effects: This is where the actual manipulation of the array occurs
	Notes: 
*/
void print_receipt_fullprice_lo(string chart[][MAX_COLS], Request r_person,
Continuous info)
{
	int i, j;
	i = info.row;
	j = info.col;
	double cust_price;

	
	for (int counter = 0; counter < r_person.tix_requested; counter++){
		chart[i][j] = r_person.custID;
		j--;
	}
		
	cust_price = calc_cust_cost(chart, r_person, info);
	
	cout << "OK " << r_person.custID << " " 
	     << cust_price << " " << r_person.tix_requested
	     << " ";
	
	assigned_seats(chart, r_person);
}

/*
void print_receipt_discount_lo
	purp: Traverses the low part of the array and inputs the custID
		in open spaces followed by the output message.  Uses two other
		function calls to calculate cost and determine the seating
		code that the user reads (e.g. A07).
	Returns: void
	Args: the array, the request struct, and the Continuous info struct
	Effects: This is where the actual manipulation of the array occurs
	Notes: 
*/
void print_receipt_discount_lo(string chart[][MAX_COLS], Request r_person,
Continuous info)
{
	double cust_price = 0;
	
	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (chart[i][j] == "---" 
			   && (i < LO_RO_MID 
			   && (j < LO_LEFT_COL_RIGHT
			   || j < LO_RIGHT_COL_LEFT))){
				chart[i][j] = r_person.custID;
			}
		}
	}
	
	cust_price = calc_cust_cost(chart, r_person, info);
	cout << "OK " << r_person.custID << " " 
			<< cust_price << " " << r_person.tix_requested
			<< " ";
	
	assigned_seats(chart, r_person);
}

/*
void print_receipt_fullprice_hi
	purp: For continuous: update the array with the custID, invoke a func
		to calc the cost for the customer, and print out the 
		output to the user using another helper function 
		for the seat codes.
	Returns: void
	Args: the array, the request struct, and the Continuous info struct
	Effects: This is where the actual manipulation of the array occurs
	Notes: 
*/
void print_receipt_fullprice_hi(string chart[][MAX_COLS],
Request r_person, Continuous info)
{
	int i, j;
	i = info.row;
	j = info.col;
	double cust_price;

	
	for (int counter = 0; counter < r_person.tix_requested; counter++){
		chart[i][j] = r_person.custID;
		j--;
	}
		
	cust_price = calc_cust_cost(chart, r_person, info);
	
	cout << "OK " << r_person.custID << " " 
			<< cust_price << " " << r_person.tix_requested
			<< " ";
	
	assigned_seats(chart, r_person);
}

/*
void print_receipt_discount_hi
	purp: Traverses the high part of the array and inputs the custID
		in open spaces followed by the output message.  Uses two other 
		function calls to calculate cost and determine the seating
		code that the user reads (e.g. A07).
	Returns: void
	Args: the array, the request struct, and the Continuous info struct
	Effects: This is where the actual manipulation of the array occurs
	Notes: 
*/
void print_receipt_discount_hi(string chart[][MAX_COLS], Request r_person,
Continuous info)
{
	double cust_price;
	
	for (int i = 0; i < HI_PRICE_ROWS; i++){
		for (int j = HI_PRICE_COLS_LEFT; j < HI_PRICE_COLS_RIGHT; j++){
			if (chart[i][j] == "---") {
				chart[i][j] = r_person.custID;
			}
		}
	}
	cust_price = calc_cust_cost(chart, r_person, info);
	
	cout << "OK " << r_person.custID << " " 
			<< cust_price << " " << r_person.tix_requested
			<< " ";
	
	assigned_seats(chart, r_person);
}

/*
void print_receipt_fullprice_any
	purp: For continuous: update the array with the custID, invoke a func 
		to calc the cost for the customer, and print out the 
		output to the user using another helper function 
		for the seat codes.
	Returns: void
	Args: the array, the request struct, and the Continuous info struct
	Effects: This is where the actual manipulation of the array occurs
	Notes: 
*/
void print_receipt_fullprice_any(string chart[][MAX_COLS],
Request r_person, Continuous info)
{
	int i, j;
	i = info.row;
	j = info.col;
	double cust_price;

	
	for (int counter = 0; counter < r_person.tix_requested; counter++){
		chart[i][j] = r_person.custID;
		j--;
	}
		
	cust_price = calc_cust_cost(chart, r_person, info);
	
	cout << "OK " << r_person.custID << " " 
			<< cust_price << " " << r_person.tix_requested
			<< " ";
	
	assigned_seats(chart, r_person);
}

/*
void print_receipt_discount_any
	purp: Traverses both parts of the array and inputs the custID
		in open spaces followed by the output message.  Uses two other
		function calls to calculate cost and determine the seating
		code that the user reads (e.g. A07).
	Returns: void
	Args: the array, the request struct, and the Continuous info struct
	Effects: This is where the actual manipulation of the array occurs
	Notes: 
*/
void print_receipt_discount_any(string chart[][MAX_COLS], Request r_person,
Continuous info)
{
	double cust_price;
	
	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (chart[i][j] == "---"){
				chart[i][j] = r_person.custID;
			}
		}
	}
	cust_price = calc_cust_cost(chart, r_person, info);
	
	cout << "OK " << r_person.custID << " " 
			<< cust_price << " " << r_person.tix_requested
			<< " ";
	
	assigned_seats(chart, r_person);
}


/*
double calc_cust_cost
	purp: Calculates the cost for the customer based on seating preference
	Returns: void
	Args: the array, the request struct, and the Continuous info struct
	Effects: Determines the price for the customer
	Notes: *Uses a helper function for the 'any' price range.
*/
double calc_cust_cost(string chart[][MAX_COLS], Request r_person,
Continuous cont)
{
	double cost;
	if(cont.success == true && r_person.price_range == "lo"){
		cost = LO_TIX_PRICE*r_person.tix_requested;
	}
	
	else if(cont.success == true && r_person.price_range == "hi"){
		cost = HI_TIX_PRICE*r_person.tix_requested;
	}
	
	else if(cont.success == false && r_person.price_range == "lo"){
		cost = DISCOUNT*(LO_TIX_PRICE*r_person.tix_requested);
	}
	
	else if(cont.success == false && r_person.price_range == "hi"){
		cost = DISCOUNT*(HI_TIX_PRICE*r_person.tix_requested);
	}
	
	else if (r_person.price_range == "any"){
		cost = calc_cost_any(chart, r_person, cont);
	}
	
	return cost;
}

/*
double calc_cost_any
	purp: The helper function to calc_cust_cost which traverses
		the array to determine the price based on whether
		or not the party is sat continuously and in which
		range to calculate the cost.
	Args: the array, the request struct, and the Continuous info struct
	Returns: double cost
	Effects: Determines the price for the customer
	Notes: 
*/
double calc_cost_any(string chart[][MAX_COLS], Request r_person,
Continuous cont)
{
	double cost = 0;

	if ((cont.success == true) && (cont.row < LO_RO_MID) 
		  && (cont.col < LO_LEFT_COL_RIGHT
		    && cont.col < LO_RIGHT_COL_LEFT)){
			cost = LO_TIX_PRICE*r_person.tix_requested;
	}
	else if ((cont.success == true) && (cont.row > LO_RO_MID)){
		cost = LO_TIX_PRICE*r_person.tix_requested;
	}
	else if ((cont.success == false) && (cont.row > LO_RO_MID)){
		cost = DISCOUNT*(LO_TIX_PRICE*r_person.tix_requested);
	}
	else if((cont.success == false) && (cont.row < LO_RO_MID) 
		&& (cont.col < LO_LEFT_COL_RIGHT
		&& cont.col < LO_RIGHT_COL_LEFT)){
			cost = DISCOUNT*(LO_TIX_PRICE*r_person.tix_requested);
	}
	else if ((cont.success == true) && (cont.row < HI_PRICE_ROWS)
		&& (cont.col > HI_PRICE_COLS_LEFT
		&& cont.col < HI_PRICE_COLS_RIGHT)){
			cost = HI_TIX_PRICE*r_person.tix_requested;
	}
	else if ((cont.success == false) && (cont.row < HI_PRICE_ROWS)
		&& (cont.col > HI_PRICE_COLS_LEFT
		&& cont.col < HI_PRICE_COLS_RIGHT)){
			cost = DISCOUNT*(HI_TIX_PRICE*r_person.tix_requested);
	}
	return cost;
}

	
/*
void assigned_seats
	purp: Determines the seating code for the receipt for the customer.
		
	Args: the array, the request struct
	Returns: void
	Effects: Provides the seat row letter and number for the customer
	Notes: **Uses ASCII char codes corresponding to the rows to determine
		the seating code.
*/
void assigned_seats(string chart[][MAX_COLS], Request r_person)
{
	char rowletter;
	
	for (int i = 0; i < MAX_ROWS; i++){
		for (int j = 0; j < MAX_COLS; j++){
			if (chart[i][j] == r_person.custID){
				rowletter = 'A' + i;
				cout << rowletter << j+1 << " ";
			}
		}
	}
	cout << endl;
}

