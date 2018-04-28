/*
This program gets the timestamp by converting the format of time to string
This program has two functions:
	-date_toString
	This function takes one parameter: date
		>The date is a string that has been converted from time_t variable

Author:Pamela Sabio
*/

#include <stdio.h>
#include <time.h>

char *date_toString(char *date){
	time_t now;
        struct tm *timeinfo;

        time(&now);
        timeinfo = localtime(&now);

        //Format time as a string
        strftime(date, 80, "%d_%m_%Y", timeinfo);

        return date;
}
char *dateTime_toString(char *time_buffer){
	time_t now;
	struct tm *timeinfo;

	time(&now);
	timeinfo = localtime(&now);

	//Format time as a string
	strftime(time_buffer, 30, "[%Y-%m-%d %H:%M:%S]", timeinfo);

	return time_buffer;
}


