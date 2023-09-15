all: httpserver
CFLAGS = -Wall -pedantic -Werror -Wextra 
CC = clang 
all: httpserver

httpserver: httpserver.o httplibrary.o
	$(CC) $(CFLAGS) -g httpserver.o httplibrary.o -o httpserver 

httplibrary.o: httplibrary.c httplibrary.h
	$(CC) $(CFLAGS) -g -c httplibrary.c

httpserver.o: httplibrary.o 
	$(CC) $(CFLAGS) -g -c httpserver.c 

clean: 
	rm -f *.o httpserver
