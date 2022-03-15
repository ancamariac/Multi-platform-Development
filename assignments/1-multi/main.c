#include "helper.h"

int main(int argc, char **argv)
{
	char *inputFileName = NULL;
	char *outputFileName = NULL;
	HashMap map = createHashMap(6);

	FILE *inFile;
	FILE *outFile;

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

   if (inputFileName) {
      inFile = fopen(inputFileName, "r");
   } else {
      inFile = stdin;
   }

   if (outputFileName) {
      outFile = fopen(outputFileName, "w");
   } else {
      outFile = stdout;
   }

	char ch;

	while ((ch = fgetc(inFile)) != EOF) {
		fputc(ch, outFile);
	}

	//fclose(inFile);
   //free(inputFileName);

	//fclose(outFile);
   //free(outputFileName);

	deleteMap(&map);
   
	
	return 0;
}
