CC = gcc
CFLAGS = -Wall -shared
fpic_FLAG = -Wall -fpic

build:  libso_stdio.so

libso_stdio.so: libso_stdio.o
	$(CC) $(CFLAGS) libso_stdio.o -o libso_stdio.so

libso_stdio.o: libso_stdio.c
	$(CC) $(fpic_FLAG) -c libso_stdio.c -o libso_stdio.o
clean:
	rm -f *.o  libso_stdio.so