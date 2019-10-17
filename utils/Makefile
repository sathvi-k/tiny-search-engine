CFLAGS=-Wall -pedantic -std=c11 -I. -g
OFILES=queue.o hash.o webpage.o

all:	$(OFILES)
			ar cr ~/engs50/tse/lib/libutils.a $(OFILES)

%.o:	%.c %.h
			gcc $(CFLAGS) -c $<

webpage.o:	webpage.c webpage.h
						gcc $(CFLAGS) -c webpage.c

qtest:	qtest.o queue.o
				gcc $(CFLAGS) qtest.o queue.o -o $@

htest: htest.o hash.o queue.o
				gcc $(CFLAGS) htest.o hash.o queue.o -o $@

clean:
				rm -f *.o qtest htest
