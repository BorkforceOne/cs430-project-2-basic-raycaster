//
// Created by Brandon Garling on 9/21/2016.
//
#include "../constants.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "json_parsers.h"

/**
 * Parses a simple JSON string from the file's current position,
 * this only supports simple ASCII characters.
 * @param fh - The file handle to read from
 * @return The string read from the file, or NULL if an error occurred
 */
char* parse_string(FILE *fh) {
	int c;
	int size = INITIAL_BUFFER_SIZE;
	char *buffer = malloc(sizeof(char) * size);

	// Check for a beginning quote "
	c = fgetc(fh);
	if (c != '"') {
		if (LOG_LEVEL > 0)
			fprintf(stderr, "Error: Expected string\n");
		return NULL;
	}

	// Read characters from the file until we find an ending quote "
	int i = 0;
	do {
		// Reallocate space if we don't have enough for this letter + the null terminator
		if (i + 2 > c) {
			size *= 2;
			buffer = realloc(buffer, size * sizeof(char));
		}
		c = fgetc(fh);
		buffer[i++] = c;
	}
	while (c != '"');

	// Add a null termination to the string
	buffer[--i] = '\0';

	// Make a copy of the string only the size that we need
	char* string = strdup(buffer);
	// Free our working buffer
	free(buffer);

	// Return the newly built string
	return string;
}