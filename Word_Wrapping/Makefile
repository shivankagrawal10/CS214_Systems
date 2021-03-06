CC = gcc
CFLAGS = -Wall -Werror -fsanitize=address   

all: ww

ww.o: ww.c
	$(CC) $(CFLAGS) -c ww.c -g

strbuf.o: strbuf.c
	$(CC) $(CFLAGS) -c strbuf.c -g

ww: strbuf.o ww.o
	$(CC) $(CFLAGS) -o ww strbuf.o ww.o -g

clean:
	rm -f *.o