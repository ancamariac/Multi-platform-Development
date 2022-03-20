#ifndef HELPER_H
#define HELPER_H

#endif

#include <fcntl.h>
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

int getLine(char **line, FILE *in);

FILE *getIncFile(char *fileName, char **directories, int numDir,
		 char *currentDir);

char *replace(char *line, char *key, char *value);

void ifelse(FILE *in, FILE *out, HashMap *map, int cond, int done);

void ifdef(FILE *in, FILE *out, HashMap *map, int cond);

char *getDirectory(char *path);

char *concatenate(char *dest, const char *src);

void check_token(char *token, char **res);

void parseFile(FILE *in, FILE *out, HashMap *map, char **directories,
	       int numDir, char *inFileName);

char **getArgs(int argc, char **argv, char **input, char **output,
HashMap *map, int *numDir);
