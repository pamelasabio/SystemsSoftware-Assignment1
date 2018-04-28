CC = gcc
objects = obj/daemon.o obj/backup.o obj/update.o obj/changePermissions.o obj/timestamp.o obj/audit.o obj/syslogger.o
cfiles = daemon.c backup.c update.c changePermissions.c timestamp.c audit.c syslogger.c
headers = update.h backup.h changePermissions.h timestamp.h audit.h syslogger.h
name = myProg
OBJDIR = obj

myProg: $(objects)
	$(CC) -o $(name) $(objects) -lm

$(OBJDIR)/daemon.o: daemon.c $(headers)
	$(CC) -c daemon.c -o $(OBJDIR)/daemon.o

$(OBJDIR)/backup.o: backup.c $(headers)
	$(CC) -c backup.c -o $(OBJDIR)/backup.o

$(OBJDIR)/update.o: update.c $(headers)
	$(CC) -c update.c -o $(OBJDIR)/update.o

$(OBJDIR)/changePermissions.o: changePermissions.c $(headers)
	$(CC) -c changePermissions.c -o $(OBJDIR)/changePermissions.o

$(OBJDIR)/timestamp.o: timestamp.c $(headers)
	 $(CC) -c timestamp.c -o $(OBJDIR)/timestamp.o

$(OBJDIR)/audit.o: audit.c $(headers)
	$(CC) -c audit.c -o $(OBJDIR)/audit.o

$(OBJDIR)/syslogger.o: syslogger.c $(headers)
	$(CC) -c syslogger.c -o $(OBJDIR)/syslogger.o

clean:
	rm $(name) $(objects)

