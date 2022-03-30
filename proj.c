/*
	Rafael Girão - ist199309
	1st IAED Project 2021/2022
*/

/*TODO:
 * - run `indent` according to guidelines.pdf
 * - Implement sorting of airports
 * - Finish c and p command
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


Airport airports[MAX_AIRPORTS];
/*Façade to simplify airport sorting*/
int ordered_airport_store[MAX_AIRPORTS];
int airport_count;
Flight flight_store[MAX_FLIGHTS];
int flight_count;

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

void addAirport() {
	Airport arprt;
	int i = 0;
	arprt.arrival_count = 0;
	arprt.departure_count = 0;

	/*Read Airport ID and country*/
	scanf("%s%s", arprt.id, arprt.country);

	/*Consume space*/
	getchar();
	/*Read Airport city, can be a string with spaces*/
	fgets(arprt.city, LIM_CITY_NAME, stdin);
	/*Remove trailing \n*/
	arprt.city[strlen(arprt.city) - 1] = '\0';

	while (arprt.id[i] != '\0') {
		if (!isupper(arprt.id[i++])) {
			printf("invalid airport ID\n");
			return;
		}
	}
	if (airport_count == MAX_AIRPORTS) {
		printf("too many airports\n");
		return;
	}
	for (i=0; i < airport_count; i++) {
		if (!strcmp(arprt.id, airports[i].id)) {
			printf("duplicate airport\n");
			return;
		}
	}


	airports[airport_count] = arprt;

	/*airports[airport_count++] = arprt;*/
	printf("airport %s\n", airports[airport_count].id);
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
/*			arprt = airports[i]; */
			arprt = airports[ordered_airport_store[i]];
			flight_count = arprt.departure_count + arprt.arrival_count;
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
					flight_count = arprt.departure_count + arprt.arrival_count;
					printf("%s %s %s %d\n", arprt.id, arprt.city, arprt.country, flight_count);
					found_airport = 1;
				}
			}
			if (!found_airport) {
				printf("%s: no such airport ID\n", arprt_id);
				/*return;*/
			}
		} while ((c = getchar()) != '\n');

	}
	return;
}

int getAirport(char arprt_id[]) {
	int i;
	Airport arprt;
	for (i=0; i < airport_count; i++) {
		arprt = airports[i];
		if (!strcmp(arprt_id, arprt.id)) {
/*			printf("DEBUG: found airport %s with id '%d'\n", arprt_id, i);*/
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

void sortFlights(int flights[], int flight_count) {
	/*
	 Use bubble sort to sort the Flights façade
	 */
	int flight_tmp_i;
	int left, right, i, j;
	int date_comparison;
	Flight flight_a, flight_b;
	left = 0;
	right = flight_count;
	for (i = left; i < right; i++) {
		for (j = left; j < right + (left - i); j++) {
/*			ordered_airport_store[j] = index in the store of the actual airport  */
			flight_a = flight_store[flights[j]];
			flight_b = flight_store[flights[j+1]];
			date_comparison = compareDate(flight_a.departure_date, flight_b.departure_date);
			if (date_comparison < 0 || (date_comparison == 0 && flight_a.departure_time > flight_b.departure_time)) {
/*			if (strcmp(arprt_a.id, arprt_b.id) > 0) {*/
				flight_tmp_i = flights[j];
				flights[j] = flights[j+1];
				flights[j+1] = flight_tmp_i;
			}
		}
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
	Airport arprt_arrival, arprt_departure;
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
				printf("flight already exists\n");
				return;
			}
		}
	}

	arprt_arrival_i = getAirport(flight.arrival_id);
	arprt_departure_i = getAirport(flight.departure_id);
	if (arprt_departure_i == -1) {
		printf("%s: no such airport ID\n", flight.departure_id);
		return;
	}
	if (arprt_arrival_i == -1) {
		printf("%s: no such airport ID\n", flight.arrival_id);
		return;
	}
	if (flight_count == MAX_FLIGHTS) {
		printf("too many flights\n");
		return;
	}
	/*tmp_date is a Date one year into the future*/
	/*if departure date is in the future relative to tmp_date (compareDate(...) < 0) , then it's invalid*/
	tmp_date.year++;
/*	tmp_date = incDate(tmp_date);*/
	/*TODO: check if date in past or more than year in future*/
	if (compareDate(system_date, flight.departure_date) < 0 || compareDate(flight.departure_date, tmp_date) < 0)  {
		printf("invalid date\n");
		return;
	}
	if(flight.duration > 1200) {
		printf("invalid duration\n");
		return;
	}
	if(!(MIN_FLIGHT_CAP <= flight.capacity && flight.capacity <= MAX_FLIGHT_CAP)) {
		printf("invalid capacity\n");
		return;
	}
	/*Store flight*/
	arprt_arrival = airports[arprt_arrival_i];
	arprt_arrival.arrivals[arprt_arrival.arrival_count++] = flight_count;
	arprt_departure = airports[arprt_departure_i];
	arprt_departure.departures[arprt_departure.departure_count++] = flight_count;

	/*Sort flight arrays*/
	sortFlights(arprt_arrival.arrivals, arprt_arrival.arrival_count);
	sortFlights(arprt_departure.departures, arprt_departure.departure_count);

	flight_store[flight_count++] = flight;

	/*Store updated airports*/
	airports[arprt_departure_i] = arprt_departure;
	airports[arprt_arrival_i] = arprt_arrival;

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
		printf("%s: no such airport ID\n", arprt_id);
		return;
	}
	/*airports[arprt_i] is safeguarded by check_airport_existence*/
	arprt = airports[arprt_i];
/*	printf("'%s'AAAAAA\n", arprt.id);*/
/*	printf("%s", arprt.id);*/
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
	/*
	 * TODO: 'Os voos são ordenados da data e hora mais antigas para a mais recente.'
	 */
	char arprt_id[LIM_AIRPORT_ID];
	int i, arprt_i;
	Airport arprt;
	Flight flight;
	/*Temporary values in case they need to be "incremented"*/
	Date tmp_date;

	scanf("%s", arprt_id);
	arprt_i = getAirport(arprt_id);
	if (arprt_i == -1) {
		printf("%s: no such airport ID\n", arprt_id);
		return;
	}
	/*airports[arprt_i] is safeguarded by check_airport_existence*/
	arprt = airports[arprt_i];
	for (i=0 ; i < arprt.arrival_count; i++) {
		flight = flight_store[arprt.arrivals[i]];
		printf("%s %s ", flight.code, flight.departure_id);
		/*Check if flight arrives on the next day*/
		if (flight.departure_time + flight.duration >= 2400) {
			/*Arrival date is only different if flight arrives on next day's morning (max duration = 12h)*/
			tmp_date = incDate(flight.departure_date);
			printDate(tmp_date);
			printf(" ");
			printTime(flight.departure_time + flight.duration - 2400);
		}
		else {
			printDate(flight.departure_date);
			printf(" ");
			printTime(flight.departure_time + flight.duration);
		}
		printf("\n");
	}
	return;
}

/*
	Functions for handling military time
*/

int readTime() {
	/*int time;*/
	/*Time to be read is of the format: 'HH:MM'*/

	char raw_time[LIM_RAW_TIME];

	raw_time[0] = getchar();
	raw_time[1] = getchar();
	getchar(); /*discard ':' char*/
	raw_time[2] = getchar();
	raw_time[3] = getchar();
	raw_time[4] = '\0';
	raw_time[LIM_RAW_TIME-1] = '\0';
	return atoi(raw_time);
}

void printTime(int time) {
	int hour, minutes;
	hour = time / 100;
	minutes = time % 100;
	/*handle hour*/
	if (hour < 10) {
		printf("0%d", hour);
	}
	else {
		printf("%d", hour);
	}
	printf(":");
	/*handle minutes*/
	if (minutes < 10) {
		printf("0%d", minutes);
	}
	else {
		printf("%d", minutes);
	}
	return;
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
/*
	printf("rawday: %s\nrawmonth: %s\nrawyear: %s\n", rawday, rawmonth, rawyear);
*/
	return newdate;
}
void printDate(Date date) {
/*	printf("%s-%d-%d", day, date.month, date.year); */
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
	/*date.year += ano; - TODO: make sure this is irrelevant because max data <= 1 yr*/
	system_date.year = date.year;
	system_date.month = date.month;
	system_date.day = date.day;

	printDate(system_date);
	printf("\n");
/*	printf("%d-%d-%d\n", system_date.day, system_date.month, system_date.year);*/
	return;
}

/*
	Returns 1 if date2 is in the future relatively to date1
	(date2 - date1 > 0)
	0 if both dates are equal
	-1 if date2 is in past relative to date1
*/
/*FIXME: I'm not confident this works as intended*/
int compareDate(Date date1, Date date2) {
	/*Comparar ano*/
	if (date2.year - date1.year > 0) {
		return 1;
	}
	else if (date2.year - date1.year < 0) {
		return -1;
	}
	/*Comparar mês*/
	if (date2.month - date1.month > 0) {
		return 1;
	}
	else if (date2.month - date1.month < 0) {
		return -1;
	}
	/*Comparar dia*/
	/*if (date1.day> date2.day) {*/
	if (date2.day - date1.day > 0) {
		return 1;
	}
	else if(date2.day - date1.day < 0) {
		return -1;
	}
	/*Datas são iguais */
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
			date = incDateAux(date, 29);
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
