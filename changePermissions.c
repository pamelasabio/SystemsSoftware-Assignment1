/*
This program changes the user permissions for the files/folder.
The path of the files/folder is passed to the program.
The program has two functions:
	-lockFiles - disables users to modify the files
	-unlockFiles - enables users to modify the files

Author:Pamela Sabio
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#include "syslogger.h"

//This function disables write permission to the files in order to disable users from modifying.
int lockFiles(char *path){
	int status;
	struct stat buffer;
	struct stat st;

	mode_t mode;
	stat(path, &st);

	mode = st.st_mode & 33188; //33188 = 644(r+w for owners and r only for everyone else)
	mode &= ~(S_IRUSR); //S_IRUSR = Read permission bit for the owner of the file
	mode |= S_IXUSR; //S_IXUSR = Execute(files) or search(directories) permission bit for the owner of the file.

	if(chmod(path,mode) < 0){
		logErrorMessages("Locking files failed: ", strerror(errno));
	}else{
		logSuccessMessage("Files locked successfully\n");
	}
	status = stat(path, &buffer);

	return status;
}

//This function changes permissions to enable users to modify the files.
int unlockFiles(char *path){
	int status;
	struct stat buffer;
	struct stat st;

	mode_t mode;
	stat(path, &st);

	mode = st.st_mode & 07777; //07777 = 420
	mode &= ~(S_IRUSR);
	mode != S_IXUSR;

	if(chmod(path,mode) < 0){
                logErrorMessages("Unlocking files failed: ", strerror(errno));
        }else{
                logSuccessMessage("Files unlocked successfully\n");
        }

	status = stat(path, &buffer);

	return status;
}
