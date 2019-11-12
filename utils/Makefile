CFLAGS=-Wall -pedantic -std=c11 -I. -g
OFILES=queue.o hash.o webpage.o pageio.o indexio.o lqueue.o
LIBFILE=../lib/libutils.a

all:	$(OFILES) lqueuetest lqueuetest1
			ar cr $(LIBFILE) $(OFILES)

%.o:	%.c %.h
			gcc $(CFLAGS) -c $<

lqueuetest:lqueuetest.o lqueue.o queue.o
						gcc $(CFLAGS) lqueuetest.o lqueue.o queue.o -o $@ -lpthread

lqueuetest1:lqueuetest1.o lqueue.o queue.o
						gcc $(CFLAGS) lqueuetest1.o lqueue.o queue.o -o $@ -lpthread

clean:
				rm -f $(OFILES) $(LIBFILE) lqueuetest lqueuetest1
