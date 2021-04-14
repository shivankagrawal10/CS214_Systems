CC = gcc
CFLAGS = -Wall -Werror -fsanitize=address

all: compare

compare.o: compare.c
	$(CC) $(CFLAGS) -c compare.c -g

strbuf.o: strbuf.c
	$(CC) $(CFLAGS) -c strbuf.c -g

compare: compare.o strbuf.o 
	$(CC) $(CFLAGS) -o compare strbuf.o compare.o -g -lm

clean:
	rm -f *.o
