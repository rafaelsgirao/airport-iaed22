/*
	Rafael Girão - ist199309
	2nd IAED Project 2021/2022
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "proj.h"

extern Date system_date;

/*
	Functions for handling Date struct
*/
Date readDate() {
	/*Getchar in between fgets to consume dash character */
	Date newdate;
	scanf("%d-%d-%d", &newdate.day, &newdate.month, &newdate.year);
	/* Date newdate; */
	/* char rawday[2+1], rawmonth[2+1], rawyear[4+1]; */
	/* getchar(); */
	/* rawday[0] = getchar(); */
	/* rawday[1] = getchar(); */
	/* rawday[2] = '\0'; */
	/* getchar(); */
	/* rawmonth[0] = getchar(); */
	/* rawmonth[1] = getchar(); */
	/* rawmonth[2] = '\0'; */
	/* getchar(); */
	/* rawyear[0] = getchar(); */
	/* rawyear[1] = getchar(); */
	/* rawyear[2] = getchar(); */
	/* rawyear[3] = getchar(); */
	/* rawyear[4] = '\0'; */

	/* newdate.day = atoi(rawday); */
	/* newdate.month = atoi(rawmonth); */
	/* newdate.year = atoi(rawyear); */
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
		printf(MSG_INVALID_DATE);
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
