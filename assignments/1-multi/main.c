#include "helper.h"

int main(int argc, char **argv)
{
	char *inputFileName = NULL;
	char *outputFileName = NULL;
	HashMap map = createHashMap(6);

	FILE *inFile = fopen(inputFileName, "r");
	FILE *outFile = fopen(outputFileName, "w");

	if (inFile == NULL) {
		exit(EXIT_FAILURE);
	}

	if (outFile == NULL) {
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < argc; i++) {
		if (argv[i][0] == '-') {
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

   if (!inputFileName) {
      inputFileName = malloc(strelen("STDIN") * sizeof(char));
      strcpy(inputFileName, "STDIN");
   }

   if (!outputFileName) {
      outputFileName = malloc(strelen("STDOUT") * sizeof(char));
      strcpy(outputFileName, "STDOUT");
   }

	char ch;

	while ((ch = fgetc(inFile)) != EOF) {
		fputc(ch, outFile);
	}

	fclose(inFile);
   free(inputFileName);

	fclose(outFile);
   free(outputFileName);
   
	deleteMap(&map);
   
	printf("%s\n", inputFileName);
	printf("%s\n", outputFileName);
	printf("%s\n", get(&map, "var0"));

	return 0;
}
