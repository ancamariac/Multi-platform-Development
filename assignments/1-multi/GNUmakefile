CC = gcc
CFLAGS = -Wextra -Wall -g

build: so-cpp

so-cpp: main.o helper.o hashmap.o
		$(CC) $(CFLAGS) $^ -o so-cpp

main.o: main.c
		$(CC) $(CFLAGS) -c $^

hashmap.o: hashmap.c hashmap.h
		$(CC) $(CFLAGS) -c $^

helper.o: helper.c helper.h
		$(CC) $(CFLAGS) -c $^

.PHONY: clean

clean: 
		rm -f *.o so-cpp
