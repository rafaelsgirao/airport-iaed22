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
	char id[LIM_AIRPORT_ID];
	char country[LIM_COUNTRY_NAME];
	char city[LIM_CITY_NAME];
	int arrival_count, departure_count;
	/*Array of ints to access flights in the flight store*/
	int arrivals[MAX_FLIGHTS];
	int departures[MAX_FLIGHTS];
} Airport;

typedef struct {
	char code[LIM_FLIGHT_CODE];
	char departure_id[LIM_AIRPORT_ID];
	char arrival_id[LIM_AIRPORT_ID];
	Date departure_date;
	/*Time and duration are stored in military time format*/
	int departure_time;
	int duration;
	int capacity;

} Flight;
/*TODO: go through every function and order their prototypes
 by order of appearance*/
/*-------------------*/
/*----Prototypes-----*/
/*-------------------*/
void addAirport();
void listAirports();
void sortAirports();
int getAirport(char arprt_id[]);
void addFlight();
void handleVCommand();
void listFlights();
void listAirportArrivals();
void listAirportDepartures();
void sortFlights(int flights[], int flight_count);
void setDate();

/*date.c*/
void printDate();
Date readDate();
Date incDate(Date date);
Date incDateAux(Date date, int month_days);
/*military_time.c*/
int readTime();
void printTime();

int compareDate(Date data1, Date data2);

/*-------------------*/
/*-----Messages------*/
/*-------------------*/

#define MSG_INVALID_FLIGHT_CODE "invalid flight code\n"
