CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all:	clean pageiotest 

pageiotest:
						gcc $(CFLAGS) pageiotest.c $(LIBS) -o $@

clean:
						rm -f pageiotest
