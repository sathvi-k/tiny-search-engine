CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all: clean indexer indexer3 indexer4 indexer5t indexer5

indexer:
				gcc $(CFLAGS) indexer.c $(LIBS) -o $@
indexer3:
				gcc $(CFLAGS) indexer3.c $(LIBS) -o $@
indexer4:
				gcc $(CFLAGS) indexer4.c $(LIBS) -o $@
indexer5t:
				gcc $(CFLAGS) indexer5t.c $(LIBS) -o $@
indexer5:
				gcc $(CFLAGS) indexer5.c $(LIBS) -o $@
clean:
				rm -f indexer indexer3 indexer4 indexer5t indexer5
