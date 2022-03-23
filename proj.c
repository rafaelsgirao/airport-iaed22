#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "proj.h"

/*-------------------*/
/*-Global Variables--*/
/*-------------------*/

Date system_date;
int airport_count;

Airport airports[MAX_AIRPORTS];

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
		if (strcmp(arprt.id, airports[i].id) == 0) {
			printf("duplicate airport\n");
			return;
		}
	}
	/*Read Airport city, can be a string with spaces*/
	fgets(arprt.city, LIM_CITY_NAME ,stdin);
	/*TODO: store the new airport, ordered alphabetically*/
	airports[airport_count++] = arprt;
	printf("airport %s\n", arprt.id);
	return;
	}


void listAirports() {
	int i;
	char c;
	Airport arprt;
	/*Case of command without arguments*/
	if ((c = getchar() == '\n')) {
		for(i=0;i<airport_count;i++) {
			arprt = airports[airport_count];
			/*TODO: implement storing flight count for each airport*/
			printf("%s %s %s #%d", arprt.id, arprt.city, arprt.country, 0);
		}
		return;
	}

	/*TODO: implement*/
	return;
}

void addFlight() {
	/*TODO: implement*/
	return;
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

	fgets(rawday, 2+1, stdin);
	getchar();
	fgets(rawmonth, 2+1, stdin);
	getchar();
	fgets(rawday, 2+1, stdin);
	getchar();

	newdate.day = atoi(rawday);
	newdate.month = atoi(rawmonth);
	newdate.year = atoi(rawyear);
	return newdate;
}

void advanceDate() {
	Date date = getDate();
	scanf("%d%d%d", &date.day, &date.month, &date.year);
	if (date.day <= 0 || date.month <= 0 || date.year <= 0 || compareDate(system_date, date)) {
		printf("invalid date\n");
		return;
	}

	/*date.year += ano; - TODO: make sure this is irrelevant because max data <= 1 yr*/
	system_date.month += date.month;
	system_date.day += date.day;
	

	return;
}

/*
	Returns 1 if data1 in the past relative to data2 (delta(data) > 0)
	0 if both are the same date
	-1 if date is in the future
*/
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
