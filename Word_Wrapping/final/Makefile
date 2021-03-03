CC = gcc
CFLAGS = -Wall -Werror -fsanitize=address   
all:  str

arraylist.o: arraylist.c
	$(CC) $(CFLAGS) -c arraylist.c -g

driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c -g

strbuf.o: strbuf.c
	$(CC) $(CFLAGS) -c strbuf.c -g

arr: driver.o arraylist.o
	$(CC) $(CFLAGS) -o arr arraylist.o driver.o -g

str: driver.o strbuf.o
	$(CC) $(CFLAGS) -o str strbuf.o driver.o -g

clean: 
	rm -f *.o driver
