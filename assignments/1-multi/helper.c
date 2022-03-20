// SPDX-License-Identifier: GPL-2.0-or-later

#include "helper.h"
#include "hashmap.h"

#define MAXLEN 256

int getLine(char **line, FILE *in)
{
	char readline[MAXLEN];
	int len;

	if (*line) {
		free(*line);
		*line = NULL;
	}

	if (fgets(readline, MAXLEN, in) == NULL)
		return -1;

	len = strlen(readline);
	*line = malloc((len + 1) * sizeof(char));

    if (!line)
        exit(12);
	strcpy(*line, readline);
	return len;
}

FILE *getIncFile(char *fileName, char **directories, int numDir,
		 char *currentDir)
{
	FILE *file;
	int i = 0;

	char *path = NULL;
	char *filePath = NULL;

	filePath =
	    malloc((strlen(currentDir) + strlen(fileName) + 1) * sizeof(char));

	if (!filePath)
		exit(12);
	strcpy(filePath, currentDir);
	strcat(filePath, fileName);

	file = fopen(filePath, "r");

	if (file) {
		free(filePath);
		return file;
	}

	free(filePath);

	for (i = 0; i < numDir; i++) {
		path = malloc((strlen(directories[i]) + strlen(fileName) + 2) *
			      sizeof(char));

		if (!path)
			exit(12);
		strcpy(path, directories[i]);
		strcat(path, "/");
		strcat(path, fileName);

		file = fopen(path, "r");

		if (file) {
			free(path);
			return file;
		}
		free(path);
	}

	return NULL;
}

char *replace(char *line, char *key, char *value)
{
	int len = strlen(line);
	int len1 = strlen(key);
	int len2 = strlen(value);
	int len_res = len + len2 - len1 + 1;
	int found = 0;
	char *result;
	int i;

	for (i = 0; i < len; i++) {
		if (line[i] == '\"') {
			i++;
			while (line[i] != '\"')
				i++;
			i++;
		}
		if (strncmp(&line[i], key, len1) == 0) {
			found = 1;
			break;
		}
	}

	if (found) {
		result = malloc((len_res + 1) * sizeof(char));

		if (!result)
			exit(12);

		strncpy(result, line, i);
		strncpy(result + i, value, len2);
		strcpy(result + i + len2, line + i + len1);
		free(line);
		return result;
	} else {
		return line;
	}

	free(line);
	return result;
}

void ifelse(FILE *in, FILE *out, HashMap *map, int cond, int done)
{

	char *line = NULL;
	int read = 0;
	static const char delimiters[] = "\t []{}<>=+-*/%!&|^.,:;()\\";

	char *val = NULL;
	char *token = NULL;

	if (done == 1) {
		while ((read = getLine(&line, in)) != -1) {
			if (!strncmp(line, "#endif", 6)) {
				free(line);
				return;
			}
		}
		return;
	}

	if (cond == 0) {
		while ((read = getLine(&line, in)) != -1) {
			if (!strncmp(line, "#endif", 6)) {
				free(line);
				return;
			} else if (!strncmp(line, "#else", 5)) {
				ifelse(in, out, map, 1, 0);
				free(line);
				return;
			} else if (!strncmp(line, "#elif", 5)) {
				token = strtok(line, delimiters);
				token = strtok(NULL, "\n");
				val = get(map, token);
				if (val != NULL)
					token = val;
				if (!strcmp(token, "0")) {
					ifelse(in, out, map, 0, 0);
					free(line);
					return;
				}

				ifelse(in, out, map, 1, 0);
				free(line);
				return;
			}
		}
	} else {
		while ((read = getLine(&line, in)) != -1) {
			if (!strncmp(line, "#endif", 6)) {
				free(line);
				return;
			} else if (!strncmp(line, "#else", 5)) {
				ifelse(in, out, map, 1, 1);
				free(line);
				return;
			} else if (!strncmp(line, "#elif", 5)) {
				ifelse(in, out, map, 1, 1);
				free(line);
			} else {
				fputs(line, out);
			}
		}
	}
	free(line);
}

void ifdef(FILE *in, FILE *out, HashMap *map, int cond)
{
	char *line = NULL;
	int read = 0;

	char *value = NULL;
	char *key = NULL;

	static const char delimiters[] = "\t []{}<>=+-*/%!&|^.,:;()\\\n";

	if (cond == 0) {
		while ((read = getLine(&line, in)) != -1) {
			if (!strncmp(line, "#endif", 6)) {
				free(line);
				return;
			}
		}
	} else {
		while ((read = getLine(&line, in)) != -1) {
			if (!strncmp(line, "#endif", 6)) {
				free(line);
				return;
			}

			if (!strncmp(line, "#define", 7)) {
				key = strtok(line, delimiters);
				key = strtok(NULL, delimiters);
				value = strtok(NULL, "\n");

				if (value)
					insert(map, key, value);
				else
					insert(map, key, "");
			} else if (!strncmp(line, "#undef", 6)) {
				key = strtok(line, delimiters);
				key = strtok(NULL, delimiters);
				delete(map, key);
			} else {
				fputs(line, out);
			}
		}
	}
}

char *getDirectory(char *path)
{
	char *dir = NULL;
	char *fileName = NULL;

	int i = 0;

	if (!strcmp(path, ""))
		return path;

	fileName = strrchr(path, '/');
	if (!fileName) {
		dir = malloc(sizeof(char));
		strcpy(dir, "");
		return dir;
	}

	dir = malloc(sizeof(char) * (strlen(path) - strlen(fileName) + 2));

	if (!dir)
		exit(12);

	while (path != fileName) {
		dir[i] = path[0];
		i++;
		path++;
	}

	dir[i] = '/';
	dir[i + 1] = '\0';

	return dir;
}

char *concatenate(char *dest, const char *src)
{
	const size_t a = strlen(dest);
	const size_t b = strlen(src);
	const size_t size_ab = a + b + 2;

	dest = realloc(dest, size_ab);

	if (!dest)
		exit(12);

	dest[a] = ' ';
	memcpy(dest + a + 1, src, b + 1);
	return dest;
}

void check_token(char *token, char **res)
{
	if (token != NULL)
		*res = concatenate(*res, token);
}

void parseFile(FILE *in, FILE *out, HashMap *map, char **directories,
	       int numDir, char *inFileName)
{
	FILE *incFile = NULL;

	int read = 0;

	char *line = NULL;
	char *result = NULL;
	char *result_m = NULL;
	char *line_copy = NULL;
	char *inputDir = NULL;
	char *val = NULL;
	char *token = NULL;
	char *key = NULL;
	char *value = NULL;
	char *another_value = NULL;
	char *parsed_value = NULL;

	static const char delimiters[] = "\t []{}<>=+-*/%!&|^.,:;()\\";
	static const char multi_lines_delim[] = "\\t\n \\";

	read = getLine(&line, in);
	while (read != -1) {
		line_copy = malloc((strlen(line) + 1) * sizeof(char));

		if (!line_copy)
			exit(12);

		result = malloc((strlen(line) + 1) * sizeof(char));

		if (!result)
			exit(12);

		strcpy(line_copy, line);
		strcpy(result, line);
		token = strtok(line_copy, delimiters);

		if (!strcmp(token, "#define")) {
			key = strtok(NULL, delimiters);
			value = strtok(NULL, "\n");
			// daca se gasesc spatii in value,
			// inseamna ca mai sunt keys
			// care trebuiesc inlocuite
			if (strchr(value, ' ')) {
				// multi lines define
				if (value[strlen(value) - 1] == '\\') {
					parsed_value = malloc((strlen(value) + 1) *
					sizeof(char));

					if (!parsed_value)
						exit(12);

					strcpy(parsed_value, value);
					do {
						read = getLine(&line, in);
						parsed_value =
						concatenate(parsed_value, line);
					} while (line[strlen(line) - 2] == '\\');

					token = strtok(parsed_value, multi_lines_delim);
					result_m = malloc((strlen(token) + 1) * sizeof(char));
					if (!result_m)
						exit(12);

					strcpy(result_m, token);

					while (token != NULL) {
						token = strtok(NULL, multi_lines_delim);
						check_token(token, &result_m);
					}
					insert(map, key, result_m);
					free(result_m);
					free(parsed_value);
				}
				parsed_value = malloc((strlen(value) + 1) * sizeof(char));

				if (!parsed_value)
					exit(12);
				strcpy(parsed_value, value);

				token = strtok(value, delimiters);

				while (token != NULL) {
					another_value = get(map, token);
					if (another_value != NULL)
						parsed_value =
						replace(parsed_value, token, another_value);
					token = strtok(NULL, delimiters);
				}
				insert(map, key, parsed_value);
				free(parsed_value);
			} else {
				insert(map, key, value);
			}
		} else if (!strcmp(token, "#undef")) {
			key = strtok(NULL, "\n");
			delete(map, key);
		} else if (!strcmp(token, "#if")) {
			token = strtok(NULL, "\n");
			val = get(map, token);

			if (val != NULL)
				token = val;
			if (!strcmp(token, "0"))
				ifelse(in, out, map, 0, 0);
			else
				ifelse(in, out, map, 1, 0);
		} else if (!strcmp(token, "#ifdef")) {
			token = strtok(NULL, "\n");
			if (get(map, token) != NULL)
				ifdef(in, out, map, 1);
			else
				ifdef(in, out, map, 0);
		} else if (!strcmp(token, "#ifndef")) {
			if (get(map, token) != NULL)
				ifdef(in, out, map, 0);
			else
				ifdef(in, out, map, 1);
		} else if (!strcmp(token, "#include")) {
			token = strtok(line, "\"");
			token = strtok(NULL, " \"");
			inputDir = getDirectory(inFileName);
			incFile =
			    getIncFile(token, directories, numDir, inputDir);

			if (incFile != NULL) {
				parseFile(incFile, out, map, directories,
					  numDir, inFileName);
				fclose(incFile);
			} else {
				exit(1);
			}

			free(inputDir);
		} else {
			while (token != NULL) {
				value = get(map, token);
				if (value != NULL)
					result = replace(result, token, value);
				token = strtok(NULL, delimiters);
			}
			fputs(result, out);
		}

		free(line_copy);
		free(result);
		free(line);
		line = NULL;
		read = getLine(&line, in);
	}
	free(line);
}

char **getArgs(int argc, char **argv, char **input, char **output,
HashMap *map, int *numDir)
{
	int currDir = 0;
	char **directories = NULL;
	int i = 0;

	for (i = 1; i < argc; i++) {
		if (!strncmp(argv[i], "-I", 2))
			*numDir += 1;
	}
	if (*numDir != 0) {
		directories = malloc((*numDir) * sizeof(char *));
		if (!directories)
			exit(12);
	}

	for (i = 1; i < argc; i++) {
		if (argv[i] && argv[i][0] == '-') {
			if (argv[i][1] == 'D') {
				char *key, *val;

				if (strlen(argv[i]) == 2) {
					key = strtok(argv[i + 1], "=");
					i++;
				} else {
					key = strtok(argv[i] + 2, "=");
				}

				val = strtok(NULL, "");

				if (val == NULL)
					val = "";
				insert(map, key, val);

			} else if (argv[i][1] == 'I') {
				if (strlen(argv[i]) == 2) {
					directories[currDir] =
					    malloc((strlen(argv[i + 1]) + 1) *
						   sizeof(char));

					if (!directories[currDir])
						exit(12);
					strcpy(directories[currDir],
					       argv[i + 1]);
					i++;

				} else {
					directories[currDir] =
					    malloc((strlen(argv[i] + 2) + 1) *
						   sizeof(char));

					if (!directories[currDir])
						exit(12);

					strcpy(directories[currDir],
					       argv[i] + 2);
				}
				currDir += 1;

			} else if (argv[i][1] == 'o') {
				if (strlen(argv[i]) == 2) {
					*output =
					    malloc((strlen(argv[i + 1]) + 1) *
						   sizeof(char));

					if (!(*output))
						exit(12);

					strcpy(*output, argv[i + 1]);
					i++;
				} else {
					*output =
					    malloc((strlen(argv[i] + 2) + 1) *
						   sizeof(char));

					if (!(*output))
						exit(12);

					strcpy(*output, argv[i] + 2);
				}
			}
		} else {
			if (*input == NULL) {
				*input = malloc((strlen(argv[i]) + 1) *
						sizeof(char));

				if (!(*input))
					exit(12);

				strcpy(*input, argv[i]);
			} else if (*output == NULL) {
				*output = malloc((strlen(argv[i]) + 1) *
						 sizeof(char));

				if (!(*output))
					exit(12);

				strcpy(*output, argv[i]);
			} else {
				free(*input);
				free(*output);
				exit(1);
			}
		}
	}
	return directories;
}
