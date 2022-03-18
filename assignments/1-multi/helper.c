#include "helper.h"
#include <string.h>

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
	const char* p = NULL;

	for (p = key; *p; p++) {
		hash += (unsigned char)(*p);
	}
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

	// aici verific daca e prea plin map (mai mult de 75%)
	// trebuie sa fac map nou pentru ca hashul se calculeaza in functie de
	// size se modifica size => hashul va returna alt index decat cel
	// calculat anterior pentru elementele deja existente index gresit => nu
	// gaseste elementul

	int i = 0;

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

	int index = hashFunc(key, map->size);

	while (1) {
		if (map->elem[index].key == NULL) {
			map->elem[index].key = malloc(strlen(key) + 1);

			if (!map->elem[index].key) {
				exit(12);
			}

			map->elem[index].value = malloc(strlen(value) + 1);

			if (!map->elem[index].value) {
				exit(12);
			}

			strcpy(map->elem[index].key, key);
			strcpy(map->elem[index].value, value);
			map->cnt++;
			return;
		}
		index += 1;
		index %= map->size;
	}
}

char *get(HashMap *map, char *key)
{
	int index = hashFunc(key, map->size);
	while (1) {
		if (map->elem[index].key == NULL) {
			return NULL;

		} else if (!strcmp(map->elem[index].key, key)) {
			return map->elem[index].value;
		}
		index += 1;
		index %= map->size;
	}
}

void delete(HashMap *map, char *key)
{
	int index = hashFunc(key, map->size);
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
