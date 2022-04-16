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

int readResCode(char res_code[]) {
	char c;
/*	char res_code[LIM_INSTRUCTION];*/
/*	char *res_code = (char *) malloc(sizeof(char) * LIM_INSTRUCTION);*/
	/* char res_ptr = res_code; */
	int i = 0;
	do {
		c = getchar();
		if (('0' <= c && c <='9') || ('A' <= c && 'Z' <= c)) {
			/* TODO: handle NULL pointer later */
			return 0;
		}
		else {
			res_code[i++] = c;
		}

	} while (c != '\0' && c != '\n');

	if (i < 10) {
		return 0;
	}

	/* TODO: actually make this pointer point to res_code */
	return 1;
}

void handleRCommand() {
	Flight *flight;
	int flight_id;
	char flight_code[LIM_FLIGHT_CODE];
/*	char res_code[LIM_INSTRUCTION];*/
	char *res_code = (char *) malloc(sizeof(char) * LIM_INSTRUCTION);
	Reservation *res;

	res = (Reservation *) malloc(sizeof(Reservation));
	scanf("%s", flight_code);
	flight_id = getFlight(flight_code);

	flight = &flight_store[flight_id];

	if (!checkReservationInput(res, flight_id, flight_code, flight, res_code)) {
		free(res);
		free(res_code);
		return;
	}

	/*TODO: order reservations alphabetically*/
	return;
}

int checkReservationInput(Reservation *res, int flight_id, char flight_code[], Flight *flight, char *res_code) {
	if (!readResCode(res_code)) {
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
	if (flight->passenger_count == flight->capacity || flight->passenger_count + res->passenger_count > flight->capacity) {
		printf(MSG_TOO_MANY_PASSENGERS);
		return 0;
	}

	if (/*Check if date is valid here*/0) {
		printf(MSG_INVALID_DATE);
		return 0;
	}
	if (res->passenger_count <= 0) {
		printf(MSG_INVALID_PASSENGER_COUNT);
		return 0;
	}
	return 1;
}

void handleECommand() {
	/* TODO: do things here */
}
/*
 Add airport to system. Handles 'a' command.
 */
void addAirport() {
	Airport arprt;
	char *newline_ptr;
	int i = 0;
	arprt.arrival_count = 0;
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
	Airport arprt;
	/*Case of command without arguments*/
	if ((c = getchar()) == '\n') {
		for(i=0;i < airport_count; i++) {
			arprt = airports[ordered_airport_store[i]];
			flight_count = arprt.departure_count;
			printf("%s %s %s %d\n", arprt.id, arprt.city,
									arprt.country, flight_count);
		}
		return;
	}
	else {
		do {
			found_airport = 0;
			scanf("%s", arprt_id);
			for(i=0; i < airport_count; i++) {
				arprt = airports[i];
				if (!strcmp(arprt_id, arprt.id)) {
					flight_count = arprt.departure_count;
					printf("%s %s %s %d\n", arprt.id, arprt.city,
											arprt.country, flight_count);
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

int getFlight(char flight_code[]) {
	int i;
	Flight *flight;
	for (i = 0;i < flight_count; i++) {
		flight = &flight_store[i];
		if (!strcmp(flight_code, flight->code)) {
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
	Airport arprt_a, arprt_b;
	int left, right, i, j;
	left = 0;
	right = airport_count;
	for (i = left; i < right; i++) {
		for (j = left; j < right + (left - i); j++) {
/*			ordered_airport_store[j]
 *				is the index in the store of the actual airport  */
			arprt_a = airports[ordered_airport_store[j]];
			arprt_b = airports[ordered_airport_store[j+1]];
			if (strcmp(arprt_a.id, arprt_b.id) > 0) {
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
	int arprt_arrival_i, arprt_departure_i;
	Airport *arprt_arrival, *arprt_departure;

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

	arprt_arrival_i = getAirport(flight.arrival_id);
	arprt_departure_i = getAirport(flight.departure_id);

	/*Store flight.*/
	arprt_arrival = &airports[arprt_arrival_i];
	arprt_arrival->arrivals[arprt_arrival->arrival_count++] = flight_count;
	arprt_departure = &airports[arprt_departure_i];
	arprt_departure->departures[arprt_departure->departure_count++] = flight_count;

	/*	Mark arrays as dirty for later sorting.*/
	arprt_arrival->arrival_dirty = 1;
	arprt_departure->departure_dirty = 1;

	/*Init reservations.*/
	flight.reservations = NULL;
/*	flight.reservations = (Reservation *) malloc(sizeof(Reservation));*/
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
	Flight flight;
	for(i=0; i < flight_count; i++) {
		flight = flight_store[i];
		printf("%s %s %s ", flight.code, flight.departure_id, flight.arrival_id);
		printDate(flight.departure_date);
		printf(" ");
		printTime(flight.departure_time);
		printf("\n");
	}

}

/*
 * Handles 'p' command.
 * Lists an airport's departures by ascending date.
 * */
void listAirportDepartures() {
	char arprt_id[LIM_AIRPORT_ID];
	int i, arprt_i;
	Airport arprt;
	Flight flight;

	scanf("%s", arprt_id);
	arprt_i = getAirport(arprt_id);
	if (arprt_i == -1) {
		printf(MSG_NO_SUCH_AIRPORT_ID, arprt_id);
		return;
	}

	/*airports[arprt_i] is safeguarded by check_airport_existence*/
	arprt = airports[arprt_i];
	/*Sort airport's departure array if dirty*/
	if (arprt.departure_dirty) {
/*		mergeSortFlights(arprt.departures, 0, arprt.departure_count, 1);*/
		bubbleSortFlights(arprt.departures, arprt.departure_count, 1);
		arprt.departure_dirty = 0;
		airports[arprt_i] = arprt;
	}
	for (i = 0 ; i < arprt.departure_count; i++) {
		flight = flight_store[arprt.departures[i]];
		printf("%s %s ", flight.code, flight.arrival_id);
		printDate(flight.departure_date);
		printf(" ");
		printTime(flight.departure_time);
		printf("\n");
	}
	return;
}

/*
 * Handles 'c' command.
 * Lists an airport's arrivals by ascending date.
 * */
void listAirportArrivals() {
	char arprt_id[LIM_AIRPORT_ID];
	int i, arprt_i;
	Airport arprt;
	Flight flight;

	scanf("%s", arprt_id);
	arprt_i = getAirport(arprt_id);
	if (arprt_i == -1) {
		printf(MSG_NO_SUCH_AIRPORT_ID, arprt_id);
		return;
	}
	/*airports[arprt_i] is safeguarded by getAirport*/
	arprt = airports[arprt_i];

	/*Sort airport's arrival array if dirty*/
	if (arprt.arrival_dirty) {
		bubbleSortFlights(arprt.arrivals, arprt.arrival_count, 0);
		arprt.arrival_dirty = 0;
		airports[arprt_i] = arprt;
	}

	for (i = 0 ; i < arprt.arrival_count; i++) {
		flight = flight_store[arprt.arrivals[i]];
		printf("%s %s ", flight.code, flight.departure_id);
		printDate(flight.arrival_date);
		printf(" ");
		printTime(flight.arrival_time);
		printf("\n");

	}
	return;
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
