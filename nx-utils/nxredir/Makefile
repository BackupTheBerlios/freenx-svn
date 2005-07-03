all: libnxredir.so.0.1

CC=gcc
CFLAGS=-g -O2 -Wall

libnxredir.so.0.1: nxredir.o
	$(CC) -fPIC $(CFLAGS) -nostdlib -shared -o libnxredir.so.0.1 nxredir.o -ldl -lc

clean:
	rm -f libnxredir.so.0.1
	rm -f *.o

install:
	cp -a libnxredir.so.0.1 $(DESTDIR)/usr/lib
	cp -a nxredir $(DESTDIR)/usr/bin
	ldconfig

.PHONY: clean install
