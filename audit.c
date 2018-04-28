/*
This program uses auditd to track all changes made to the intranet folder.
This program has one function:
	-createAuditLog
	This function creates logs the audit into a text file.
	The text file is saved in the changes_log folder.
	The name of the file is the date of the creation of file.
Author: Pamela Sabio
*/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "timestamp.h"
#include "syslogger.h"

void createAuditLog(){
        char date_buffer[80];
        char *date = date_toString(date_buffer);
        char *path_beginning = "ausearch -ts today -f home/pamelalouisse/Desktop/Assignment/var/www/html/intranet/ -i > home/pamelalouisse/Desktop/Assignment/changes_log/";
        char *file_type = ".txt";
        int newSize = strlen(path_beginning) + strlen(date) + strlen(file_type) + 1;
        char *new_buffer = (char *)malloc(newSize); 
        strcpy(new_buffer,path_beginning); //file path
        strcat(new_buffer,date); //name of the file is the date
        strcat(new_buffer,file_type); //file type
	if(system(new_buffer) < 0){
		logErrorMessages("Failed to Auditd", strerror(errno));
	}else{
		logSuccessMessage("Audit Report has been created");
	}
        free(new_buffer);
}

