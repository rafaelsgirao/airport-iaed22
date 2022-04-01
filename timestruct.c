/*
	Rafael Girão - ist199309
	1st IAED Project 2021/2022
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "proj.h"
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
