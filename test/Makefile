CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all:	clean pageiotest indexiotest lqtest lqtest1 lqtest2 lqtest3 lqtest4 lqtest5 lhtest

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

lqtest4:
						gcc $(CFLAGS) lqtest4.c $(LIBS) -o $@ -lpthread

lqtest5:
						gcc $(CFLAGS) lqtest5.c $(LIBS) -o $@ -lpthread

lhtest:
						gcc $(CFLAGS) lhtest.c $(LIBS) -o $@

clean:
						rm -f pageiotest indexiotest lqtest lqtest1 lqtest2 lqtest3 lqtest4 lqtest5 lhtest
