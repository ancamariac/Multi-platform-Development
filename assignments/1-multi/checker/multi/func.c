#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {

    const char *line;
    const char *key; 
    const char *val;
    
    char *result;
	int i = 0, count = 0;
	int newWlen = strlen(val);
	int oldWlen = strlen(key);

	// Counting the number of times old word
	// occur in the string
	for (i = 0; line[i] != '\0'; i++) {
		if (strstr(&line[i], key) == &line[i]) {
			count++;

			// Jumping to index after the old word.
			i += oldWlen - 1;
		}
	}

	// Making new string of enough length
	result = (char *)malloc(i + count * (newWlen - oldWlen) + 1);

	i = 0;
	while (*line) {
		// compare the substring with the result
		if (strstr(line, key) == line) {
			strcpy(&result[i], val);
			i += newWlen;
			line += oldWlen;
		} else
			result[i++] = *line++;
	}

	result[i] = '\0';
	return result;
}