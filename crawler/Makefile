CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all: clean crawler crawler3 crawler4

crawler:
				gcc $(CFLAGS) crawler.c $(LIBS) -o $@

crawler3:
				gcc $(CFLAGS) crawler3.c $(LIBS) -o $@

crawler4:
				gcc $(CFLAGS) crawler4.c $(LIBS) -o $@

clean:
				rm -f crawler crawler3 crawler4
