// SPDX-License-Identifier: GPL-2.0-or-later

#include "hashmap.h"

#define MAX_SIZE 20

HashMap createHashMap(int size)
{
	HashMap newMap;

	int i = 0;

	newMap.size = size;
	newMap.cnt = 0;
	newMap.elem = malloc(newMap.size * sizeof(Elem));

	if (!newMap.elem)
		exit(12);

	for (i = 0; i < newMap.size; i++) {
		newMap.elem[i].key = NULL;
		newMap.elem[i].value = NULL;
	}
	return newMap;
}

int hashFunc(char *key, int size)
{
	int hash = 0;
	const char *p = NULL;

	for (p = key; *p; p++)
		hash += (unsigned char)(*p);
	return hash % size;
}

void deleteMap(HashMap *map)
{
	int i = 0;

	for (i = 0; i < map->size; i++) {
		if (map->elem[i].key) {
			free(map->elem[i].key);
			free(map->elem[i].value);
		}
	}
	free(map->elem);
}

void insert(HashMap *map, char *key, char *value)
{
	int i = 0;
	int index = 0;

	if (map->cnt * 4 > map->size * 3) {
		HashMap newMap = createHashMap(2 * map->size);

		for (i = 0; i < map->size; i++) {
			if (map->elem[i].key != NULL) {
				insert(&newMap, map->elem[i].key,
				       map->elem[i].value);
			}
		}
		deleteMap(map);
		*map = newMap;
	}

	index = hashFunc(key, map->size);

	while (1) {
		if (map->elem[index].key == NULL) {
			map->elem[index].key = malloc(strlen(key) + 1);

			if (!map->elem[index].key)
				exit(12);

			map->elem[index].value = malloc(strlen(value) + 1);

			if (!map->elem[index].value)
				exit(12);

			strcpy(map->elem[index].key, key);
			strcpy(map->elem[index].value, value);
			map->cnt++;
			return;
		} else if (strcmp(map->elem[index].key, key) == 0) {
			
			free(map->elem[index].value);
			map->elem[index].value = malloc(strlen(value) + 1);

			if (!map->elem[index].value)
				exit(12);
			
			strcpy(map->elem[index].value, value);
			return;

		}
		index += 1;
		index %= map->size;
	}
}

char *get(HashMap *map, char *key)
{
	int index = 0;

	index = hashFunc(key, map->size);

	while (1) {
		if (map->elem[index].key == NULL)
			return NULL;
		else if (!strcmp(map->elem[index].key, key))
			return map->elem[index].value;
		index += 1;
		index %= map->size;
	}
}

void deleteKey(HashMap *map, char *key)
{
	int index = 0;

	index = hashFunc(key, map->size);

	while (1) {
		if (map->elem[index].key == NULL) {
			return;

		} else if (!strcmp(map->elem[index].key, key)) {
			free(map->elem[index].key);
			free(map->elem[index].value);
			map->elem[index].key = NULL;
			map->elem[index].value = NULL;

			return;
		}
		index += 1;
		index %= map->size;
	}
}
