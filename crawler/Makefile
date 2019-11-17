CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all: clean crawler crawler3 crawler4 crawler5 crawlerf pcrawlerf

crawler:
				gcc $(CFLAGS) crawler.c $(LIBS) -o $@

crawler3:
				gcc $(CFLAGS) crawler3.c $(LIBS) -o $@

crawler4:
				gcc $(CFLAGS) crawler4.c $(LIBS) -o $@

crawler5:
				gcc $(CFLAGS) crawler5.c $(LIBS) -o $@

crawlerf:
				gcc $(CFLAGS) crawlerf.c $(LIBS) -o $@

pcrawlerf:
				gcc $(CFLAGS) pcrawlerf.c $(LIBS) -o $@ -lpthread

run2:
				rm -f ../pages/*
				./crawlerf https://thayer.github.io/engs50/ pages 2

grind2:
				rm -f ../pages/*
				valgrind --leak-check=full ./crawlerf https://thayer.github.io/engs50/ pages 2 

clean:
				rm -f crawler crawler3 crawler4 crawler5 crawlerf pcrawlerf
