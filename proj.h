/*
	Rafael Girão - ist199309
	1st IAED Project 2021/2022
*/
#define LIM_AIRPORT_ID 30+1
#define LIM_COUNTRY_NAME 30+1
#define LIM_CITY_NAME 50+1
#define LIM_FLIGHT_CODE 6+1
#define LIM_DATE 10+1
#define LIM_RAW_TIME 5+1
#define MAX_AIRPORTS 40
#define MAX_FLIGHTS 30000
#define MIN_FLIGHT_CAP 10
#define MAX_FLIGHT_CAP 100

/*-------------------*/
/*-----Structures----*/
/*-------------------*/
typedef struct {
	int day;
	int month;
	int year;
} Date;

typedef struct {
	int hour;
	int minute;
} mTime;

typedef struct {
	char id[LIM_AIRPORT_ID];
	char country[LIM_COUNTRY_NAME];
	char city[LIM_CITY_NAME];
	int arrival_count, departure_count;
	/*Array of ints to access flights in the flight store*/
	int arrivals[MAX_FLIGHTS];
	int departures[MAX_FLIGHTS];
	/*These are 1 if values have been added to arrays above since last sort, 0 otherwise*/
	/*This way sorting only occurs if dirty is 1 and if a function is going to use the array*/
	int arrival_dirty;
	int departure_dirty;
} Airport;

typedef struct {
	char code[LIM_FLIGHT_CODE];
	char departure_id[LIM_AIRPORT_ID];
	char arrival_id[LIM_AIRPORT_ID];
	Date departure_date;
	mTime departure_time;
	mTime duration;
	Date arrival_date;
	mTime arrival_time;
	int capacity;

} Flight;


/*TODO: go through every function and order their prototypes
 by order of appearance*/
/*-------------------*/
/*----Prototypes-----*/
/*-------------------*/
void addAirport();
void listAirports();
int getAirport(char arprt_id[]);
void sortAirports();

void handleVCommand();
void addFlight();
int checkFlightInput(Flight flight);
void listFlights();
void listAirportDepartures();
void listAirportArrivals();
void bubbleSortFlights(int flights[], int flight_count, int departures);
int compareFlights(Flight *flight_a, Flight *flight_b, int departures);

/*datestruct.c*/
Date readDate();
void printDate(Date date);
void setDate();
int compareDate(Date data1, Date data2);
Date incDate(Date date);
Date incDateAux(Date date, int month_days);

/*timestruct.c*/
mTime readTime();
void printTime();
int compareTime(mTime time1, mTime time2);
mTime addTime(mTime time1, mTime time2);


/*-------------------*/
/*-----Messages------*/
/*-------------------*/

#define MSG_INVALID_FLIGHT_CODE "invalid flight code\n"
#define MSG_INVALID_AIRPORT_ID "invalid airport ID\n"
#define MSG_TOO_MANY_AIRPORTS "too many airports\n"
#define MSG_DUPLICATE_AIRPORT "duplicate airport\n"
#define MSG_NO_SUCH_AIRPORT_ID "%s: no such airport ID\n"
#define MSG_FLIGHT_ALREADY_EXISTS "flight already exists\n"
#define MSG_TOO_MANY_FLIGHTS "too many flights\n"
#define MSG_INVALID_DATE "invalid date\n"
#define MSG_INVALID_DURATION "invalid duration\n"
#define MSG_INVALID_CAPACITY "invalid capacity\n"

#define MSG_NEW_AIRPORT "airport %s\n"
