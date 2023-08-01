CC=clang
CFLAGS=-g -Wall -std=gnu99 -I.
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
HEADERS=$(SOURCES:.c=.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

cli: $(OBJECTS)
	$(CC) $(CFLAGS) -o build/$@ $^
