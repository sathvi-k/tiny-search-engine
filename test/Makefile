CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all:	clean pageiotest indexiotest lqtest

pageiotest:
						gcc $(CFLAGS) pageiotest.c $(LIBS) -o $@

indexiotest:
						gcc $(CFLAGS) indexiotest.c $(LIBS) -o $@

lqtest:
						gcc $(CFLAGS) lqtest.c $(LIBS) -o $@

clean:
						rm -f pageiotest indexiotest lqtest
