// SPDX-License-Identifier: GPL-2.0-or-later

#include "helper.h"

int main(int argc, char **argv)
{
	char *inputFileName = NULL;
	char *outputFileName = NULL;
	char **directories = NULL;
	int numDir = 0;
	int i = 0;

	HashMap map = createHashMap(6);
	FILE *inFile = NULL;
	FILE *outFile = NULL;

	directories =
	    getArgs(argc, argv, &inputFileName, &outputFileName, &map, &numDir);

	if (inputFileName) {
		inFile = fopen(inputFileName, "r");
		if (inFile == NULL)
			return 1;
	} else {
		inFile = stdin;
	}

	if (outputFileName)
		outFile = fopen(outputFileName, "w+");
	else
		outFile = stdout;

	if (!inputFileName)
		parseFile(inFile, outFile, &map, directories, numDir, "");
	else
		parseFile(inFile, outFile, &map, directories, numDir,
			  inputFileName);

	fflush(inFile);
	fclose(inFile);
	free(inputFileName);

	fflush(outFile);
	fclose(outFile);
	free(outputFileName);

	deleteMap(&map);

	for (i = 0; i < numDir; i++)
		free(directories[i]);
	free(directories);

	return 0;
}
