/*
	Rafael Girão - ist199309
	2nd IAED Project 2021/2022
*/

/*TODO:
 * - run `indent` according to guidelines.pdf
 * - Run `lizard`, check for errors
 * - Comments
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "proj.h"

/*-------------------*/
/*-Global Variables--*/
/*-------------------*/

/*Init system date.*/
Date system_date = {1, 1, 2022};

/*Airports are stored here. Airport order is never changed.*/
Airport airports[MAX_AIRPORTS];
int airport_count=0;

/*Stores the indexes of the corresponding airport in airports.*/
/*Indexes are sorted by flight code in sortAirports.*/
int ordered_airport_store[MAX_AIRPORTS];

/*Flights are stored here. Flight order is never changed.*/
Flight flight_store[MAX_FLIGHTS];
int flight_count=0;

/*-------------------*/
/*-----Functions-----*/
/*-------------------*/
int readWord(char *str) {
    char c = getchar();
    int i = 0;
    while (c == ' ' || c == '\t')
        c = getchar();
    while (c != ' ' && c != '\t' && c != '\n') {
        str[i++] = c;
        c = getchar();
    }
    str[i] = '\0';
    return (c == '\n');
}

int main() {
	char command;
	while ((command = getchar()) != 'q') {
		switch (command) {
		case 'a':
			addAirport();
			break;
		case 'l':
			listAirports();
			break;
		case 'v':
			handleVCommand();
			break;
		case 'p':
			listAirportDepartures();
			break;
		case 'c':
			listAirportArrivals();
			break;
		case 't':
			setDate();
			break;
		case 'r':
			handleRCommand();
			break;
		case 'e':
			handleECommand();
			break;
		}
	}
	return 0;
}

int checkRescode(char res_code[]) {
	char c;
	/* char res_ptr = res_code; */
	int i = 0;
	int len = strlen(res_code);
	if (len+1<10) {
		printf("%s", res_code);
		fprintf(stderr, "failed at strlen %d\n", len);
		return 0;
	}
	for (i=0; i < len ;i++) {
		c = res_code[i];
		if (!(('0' <= c && c <= '9') || ('A' <= c && c <= 'Z'))) {
			fprintf(stderr, "failed at char %c\n", c);
			return 0;
		}
	}

	return 1;
}

void handleRCommand() {
	char flight_code[LIM_FLIGHT_CODE];
	char c;
	Date date;
	readWord(flight_code);
	getchar();
	date = readDate();
	if ((c = getchar()) == '\n') {
		listReservations(flight_code, date);
		return;
	}
	addReservation(flight_code, date);
	return;
}

void listReservations(char flight_code[], Date date) {
	int flight_id;
	Flight *flight;
	flight_id = getFlight(flight_code, date);
	flight = &flight_store[flight_id];
	res_print(flight->reservations);


}
void addReservation(char flight_code[], Date date) {
	Flight *flight;
	int flight_id, res_passenger_count;
	char *res_code = malloc(LIM_INSTRUCTION);
	/* Date *res_date = malloc(sizeof(Date)); */
	Date null_date = {0,0,0};
	readWord(res_code);

	scanf("%d", &res_passenger_count);
	flight_id = getFlight(flight_code, null_date);

	flight = &flight_store[flight_id];


	if (!checkReservationInput(flight_id, flight_code, flight, res_code, date, res_passenger_count)) {
		free(res_code);
		return;
	}

	flight->reservations = res_push(flight->reservations, &date, res_code, res_passenger_count);

	/*TODO: order reservations alphabetically*/
	free(res_code);
	return;
}

/* Flight code: string composed of 2 uppercase letters and a number between 1 and 9999 */
/* Reservation code: string composed of uppercase and digits, no less than 10 chars */

int checkReservationInput(int flight_id, char flight_code[], Flight *flight, char *res_code, Date date, int res_passenger_count) {
	Date tmp_date = system_date;
	if (!checkRescode(res_code)) {

		printf(MSG_INVALID_RES_CODE);
		return 0;
	}
	if (flight_id == -1) {
		printf(MSG_INVALID_FLIGHT, flight_code);
		return 0;
	}
	if (0) { /*FIXME: Make function to try to fetch reservation code, return -1 if nonexistent*/
		printf(MSG_RES_ALREADY_EXISTS, res_code);
		return 0;
	}
	if (flight->passenger_count == flight->capacity || flight->passenger_count + res_passenger_count > flight->capacity) {
		printf(MSG_TOO_MANY_PASSENGERS);
		fprintf(stderr, "flight->passenger_count = %d, flight->capacity=%d\n", flight->passenger_count, flight->capacity);
		fprintf(stderr, "res_passenger_count = %d\n", res_passenger_count);
		return 0;
	}

	tmp_date.year++;
	/*TODO: see if we can put this in a function. checkFlightInput does the same*/
	if (compareDate(system_date, date) < 0 ||
			compareDate(date, tmp_date) < 0) {
		printf(MSG_INVALID_DATE);
		return 0;
	}
	if (res_passenger_count <= 0) {
		printf(MSG_INVALID_PASSENGER_COUNT);
		return 0;
	}
	return 1;
}

int removeReservation(char res_code[]) {
	/* FIXME */
	fprintf(stderr, "FIXME: %s\n", res_code);
	return 0;

}

int removeFlight(char flight_code[]) {
	int flight_id, i, arprt_id;
	Flight flight;
	Airport *arprt;
	Date null_date = {0,0,0};
	flight_id = getFlight(flight_code, null_date);

	/*Do loop for every flight with the same code*/
	while (flight_id != -1) {
		fprintf(stderr, "Got inside while loop, flight_id=%d\n", flight_id);
		/*Temporarily save copy to access airports*/
		flight = flight_store[flight_id];
		/*Shift all flights to the right of removed flight to the left*/
		i = flight_id;
		flight_count--;

		while (i < flight_count) {
			flight_store[i] = flight_store[i+1];
			i++;
		}

		arprt_id = getAirport(flight.departure_id);
		arprt = &airports[arprt_id];
		arprt->departure_count--;


		flight_id = getFlight(flight_code, null_date);
	}

	/*Remove matching indexes and decrement all flights that had an index to the right*/
	/* FIXME */


	return flight_id;

}


void handleECommand() {
	/* TODO: do things here */
	char *code = malloc(LIM_INSTRUCTION+1);
	scanf("%s", code);

	/*Swapping Flights shouldn't be too bad.
	 * At time of writing sizeof(Flight)=136 bytes
	 * */
	/*Assume a res code cannot be the same as a flight code*/
	if (!(removeReservation(code) || removeFlight(code))) {
		printf(MSG_NOT_FOUND);
	}
	return;

}


/*
 Add airport to system. Handles 'a' command.
 */
void addAirport() {
	Airport arprt;
	char *newline_ptr;
	int i = 0;

	arprt.departure_count = 0;
	/*Read Airport ID and country*/
	scanf("%s%s", arprt.id, arprt.country);

	/*Consume spacing*/
	getchar();
	/*Read Airport city, can be a string with spacing*/
	fgets(arprt.city, LIM_CITY_NAME, stdin);

	if ((newline_ptr = strchr(arprt.city, '\n')) != NULL) {
			*newline_ptr = '\0';
		}

	while (arprt.id[i] != '\0') {
		if (!isupper(arprt.id[i++])) {
			printf(MSG_INVALID_AIRPORT_ID);
			return;
		}
	}
	if (airport_count == MAX_AIRPORTS) {
		printf(MSG_TOO_MANY_AIRPORTS);
		return;
	}
	for (i=0; i < airport_count; i++) {
		if (!strcmp(arprt.id, airports[i].id)) {
			printf(MSG_DUPLICATE_AIRPORT);
			return;
		}
	}

	airports[airport_count] = arprt;
	printf(MSG_NEW_AIRPORT, airports[airport_count].id);
	ordered_airport_store[airport_count] = airport_count;
	sortAirports();
	airport_count++;
	return;
}


/*
* Handles 'l' command.
* Lists airports sorted by alphabetical order of their id if none is specified.
 * */
void listAirports() {
	int i, found_airport;
	char c;
	char arprt_id[LIM_AIRPORT_ID];
	Airport *arprt;
	/*Case of command without arguments*/
	if ((c = getchar()) == '\n') {
		for(i=0;i < airport_count; i++) {
			arprt = &airports[ordered_airport_store[i]];
			printf("%s %s %s %d\n", arprt->id, arprt->city,
									arprt->country, arprt->departure_count);
		}
		return;
	}
	else {
		do {
			found_airport = 0;
			scanf("%s", arprt_id);
			for(i=0; i < airport_count; i++) {
				arprt = &airports[i];
				if (!strcmp(arprt_id, arprt->id)) {
					printf("%s %s %s %d\n", arprt->id, arprt->city,
											arprt->country, arprt->departure_count);
					found_airport = 1;
				}
			}
			if (!found_airport) {
				printf(MSG_NO_SUCH_AIRPORT_ID, arprt_id);
				/*return;*/
			}
		} while ((c = getchar()) != '\n');

	}
	return;
}

/*
 * Returns the index of the airport in the airport by it's ID.
 * Returns -1 if it does not exist.
*/
int getAirport(char arprt_id[]) {
	int i;
	Airport *arprt;
	for (i=0; i < airport_count; i++) {
		arprt = &airports[i];
		if (!strcmp(arprt_id, arprt->id)) {
			return i;
		}
	}
	return -1;
}

int getFlight(char flight_code[], Date date) {
	int i;
	Flight *flight;
	Date null_date = {0,0,0};
	for (i = 0;i < flight_count; i++) {
		flight = &flight_store[i];
		/*If date is 0,0,0 then we intentionally ignore it*/
		if (!strcmp(flight_code, flight->code) &&
				(compareDate(date, flight->departure_date) == 0 || compareDate(null_date, date) == 0) ) {
		/* if (!strcmp(flight_code, flight->code)) { */
			/* fprintf(stderr, "got flight, %d\n", date.day); */
			return i;
		}
	}
	return -1;
}
/*
 * Sorts the Airport store by airport code (alphabetically) with bubble sort.
*/
void sortAirports() {
	int arprt_tmp_i;
	Airport *arprt_a, *arprt_b;
	int left, right, i, j;
	left = 0;
	right = airport_count;
	for (i = left; i < right; i++) {
		for (j = left; j < right + (left - i); j++) {
/*			ordered_airport_store[j]
 *				is the index in the store of the actual airport  */
			arprt_a = &airports[ordered_airport_store[j]];
			arprt_b = &airports[ordered_airport_store[j+1]];
			if (strcmp(arprt_a->id, arprt_b->id) > 0) {
				arprt_tmp_i = ordered_airport_store[j];
				ordered_airport_store[j] = ordered_airport_store[j+1];
				ordered_airport_store[j+1] = arprt_tmp_i;
			}
		}
	}
	return;
}


/*
 * Handles 'v' command.
 * If no input is provided, list all flights.
 * Else, add a new flight.
 * */
void handleVCommand() {
	char c;
	if ((c = getchar()) == '\n') {
		listFlights();
		return;
	}
	addFlight();
	return;
}

/*
 * Adds a new flight to the system and corresponding airport.
 * Part of 'v' command.
 * */
void addFlight() {
	Flight flight;
	int arprt_departure_i;
	Airport *arprt_departure;

	/*Get command input.*/
	scanf("%s%s%s", flight.code, flight.departure_id, flight.arrival_id);
	flight.departure_date = readDate();
	getchar();
	flight.departure_time = readTime();
	getchar();
	flight.duration = readTime();
	scanf("%d", &flight.capacity);

	/*Sanity checks.*/
	if(!checkFlightInput(flight)) {
		return;
	}

	/*Calculate arrival date and time.*/
	/*Check if flight arrives on the next day.*/
	if (flight.departure_time.hour + flight.duration.hour >= 24 ||
		(flight.departure_time.hour + flight.duration.hour >= 23 &&
		 flight.departure_time.minute + flight.duration.minute >= 60)) {
		/*Arrival date is only different if flight arrives
		 * on next day's morning (max duration = 12h)*/
		flight.arrival_date = incDate(flight.departure_date);
	}
	else {
		flight.arrival_date = flight.departure_date;
	}
	flight.arrival_time = addTime(flight.departure_time, flight.duration);
	flight.passenger_count = 0;

	arprt_departure_i = getAirport(flight.departure_id);
	arprt_departure = &airports[arprt_departure_i];
	arprt_departure->departure_count++;

	flight.reservations = NULL;
	flight_store[flight_count++] = flight;
	return;
}

/*
 Checks input feeded into addFlight() for sanity checks.
*/
int checkFlightInput(Flight flight) {
	int i, arprt_arrival_i, arprt_departure_i;
	Flight flight_i;
	Date tmp_date = system_date;
	for(i = 0; i < LIM_FLIGHT_CODE && flight.code[i] != '\0'; i++) {
		if(i < 2 && !isupper(flight.code[i])) {
			printf(MSG_INVALID_FLIGHT_CODE);
			return 0;
		}
		else if (i == 2 && flight.code[i] == '0') {
			printf(MSG_INVALID_FLIGHT_CODE);
			return 0;
		}
		else if (i > 2 && !('0' <= flight.code[i] && flight.code[i] <= '9')) {
				printf(MSG_INVALID_FLIGHT_CODE);
				return 0;
			}
	}

	for(i=0;i<flight_count;i++) {
		flight_i = flight_store[i];
		if(!strcmp(flight_i.code, flight.code) &&
			compareDate(flight.departure_date, flight_i.departure_date) == 0) {
				printf(MSG_FLIGHT_ALREADY_EXISTS);
				return 0;
		}
	}

	arprt_arrival_i = getAirport(flight.arrival_id);
	arprt_departure_i = getAirport(flight.departure_id);
	if (arprt_departure_i == -1) {
		printf(MSG_NO_SUCH_AIRPORT_ID, flight.departure_id);
		return 0;
	}
	if (arprt_arrival_i == -1) {
		printf(MSG_NO_SUCH_AIRPORT_ID, flight.arrival_id);
		return 0;
	}
	if (flight_count == MAX_FLIGHTS) {
		printf(MSG_TOO_MANY_FLIGHTS);
		return 0;
	}
	/*tmp_date is a Date one year into the future*/
	/*if departure date is in the future
	 *relatively to tmp_date (compareDate(...) < 0) , then it's invalid*/
	tmp_date.year++;
	if (compareDate(system_date, flight.departure_date) < 0 ||
			compareDate(flight.departure_date, tmp_date) < 0)  {
		printf(MSG_INVALID_DATE);
		return 0;
	}
	if((flight.duration.hour > 12) ||
			(flight.duration.hour == 12 && flight.duration.minute > 0)) {
		printf(MSG_INVALID_DURATION);
		return 0;
	}

	/*Max flight capacity was removed on second project*/
	if(!(MIN_FLIGHT_CAP <= flight.capacity)) {
		printf(MSG_INVALID_CAPACITY);
		return 0;
	}
	return 1;

}

/*
 Lists all flights, by creation order.
 Part of 'v' command.
 */
void listFlights() {
	int i;
	Flight *flight;
	for(i=0; i < flight_count; i++) {
		flight = &flight_store[i];
		printf("%s %s %s ", flight->code, flight->departure_id, flight->arrival_id);
		printDate(flight->departure_date);
		printf(" ");
		printTime(flight->departure_time);
		printf("\n");
	}

}

/*
 * Handles 'p' command.
 * Lists an airport's departures by ascending date.
 * */
void listAirportDepartures() {
	char arprt_id[LIM_AIRPORT_ID];
	int i, arprt_i, n;
	int departures[MAX_FLIGHTS];
	/* Airport *arprt; */
	Flight *flight;
	scanf("%s", arprt_id);
	arprt_i = getAirport(arprt_id);
	if (arprt_i == -1) {
		printf(MSG_NO_SUCH_AIRPORT_ID, arprt_id);
		return;
	}
	n = 0;
	for (i=0;i<flight_count;i++) {
		flight = &flight_store[i];
		if (!strcmp(flight->departure_id, arprt_id)) {
			departures[n++] = i;
		}
	}
	bubbleSortFlights(departures, n, 1);

	for (i = 0 ; i < n; i++) {
		flight = &flight_store[departures[i]];
		printf("%s %s ", flight->code, flight->arrival_id);
		printDate(flight->departure_date);
		printf(" ");
		printTime(flight->departure_time);
		printf("\n");
	}
}


/*
 * Handles 'c' command.
 * Lists an airport's arrivals by ascending date.
 * */
/*TODO: if time, merge listAirportArrivals and listAirportDepartures*/
void listAirportArrivals() {
	char arprt_id[LIM_AIRPORT_ID];
	int i, arprt_i, n;
	int arrivals[MAX_FLIGHTS];
	Flight *flight;
	scanf("%s", arprt_id);
	arprt_i = getAirport(arprt_id);
	if (arprt_i == -1) {
		printf(MSG_NO_SUCH_AIRPORT_ID, arprt_id);
		return;
	}
	n = 0;
	for (i=0;i<flight_count;i++) {
		flight = &flight_store[i];
		if (!strcmp(arprt_id, flight->arrival_id)) {
			arrivals[n++] = i;
		}
	}
	bubbleSortFlights(arrivals, n, 0);

	for (i = 0 ; i < n; i++) {
		flight = &flight_store[arrivals[i]];
		printf("%s %s ", flight->code, flight->departure_id);
		printDate(flight->arrival_date);
		printf(" ");
		printTime(flight->arrival_time);
		printf("\n");
	}
}

/*
 Use bubble sort to sort the Flights façade.
 departures should be passed as 1 if we're sorting
 an airport's departures façade, 0 otherwise (sorting arrivals).
*/
void bubbleSortFlights(int flights[], int flight_count, int departures) {
	int flight_tmp_i;
	int left, right, i, j;
	int done;
	Flight *flight_a, *flight_b;
	left = 0;
	right = flight_count;
	for (i = 1; i < flight_count; i++) {
		done = 1;
		for (j = left; j < right - 1; j++) {
			flight_a = &flight_store[flights[j]];
			flight_b = &flight_store[flights[j+1]];
			if(compareFlights(flight_a, flight_b, departures) == -1) {
				done = 0;
				flight_tmp_i = flights[j];
				flights[j] = flights[j+1];
				flights[j+1] = flight_tmp_i;
			}
		}
		if (done) {
			return;
		}
		--right;
	}
	return;
}

/*
 * Compares two flights by their date and time.
 * Returns -1 if flight_a is in future relative to flight_b, 1 otherwise.
*/
int compareFlights(Flight *flight_a, Flight *flight_b, int departures) {
	int date_comparison, time_comparison;
	if (departures) {
		date_comparison = compareDate(flight_a->departure_date,
										flight_b->departure_date);
		time_comparison = compareTime(flight_a->departure_time,
										flight_b->departure_time);
	}
	else {
		date_comparison = compareDate(flight_a->arrival_date,
										flight_b->arrival_date);
		time_comparison = compareTime(flight_a->arrival_time,
										flight_b->arrival_time);
	}

	if (date_comparison < 0 || (date_comparison == 0 && time_comparison < 0)) {
		return -1;
	}
	return 1;

}
