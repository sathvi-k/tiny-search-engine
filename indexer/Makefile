CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all: clean indexer indexer3

indexer:
				gcc $(CFLAGS) indexer.c $(LIBS) -o $@
indexer3:
				gcc $(CFLAGS) indexer3.c $(LIBS) -o $@
clean:
				rm -f indexer indexer3
