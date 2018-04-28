/*
This program updates files in the live site.
The program uses rsync to sync the files from the intranet to the live folder
The program has one function:
	-updateLive
	This function accepts 2 parameters: source and dest
		>The source is the intranet folder
		>The dest is the live folder.

Author: Pamela Sabio
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

#include "syslogger.h"

void updateLive(char *source, char *dest){
	//copy file from source to dest
	char *args[] = {"rsync", "-r", source, dest, NULL};
	printf(*args);
	if(!vfork()){
		execv("usr/bin/rsync", args);
	}

	logSuccessMessage("Live Website successfully updated");
}
