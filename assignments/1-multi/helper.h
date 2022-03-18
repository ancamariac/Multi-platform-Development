#ifndef HELPER_H
#define HELPER_H

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Elem {
	char *key;
	char *value;
} Elem;

typedef struct HashMap {
	Elem *elem;
	int cnt;
	int size;
} HashMap;

HashMap createHashMap(int size);

int hashFunc(char *key, int size);

void deleteMap(HashMap *map);

void insert(HashMap *map, char *key, char *value);

char *get(HashMap *map, char *key);

void delete(HashMap *map, char *key);
