CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all:	clean pageiotest indexiotest lqtest lqtest1 lqtest2 lqtest3

pageiotest:
						gcc $(CFLAGS) pageiotest.c $(LIBS) -o $@

indexiotest:
						gcc $(CFLAGS) indexiotest.c $(LIBS) -o $@

lqtest:
						gcc $(CFLAGS) lqtest.c $(LIBS) -o $@

lqtest1:
						gcc $(CFLAGS) lqtest1.c $(LIBS) -o $@

lqtest2:
						gcc $(CFLAGS) lqtest2.c $(LIBS) -o $@

lqtest3:
						gcc $(CFLAGS) lqtest3.c $(LIBS) -o $@

clean:
						rm -f pageiotest indexiotest lqtest lqtest1 lqtest2 lqtest3
