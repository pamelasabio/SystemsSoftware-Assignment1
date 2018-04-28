/*
This program backs up the files by copying the files in a folder to a backup folder.
The program has one function:
	-backupFiles
	This function accepts 2 parameters: source and dest.
		>The source is the path of the folder to be copied.
		>The dest is the path where the folder is copied.

Author: Pamela Sabio
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include "changePermissions.h"
#include "timestamp.h"
#include "syslogger.h"

void backupFiles(char *source, char *dest){
	//concatenate the date to dest to make it the name of the backup file
	char date[50];
	char *time = date_toString(date);;
	strcat(dest,time);

	//copy file from source to dest
	char *args[] = {"cp", "-R", source, dest, NULL};

	if(!vfork()){
		execv("bin/cp", args);
	}

	logSuccessMessage("Backup Complete");
}
