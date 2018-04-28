/*
This program logs the messages to the syslog.
To view the log:
	-cd /var/log
	-cat user.log | grep systemsoftware_log

Author: Pamela Sabio
*/
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

void logSuccessMessage(char *m){
	openlog("systemsoftware_log",LOG_PID|LOG_CONS, LOG_USER);
	syslog(LOG_INFO,"%s\n",m);
	closelog();
}

void logErrorMessages(char *m1, char *m2){
	openlog("systemsoftware_log",LOG_PID|LOG_CONS, LOG_USER);
        syslog(LOG_ERR,"%s: %s\n",m1,m2);
        closelog();
}
