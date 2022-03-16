#include "helper.h"
#include <fcntl.h>

char* replace(char* line, char* key, char* value) {
	int len = strlen(line);
	int len1 = strlen(key);
	int len2 = strlen(value);
	int len_res = len + len2 - len1 + 1;
	int found = 0;
	char* result;
	int i;

	for (i = 0; i < len; i++) {
		if (line[i] == '\"') {
			i++;
			while(line[i] != '\"') {
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

		strncpy(result, line, i);
		strncpy(result + i, value, len2);
		strcpy(result + i + len2, line + i + len1);
		free(line);
		return result; 
	} else {
		//strcpy(result, line);
		return line;
	}

	free(line);
	return result;
}

void parseFile(FILE *in, FILE *out, HashMap *map)
{
	char *line = NULL;
	size_t len = 0;
	int read = 0;

	char *token = NULL, *key = NULL, *value = NULL;
	const char delimiters[] = " ();{}/," ;
	//char *result = NULL;

	while ((read = getline(&line, &len, in)) != -1) {
		char* line_copy = malloc(strlen(line) * sizeof(char));
		char* result = malloc(strlen(line) *sizeof(char));
		strcpy(line_copy, line);
		strcpy(result, line); 
		token = strtok(line_copy, delimiters);

		if (!strcmp(token, "#define")) {
			key = strtok(NULL, delimiters); // se ia VAR
			value = strtok(NULL, "\n"); // se ia 1
			insert(map, key, value);
		} else {
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

int main(int argc, char **argv)
{
	char *inputFileName = NULL;
	char *outputFileName = NULL;
	HashMap map = createHashMap(6);

	FILE *inFile = NULL;
	FILE *outFile = NULL;

	for (int i = 1; i < argc; i++) {
		if (argv[i] && argv[i][0] == '-') {
			if (argv[i][1] == 'D') {
				if (strlen(argv[i]) == 2) {
					if (argv[i + 2] &&
					    argv[i + 1][0] != '-') {
						insert(&map, argv[i + 1],
						       argv[i + 2]);
					} else {
						insert(&map, argv[i + 1], "");
						i--;
					}
					i += 2;
				} else {

					if (argv[i + 1] &&
					    argv[i + 1][0] != '-') {
						insert(&map, argv[i] + 2,
						       argv[i + 1]);

					} else {
						insert(&map, argv[i] + 2, "");
						i--;
					}
					i += 1;
				}
			} else if (argv[i][1] == 'I') {
				// to do
			} else if (argv[i][1] == 'o') {
				if (strlen(argv[i]) == 2) {
					outputFileName =
					    malloc((strlen(argv[i + 1]) + 1) *
						   sizeof(char));
					strcpy(outputFileName, argv[i + 1]);
					i++;
				} else {
					outputFileName =
					    malloc((strlen(argv[i] + 2) + 1) *
						   sizeof(char));
					strcpy(outputFileName, argv[i] + 2);
				}
			}
		} else {
			inputFileName =
			    malloc((strlen(argv[i]) + 1) * sizeof(char));
			strcpy(inputFileName, argv[i]);
		}
	}

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


	parseFile(inFile, outFile, &map);

	fflush(inFile);
	fclose(inFile);
	free(inputFileName);

	fflush(outFile);
	fclose(outFile);
	free(outputFileName);

	deleteMap(&map);

	return 0;
}
