#include "helper.h"
#include <fcntl.h>

// start
FILE *getIncFile(char *fileName, char **directories, int numDir,
		 char *currentDir)
{
	FILE *file;

	char *filePath =
	    malloc((strlen(currentDir) + strlen(fileName) + 1) * sizeof(char));

	if (!filePath) {
		exit(1);
	}

	strcpy(filePath, currentDir);
	strcat(filePath, fileName);

	file = fopen(filePath, "r");

	if (file) {
		free(filePath);
		return file;
	}

	free(filePath);

	for (int i = 0; i < numDir; i++) {
		char *path =
		    malloc((strlen(directories[i]) + strlen(fileName) + 2) *
			   sizeof(char));

		if (!path) {
			exit(1);
		}

		strcpy(path, directories[i]);
		strcat(path, "/");
		strcat(path, fileName);

		if ((file = fopen(path, "r"))) {
			free(path);
			return file;
		}
		free(path);
	}

	return NULL;
}
// end

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
			while (line[i] != '\"') {
				i++;
			}
			i++;
		}
		if (strncmp(&line[i], key, len1) == 0) {
			found = 1;
			break;
		}
	}

	if (found) {
		result = malloc(len_res * sizeof(char));

		if (!result) {
			exit(1);
		}

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
	size_t len = 0;
	int read = 0;
	static const char delimiters[] = "\t []{}<>=+-*/%!&|^.,:;()\\";

	if (done == 1) {
		while ((read = getline(&line, &len, in)) != -1) {
			if (!strncmp(line, "#endif", 6)) {
				free(line);
				return;
			}
		}
		return;
	}

	if (cond == 0) {
		while ((read = getline(&line, &len, in)) != -1) {
			if (!strncmp(line, "#endif", 6)) {
				free(line);
				return;
			} else if (!strncmp(line, "#else", 5)) {
				ifelse(in, out, map, 1, 0);
				free(line);
				return;
			} else if (!strncmp(line, "#elif", 5)) {
				char *token = strtok(line, delimiters);
				token = strtok(NULL, "\n");
				char *val = get(map, token);
				if (val != NULL) {
					token = val;
				}
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
		while ((read = getline(&line, &len, in)) != -1) {
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
	size_t len = 0;
	int read = 0;

	const char delimiters[] = "\t []{}<>=+-*/%!&|^.,:;()\\\n";

	if (cond == 0) {
		while ((read = getline(&line, &len, in)) != -1) {
			if (!strncmp(line, "#endif", 6)) {
				free(line);
				return;
			}
		}
	} else {
		while ((read = getline(&line, &len, in)) != -1) {
			if (!strncmp(line, "#endif", 6)) {
				free(line);
				return;
			} else {
				if (!strncmp(line, "#define", 7)) {
					char *key = strtok(line, delimiters);
					key = strtok(NULL, delimiters);
					char *value = strtok(NULL, "\n");

					if (value) {
						insert(map, key, value);
					} else {
						insert(map, key, "");
					}
				} else if (!strncmp(line, "#undef", 6)) {
					char *key = strtok(line, delimiters);
					key = strtok(NULL, delimiters);
					delete (map, key);
				} else {
					fputs(line, out);
				}
			}
		}
	}
}

char *getDirectory(char *path)
{

	if (!strcmp(path, "")) {
		return path;
	}

	char *fileName = strrchr(path, '/');

	char *dir =
	    malloc(sizeof(char) * (strlen(path) - strlen(fileName + 2)));

	if (!dir) {
		exit(1);
	}

	int i = 0;

	while (path != fileName) {
		dir[i] = path[0];
		i++;
		path++;
	}

	dir[i] = '/';
	dir[i + 1] = '\0';

	return dir;
}

void parseFile(FILE *in, FILE *out, HashMap *map, char **directories,
	       int numDir, char *inFileName)
{
	char *line = NULL;
	size_t len = 0;
	int read = 0;

	char *token = NULL, *key = NULL, *value = NULL;
	const char delimiters[] = "\t []{}<>=+-*/%!&|^.,:;()\\";

	while ((read = getline(&line, &len, in)) != -1) {
		char *line_copy = malloc((strlen(line) + 1) * sizeof(char));

		if (!line_copy) {
			exit(1);
		}

		char *result = malloc((strlen(line) + 1) * sizeof(char));

		if (!result) {
			exit(1);
		}

		strcpy(line_copy, line);
		strcpy(result, line);
		token = strtok(line_copy, delimiters);

		if (!strcmp(token, "#define")) {
			key = strtok(NULL, delimiters);
			value = strtok(NULL, "\n");
			insert(map, key, value);
		} else if (!strcmp(token, "#undef")) {
			key = strtok(NULL, "\n");
			delete (map, key);
		} else if (!strcmp(token, "#if")) {
			token = strtok(NULL, "\n");
			char *val = get(map, token);
			if (val != NULL) {
				token = val;
			}
			if (!strcmp(token, "0")) {
				ifelse(in, out, map, 0, 0);
			} else {
				ifelse(in, out, map, 1, 0);
			}
		} else if (!strcmp(token, "#ifdef")) {
			token = strtok(NULL, "\n");
			if (get(map, token) != NULL) {
				ifdef(in, out, map, 1);
			} else {
				ifdef(in, out, map, 0);
			}
		} else if (!strcmp(token, "#ifndef")) {
			if (get(map, token) != NULL) {
				ifdef(in, out, map, 0);
			} else {
				ifdef(in, out, map, 1);
			}
			// start
		} else if (!strcmp(token, "#include")) {
			token = strtok(line, delimiters);
			token = strtok(NULL, " \"");
			char *inputDir = getDirectory(inFileName);

			FILE *incFile =
			    getIncFile(token, directories, numDir, inputDir);

			if (incFile != NULL) {
				parseFile(incFile, out, map, directories,
					  numDir, inFileName);
				fclose(incFile);
			} else {
				exit(1);
			}

			free(inputDir);
		}
		// end
		else {
			while (token != NULL) {
				value = get(map, token);
				if (value != NULL) {
					result = replace(result, token, value);
				}
				token = strtok(NULL, delimiters);
			}
			fputs(result, out);
		}

		free(line_copy);
		free(result);
	}
	free(line);
}
// start
char **getArgs(int argc, char **argv, char **input, char **output, HashMap *map,
	       int *numDir)
{
	int currDir = 0;
	char **directories = NULL;
	for (int i = 1; i < argc; i++) {
		if (!strncmp(argv[i], "-I", 2)) {
			*numDir += 1;
		}
	}
	if (*numDir != 0) {
		directories = malloc((*numDir) * sizeof(char *));

		if (!directories) {
			exit(1);
		}
	}
	// end
	for (int i = 1; i < argc; i++) {
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
				if (val == NULL) {
					val = "";
				}

				insert(map, key, val);

			} else if (argv[i][1] == 'I') {
				// start
				if (strlen(argv[i]) == 2) {
					directories[currDir] =
					    malloc((strlen(argv[i + 1]) + 1) *
						   sizeof(char));

					if (!directories[currDir]) {
						exit(1);
					}

					strcpy(directories[currDir],
					       argv[i + 1]);
					i++;

				} else {
					directories[currDir] =
					    malloc((strlen(argv[i] + 2) + 1) *
						   sizeof(char));

					if (!directories[currDir]) {
						exit(1);
					}

					strcpy(directories[currDir],
					       argv[i] + 2);
				}
				currDir += 1;
				// end
			} else if (argv[i][1] == 'o') {
				if (strlen(argv[i]) == 2) {
					*output =
					    malloc((strlen(argv[i + 1]) + 1) *
						   sizeof(char));

					if (!output) {
						exit(1);
					}

					strcpy(*output, argv[i + 1]);
					i++;
				} else {
					*output =
					    malloc((strlen(argv[i] + 2) + 1) *
						   sizeof(char));

					if (!output) {
						exit(1);
					}

					strcpy(*output, argv[i] + 2);
				}
			}
		} else {
			if (*input == NULL) {
				*input = malloc((strlen(argv[i]) + 1) *
						sizeof(char));

				if (!input) {
					exit(1);
				}

				strcpy(*input, argv[i]);
			} else if (*output == NULL) {
				*output = malloc((strlen(argv[i]) + 1) *
						 sizeof(char));

				if (!output) {
					exit(1);
				}

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

int main(int argc, char **argv)
{
	char *inputFileName = NULL;
	char *outputFileName = NULL;
	// start
	char **directories = NULL;
	int numDir = 0;
	// end

	HashMap map = createHashMap(6);
	FILE *inFile = NULL;
	FILE *outFile = NULL;
	// start
	directories =
	    getArgs(argc, argv, &inputFileName, &outputFileName, &map, &numDir);
	// end
	if (inputFileName) {
		inFile = fopen(inputFileName, "r");
		if (inFile == NULL) {
			return 1;
		}
	} else {
		inFile = stdin;
	}

	if (outputFileName) {
		outFile = fopen(outputFileName, "w+");
	} else {
		outFile = stdout;
	}

	if (!inputFileName) {
		parseFile(inFile, outFile, &map, directories, numDir, "");
	} else {
		parseFile(inFile, outFile, &map, directories, numDir,
			  inputFileName);
	}

	fflush(inFile);
	fclose(inFile);
	free(inputFileName);

	fflush(outFile);
	fclose(outFile);
	free(outputFileName);

	deleteMap(&map);
	// start
	for (int i = 0; i < numDir; i++) {
		free(directories[i]);
	}
	free(directories);
	// end

	return 0;
}
