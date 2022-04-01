/*
	Rafael Girão - ist199309
	1st IAED Project 2021/2022
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

Date system_date = {1, 1, 2022};


/*FIXME comment global vars*/

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
		}
	}
	return 0;
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

	/*Consume space*/
	getchar();
	/*Read Airport city, can be a string with spaces*/
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
			printf("%s %s %s %d\n", arprt.id, arprt.city, arprt.country, flight_count);
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
					printf("%s %s %s %d\n", arprt.id, arprt.city, arprt.country, flight_count);
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


void sortAirports() {
	/*
	 Use bubble sort to sort the Airport façade
	 */
	int arprt_tmp_i;
	Airport arprt_a, arprt_b;
	int left, right, i, j;
	left = 0;
	right = airport_count;
	for (i = left; i < right; i++) {
		for (j = left; j < right + (left - i); j++) {
/*			ordered_airport_store[j] = index in the store of the actual airport  */
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


int compareFlights(Flight *flight_a, Flight *flight_b, int departures) {
	int date_comparison, time_comparison;
	if (departures) {
		date_comparison = compareDate(flight_a->departure_date, flight_b->departure_date);
		time_comparison = compareTime(flight_a->departure_time, flight_b->departure_time);
	}
	else {
		date_comparison = compareDate(flight_a->arrival_date, flight_b->arrival_date);
		time_comparison = compareTime(flight_a->arrival_time, flight_b->arrival_time);
	}

	if (date_comparison < 0 || (date_comparison == 0 && time_comparison < 0)) {
		return -1;
	}
	return 1;

}
/*
 departures should be passed as 1 if we're sorting an airport's departures façade, 0 otherwise (sorting arrivals).
*/
void bubbleSortFlights(int flights[], int flight_count, int departures) {
	/*
	 Use bubble sort to sort the Flights façade
	 */
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

void handleVCommand() {
	char c;
	if ((c = getchar()) == '\n') {
		listFlights();
		return;
	}
	addFlight();

	return;
}


void addFlight() {
	int i;
	Flight flight, flight_i;
	int arprt_arrival_i, arprt_departure_i;
	Airport *arprt_arrival, *arprt_departure;
	Date tmp_date = system_date;

	scanf("%s%s%s", flight.code, flight.departure_id, flight.arrival_id);
	flight.departure_date = readDate();
	getchar();
	flight.departure_time = readTime();
	getchar();
	flight.duration = readTime();
	scanf("%d", &flight.capacity);

	for(i = 0; i < LIM_FLIGHT_CODE && flight.code[i] != '\0'; i++) {
		if(i < 2) {
			if(!isupper(flight.code[i])) {
				printf(MSG_INVALID_FLIGHT_CODE);
				return;
			}
		}
		else {
			if(!('0' <= flight.code[i] && flight.code[i] <= '9'))
			{
				printf(MSG_INVALID_FLIGHT_CODE);
				return;
			}
		}
	}
	for(i=0;i<flight_count;i++) {
		flight_i = flight_store[i];
		if(!strcmp(flight_i.code, flight.code)) {
			if((compareDate(flight.departure_date, flight_i.departure_date) == 0)) {
				printf(MSG_FLIGHT_ALREADY_EXISTS);
				return;
			}
		}
	}

	arprt_arrival_i = getAirport(flight.arrival_id);
	arprt_departure_i = getAirport(flight.departure_id);
	if (arprt_departure_i == -1) {
		printf(MSG_NO_SUCH_AIRPORT_ID, flight.departure_id);
		return;
	}
	if (arprt_arrival_i == -1) {
		printf(MSG_NO_SUCH_AIRPORT_ID, flight.arrival_id);
		return;
	}
	if (flight_count == MAX_FLIGHTS) {
		printf(MSG_TOO_MANY_FLIGHTS);
		return;
	}
	/*tmp_date is a Date one year into the future*/
	/*if departure date is in the future relative to tmp_date (compareDate(...) < 0) , then it's invalid*/
	tmp_date.year++;
	if (compareDate(system_date, flight.departure_date) < 0 || compareDate(flight.departure_date, tmp_date) < 0)  {
		printf(MSG_INVALID_DATE);
		return;
	}
	if((flight.duration.hour > 12) || (flight.duration.hour == 12 && flight.duration.minute > 0)) {
		printf(MSG_INVALID_DURATION);
		return;
	}
	if(!(MIN_FLIGHT_CAP <= flight.capacity && flight.capacity <= MAX_FLIGHT_CAP)) {
		printf(MSG_INVALID_CAPACITY);
		return;
	}

	/*Calculate arrival date and time*/
	/*Check if flight arrives on the next day*/
	if (flight.departure_time.hour + flight.duration.hour >= 24 || (flight.departure_time.hour + flight.duration.hour >= 23 && flight.departure_time.minute + flight.duration.minute > 60)) {
		/*Arrival date is only different if flight arrives on next day's morning (max duration = 12h)*/
		flight.arrival_date = incDate(flight.departure_date);
	}
	else {
		flight.arrival_date = flight.departure_date;
	}
	flight.arrival_time = addTime(flight.departure_time, flight.duration);

	/*Store flight*/
	arprt_arrival = &airports[arprt_arrival_i];
	arprt_arrival->arrivals[arprt_arrival->arrival_count++] = flight_count;
	arprt_departure = &airports[arprt_departure_i];
	arprt_departure->departures[arprt_departure->departure_count++] = flight_count;

	/*	Mark arrays as dirty for later sorting*/
	arprt_arrival->arrival_dirty = 1;
	arprt_departure->departure_dirty = 1;

	/*Sort flight arrays*/
	/*
	sortFlights(arprt_arrival->arrivals, arprt_arrival->arrival_count, 0);
	sortFlights(arprt_departure->departures, arprt_departure->departure_count, 1);
	*/

	flight_store[flight_count++] = flight;
	return;
}


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

void listAirportDepartures() {
	/*
	 * TODO: 'Os voos são ordenados da data e hora mais antigas para a mais recente.'
	 */
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

	for (i=0 ; i < arprt.arrival_count; i++) {
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
	Functions for handling military time
*/

mTime readTime() {
	/*int time;*/
	/*Time to be read is of the format: 'HH:MM'*/

	char raw_minute[LIM_RAW_TIME], raw_hour[LIM_RAW_TIME];
	mTime time;

	raw_hour[0] = getchar();
	raw_hour[1] = getchar();
	raw_hour[2] = '\0';
	getchar(); /*discard ':' char*/
	raw_minute[0] = getchar();
	raw_minute[1] = getchar();
	raw_minute[2] = '\0';
	time.hour = atoi(raw_hour);
	time.minute = atoi(raw_minute);

	return time;
}

void printTime(mTime time) {
	/*handle hour*/
	if (time.hour < 10) {
		printf("0%d", time.hour);
	}
	else {
		printf("%d", time.hour);
	}
	printf(":");
	/*handle minutes*/
	if (time.minute < 10) {
		printf("0%d", time.minute);
	}
	else {
		printf("%d", time.minute);
	}
	return;
}

/*
	Returns 1 if time2 is in the future relatively to time1
	(time2 - time1 > 0)
	0 if times are equal
	-1 if time2 is in past relative to time1
*/
int compareTime(mTime time1, mTime time2) {
	/*Compare hour*/
	if (time2.hour - time1.hour > 0) {
		return 1;
	}
	else if (time2.hour - time1.hour < 0) {
		return -1;
	}
	/*Compare minute*/
	if (time2.minute - time1.minute > 0) {
		return 1;
	}
	else if (time2.minute - time1.minute < 0) {
		return -1;
	}
	/*Times are equal */
	return 0;
}

mTime addTime(mTime time1, mTime time2) {
	mTime time = {0, 0};
	/*Sum minutes*/
	time.minute = time1.minute + time2.minute;
	if (time.minute >= 60) {
		time.minute -= 60;
		time.hour++;
	}
	time.hour += time1.hour + time2.hour;
	if (time.hour >= 24) {
		time.hour -= 24;
	}
	return time;
}

/*
	Functions for handling Date struct
*/
Date readDate() {
	/*Getchar in between fgets to consume dash character */
	Date newdate;
/*	char rawdate[LIM_DATE];*/
	char rawday[2+1], rawmonth[2+1], rawyear[4+1];
	getchar();
	rawday[0] = getchar();
	rawday[1] = getchar();
	rawday[2] = '\0';
	getchar();
	rawmonth[0] = getchar();
	rawmonth[1] = getchar();
	rawmonth[2] = '\0';
	getchar();
	rawyear[0] = getchar();
	rawyear[1] = getchar();
	rawyear[2] = getchar();
	rawyear[3] = getchar();
	rawyear[4] = '\0';

	newdate.day = atoi(rawday);
	newdate.month = atoi(rawmonth);
	newdate.year = atoi(rawyear);
	return newdate;
}
void printDate(Date date) {
	if (date.day < 10)
		printf("0%d-", date.day);
	else
		printf("%d-", date.day);
	if (date.month < 10)
		printf("0%d-", date.month);
	else
		printf("%d-", date.month);
	printf("%d", date.year);
}

void setDate() {
	Date date = readDate();
	if (date.day <= 0 || date.month <= 0 || date.year <= 0 || compareDate(system_date, date) < 0) {
		printf("invalid date\n");
		return;
	}
	system_date.year = date.year;
	system_date.month = date.month;
	system_date.day = date.day;

	printDate(system_date);
	printf("\n");
	return;
}

/*
	Returns 1 if date2 is in the future relatively to date1
	(date2 - date1 > 0)
	0 if both dates are equal
	-1 if date2 is in past relative to date1
*/
int compareDate(Date date1, Date date2) {
	/*Compare year*/
	if (date2.year - date1.year > 0) {
		return 1;
	}
	else if (date2.year - date1.year < 0) {
		return -1;
	}
	/*Compare month*/
	if (date2.month - date1.month > 0) {
		return 1;
	}
	else if (date2.month - date1.month < 0) {
		return -1;
	}
	/*Compare day*/
	if (date2.day - date1.day > 0) {
		return 1;
	}
	else if (date2.day - date1.day < 0) {
		return -1;
	}
	/*Dates are the same*/
	return 0;
}

Date incDate(Date date) {
	switch (date.month) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			date = incDateAux(date, 31);
			break;
		case 2:
			date = incDateAux(date, 28);
			break;
		default:
			date = incDateAux(date, 30);
			break;
	}
	return date;
}

Date incDateAux(Date date, int month_days) {
	if (date.day < month_days) {
		date.day++;
	}
	else {
		date.day = 1;
		if (date.month < 12) {
			date.month++;
		}
		else {
			date.month = 1;
			date.year++;
		}
	}
	return date;
}
