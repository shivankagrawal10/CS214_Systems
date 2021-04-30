CC = gcc
CFLAGS = -Wall -Werror -fsanitize=address -pthread

all: server

server.o: server.c
	$(CC) $(CFLAGS) -c server.c -g

strbuf.o: strbuf.c
	$(CC) $(CFLAGS) -c strbuf.c -g

server: server.o strbuf.o 
	$(CC) $(CFLAGS) -o server strbuf.o server.o -g -lm

clean:
	rm -f *.o
