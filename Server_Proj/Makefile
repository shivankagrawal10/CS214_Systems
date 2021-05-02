CC = gcc
CFLAGS = -Wall -Werror -fsanitize=address -pthread

all: server

server.o: server.c
	$(CC) $(CFLAGS) -c server.c -g

strbuf.o: strbuf.c
	$(CC) $(CFLAGS) -c strbuf.c -g

LLNode.o: LLNode.c
	$(CC) $(CFLAGS) -c LLNode.c -g

server: server.o strbuf.o 
	$(CC) $(CFLAGS) -o server strbuf.o server.o -g -lm

temp.o: temp_main.c
	$(CC) $(CFLAGS) -c temp_main.c -g

temp: temp_main.o strbuf.o LLNode.o
	$(CC) $(CFLAGS) -o temp strbuf.o temp_main.o LLNode.o -g -lm

clean:
	rm -f *.o
