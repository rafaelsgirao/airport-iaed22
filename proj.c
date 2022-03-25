/*
	Rafael Girão - ist199309
	1st IAED Project 2021/2022
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "proj.h"

/*-------------------*/
/*-Global Variables--*/
/*-------------------*/

Date system_date = {1, 1, 2022, 0, 0};


Airport airports[MAX_AIRPORTS];
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
			addFlight();
			break;
		case 'p':
			listAirportFlights();
			break;
		case 't':
			advanceDate();
		}
	}
	return 0;
}


void addAirport() {
	Airport arprt;
	int i = 0;
	arprt.flight_count = 0;
	
	/*Read Airport ID and country*/
	scanf("%s%s", arprt.id, arprt.country);
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

	/*Consume space*/
	getchar();
	/*Read Airport city, can be a string with spaces*/
	fgets(arprt.city, LIM_CITY_NAME, stdin);
	/*Remove trailing \n*/
	arprt.city[strlen(arprt.city) - 1] = '\0';
	/*TODO: store the new airport, ordered alphabetically*/

	/*Store values*/
/*	strcpy(airports[airport_count].id, arprt.id);
	strcpy(airports[airport_count].country, arprt.country);
	strcpy(airports[airport_count].city, arprt.city);
	airports[airport_count].flight_count = 0;*/
	airports[airport_count] = arprt;

	/*airports[airport_count++] = arprt;*/
	printf("airport %s\n", airports[airport_count].id);

	airport_count++;
	return;
}


void listAirports() {
	int i, found_airport;
	char c;
	char arprt_id[LIM_AIRPORT_ID];
	Airport arprt;
	/*Airport arprt;*/
	/*Case of command without arguments*/
	if ((c = getchar()) == '\n') {
		for(i=0;i < airport_count; i++) {
			arprt = airports[i];
			/*TODO: implement storing flight count for each airport*/
			printf("%s %s %s %d\n", arprt.id, arprt.city, arprt.country, arprt.flight_count);
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
					printf("%s %s %s %d\n", arprt.id, arprt.city, arprt.country, arprt.flight_count);
					found_airport = 1;
				}
			}
			if (!found_airport) {
				printf("%s: no such airport ID\n", arprt_id);
				return;
			}
		} while ((c = getchar()) != '\n');
		
	}

	/*TODO: implement*/
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

int check_airport_existence(char arprt_id[]) {
	int i;
	Airport arprt;
	for (i=0;i<airport_count;i++) {
		arprt = airports[i];
		if (!strcmp(arprt_id, arprt.id)) {
			return 1;
		}
	}
	return 0;
}


void addFlight() {
	int i;
	char flight_code[LIM_FLIGHT_CODE];
	Flight flight;
	char raw_departure_time[LIM_RAW_TIME], raw_duration_time[LIM_RAW_TIME];


	scanf("%s%s%s", flight.code, flight.departure_id, flight.arrival_id);
	flight.departure_date = getDate();
	/*FIXME: process raw_departure_time*/
	scanf("%s%s%d", raw_departure_time, raw_duration_time, &flight.capacity);

	for(i=0; i < LIM_FLIGHT_CODE; i++) {
		if(i < 2) {
			if(!isupper(flight.code[i])) {
				printf("invalid flight code\n");
			}
		}
		else {
			if(!('0' <= flight.code[i] && flight.code[i] <= '9'))
			{
				printf("invalid flight code\n");
			}
		}
	}
	
	/*TODO: implement*/
	for(i=0;i<flight_count;i++) {
		flight = flight_store[i];
		if(!strcmp(flight.code, flight_code)) {
			if(0) /*FIXME: use compareDate*/
				printf("flight already exists\n");
		}
	}

	if(!check_airport_existence(flight.departure_id)) {
		printf("%s: no such airport ID\n", flight.departure_id);
		return;
	}
	if(!check_airport_existence(flight.arrival_id)) {
		printf("%s: no such airport ID\n", flight.departure_id);
		return;
	}
	if (flight_count == MAX_FLIGHTS) {
		printf("too many flights\n");
	}
	return;
}

void listFlights() {
	char c;
	int i;
	Airport arprt;

	/*Case of command without arguments*/
	if ((c = getchar()) == '\n') {
		for(i=0;i < airport_count; i++) {
			arprt = airports[i];
			/*TODO: implement storing flight count for each airport*/
			printf("%s %s %s %d\n", arprt.id, arprt.city, arprt.country, arprt.flight_count);
		}
		return;
	}
}
void listAirportFlights() {
	/*TODO: implement*/
	return;
}

Date getDate() {
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

/*
	printf("rawday: %s\nrawmonth: %s\nrawyear: %s\n", rawday, rawmonth, rawyear);
	Borked
	fgets(rawday, 2+1, stdin);
	getchar();
	fgets(rawmonth, 2+1, stdin);
	getchar();
	fgets(rawday, 2+1, stdin);
	getchar();
	*/

	newdate.day = atoi(rawday);
	newdate.month = atoi(rawmonth);
	newdate.year = atoi(rawyear);
/*
	printf("rawday: %s\nrawmonth: %s\nrawyear: %s\n", rawday, rawmonth, rawyear);
*/
	return newdate;
}

void advanceDate() {
	Date date = getDate();
/*	
	scanf("%d%d%d", &date.day, &date.month, &date.year);
	printf("date.day = '%d'\n", date.day);
	printf("date.month = '%d'\n", date.month);
	printf("date.year = '%d'\n", date.year);
*/
	if (date.day <= 0 || date.month <= 0 || date.year <= 0 || !compareDate(system_date, date)) {
		printf("invalid date\n");
		return;
	}
	/*date.year += ano; - TODO: make sure this is irrelevant because max data <= 1 yr*/
	system_date.year = date.year;
	system_date.month = date.month;
	system_date.day = date.day;
	
	printf("%d-%d-%d\n", system_date.day, system_date.month, system_date.year);
	return;
}

/*
	Returns 1 if data1 in the past relative to data2 (delta(data) > 0)
	0 if both are the same date
	-1 if date is in the future
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
	if (date1.day> date2.day) {
		return 1;
	}
	else if(date1.day < date2.day) {
		return -1;
	}
	/*Datas são iguais */
	return 0;
}
