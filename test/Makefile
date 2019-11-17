CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all:	clean pageiotest indexiotest lqtest lqtest1 lqtest2 lqtest3 lqtest4 lqtest5 lhtest lhtest1 lhtest2 lhtest3 lhtest4

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

lhtest1:
						gcc $(CFLAGS) lhtest1.c $(LIBS) -o $@

lhtest2:
						gcc $(CFLAGS) lhtest2.c $(LIBS) -o $@

lhtest3:
						gcc $(CFLAGS) lhtest3.c $(LIBS) -o $@ -lpthread

lhtest4:
						gcc $(CFLAGS) lhtest4.c $(LIBS) -o $@ -lpthread

clean:
						rm -f pageiotest indexiotest lqtest lqtest1 lqtest2 lqtest3 lqtest4 lqtest5 lhtest lhtest1 lhtest2 lhtest3 lhtest4
