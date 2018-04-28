/*
This program creates a daemon to continually manage the operation of the
requirements for the new website management model:
	-Back up website content every night
	-Document the changes made to the Intranet version of the site: record username, page modified and timestamp.
	-Update live site based on the changes made to the Intranet site during the night.
	-Change permissions of the files so that no changes can be made while backup/update is happening.

Author: Pamela Sabio
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#include "changePermissions.h"
#include "backup.h"
#include "update.h"
#include "audit.h"
#include "syslogger.h"

#define INTRANET_PATH "/home/pamelalouisse/Desktop/Assignment/var/www/html/intranet/"
#define LIVE_PATH "/home/pamelalouisse/Desktop/Assignment/var/www/html/live/"
#define BACKUP_PATH "/home/pamelalouisse/Desktop/Assignment/var/www/html/backup/"

void signal_handler(int signal){
	char intranet_path[255];
	char live_path[255];
	strcpy(intranet_path,INTRANET_PATH);
	strcpy(live_path,LIVE_PATH);
	if(signal == SIGUSR1){
		updateLive(intranet_path,live_path);
	}
}

int main(){
	time_t now;
	struct tm newyear;
	double seconds;
	time(&now);//get current time; same as now = time(NULL)
	newyear = *localtime(&now);
	newyear.tm_hour = 23;
	newyear.tm_min = 59;
	newyear.tm_sec = 00;

	//Start watching the intranet directory for modification
	char *start_filewatch = "auditctl -w /home/pamelalouisse/Desktop/Assignment/var/www/html/intranet/ -p wa";
	if(system(start_filewatch) < 0){
		logErrorMessages("Failed to start Auditd",strerror(errno));
	}else{
		logSuccessMessage("Website filewatch is active");
	}

	//Create a child process
	int pid = fork();

	if(pid > 0){
		//if PID > 0 :: this is the parent
		//printf("Parent");
		//sleep(1); //wait 1 seconds before killing the process
		exit(EXIT_SUCCESS);
	}else if(pid == 0){
		//Step 1: Create the orphan process
		//printf("Child process");

		//Step 2: Elevate the orphan process to session leader
		//to loose control in TTY
		//This command run the process in a new session
		if(setsid() < 0){
			exit(EXIT_FAILURE);
		}

		//We could for here again to guarantee that the process
		//is not a session loader
		int pid = fork();
		if(pid >0){
			exit(EXIT_SUCCESS);
		}else{

			//Step 3: call umask() to set the file mode creation mask to 0
			//This will allow the daemon to read and write files
			//with the permissions/access required
			umask(0);

			//Step 4: Change the current working dir to root.
			//This will eliminate any issues of running on a mounted drive
			//that potentially could be removed etc.
			if(chdir("/")< 0){
				exit(EXIT_FAILURE);
			}

			//Step 5: Close all open file descriptors
			//Close all open file descriptors
			int x;
			for(x = sysconf(_SC_OPEN_MAX); x >= 0; x--){
				close(x);
			}

			// Signal handler
			if(signal(SIGUSR1, signal_handler) == SIG_ERR){
				logErrorMessages("daemon.c (signal):", "Failed to upload files to live site");
			}else{
				logSuccessMessage("Live website urgently updated");
			}


			//Orphan logic goes here
			//Keep process running with infinite loop
			//When the parent finishes.
			while(1){
				sleep(1);
				time(&now);
				seconds = difftime(now,mktime(&newyear));
				printf("\n%.f", seconds);

				if(seconds == 0){
					char live_path[255];
                                	char backup_path[255];
					char intranet_path[255];
                                	strcpy(live_path,LIVE_PATH);
                                	strcpy(backup_path,BACKUP_PATH);
					strcpy(intranet_path,INTRANET_PATH);

					//Lock files
					lockFiles(live_path);
					printf("Files locked\n");

					//Back up the files in the intranet folder
					backupFiles(live_path,backup_path);
					printf("Back up complete\n");

					//Create auditd record to document changes in the intranet folder and log it into a file.
					createAuditLog();
					printf("Audit Log created\n");

					//Update the files in the live folder to update the live site
					updateLive(intranet_path,live_path);
					printf("Live site updated\n");

					//Unlock files
					unlockFiles(live_path);
				}
			}
		}
	}
	return 0;
}
