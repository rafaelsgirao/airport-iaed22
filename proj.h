#define LIM_AIRPORT_ID 30+1
#define LIM_COUNTRY_NAME 30+1
#define LIM_CITY_NAME 50+1
#define LIM_FLIGHT_CODE 6+1
#define LIM_DATE 10+1
#define LIM_RAW_TIME 5+1
#define MAX_AIRPORTS 40
#define MAX_FLIGHTS 30000


/*-------------------*/
/*-----Structures----*/
/*-------------------*/
typedef struct {
	int day;
	int month;
	int year;
	int hour;
	int minute;
} Date;

typedef struct {
	char id[LIM_AIRPORT_ID];
	char country[LIM_COUNTRY_NAME];
	char city[LIM_CITY_NAME];
	int flight_count;
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

/*-------------------*/
/*----Prototypes-----*/
/*-------------------*/
void addAirport();
void listAirports();
void addFlight();
void handleVCommand();
void listFlights();
void listAirportFlights();
void advanceDate();
Date getDate();


int compareDate(Date data1, Date data2);
