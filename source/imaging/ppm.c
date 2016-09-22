//
// Created by Brandon Garling on 9/20/2016.
//

#include <stdlib.h>
#include <stdio.h>
#include "imaging.h"

/**
 * Reads a ppm (P6|P3) image into a Image struct
 * @param fname - The ppm (P6|P3) image filename to load
 * @param imageRef - A pointer to the struct to read the ppm file image data into
 * @return 0 if success, otherwise a failure occurred
 */
int read_ppm_image(char *fname, Image *imageRef) {
	FILE *fp = fopen(fname, "r");
	if (fp) {
		// Read the magic number
	} else {
		fprintf(stderr, "Error: File '%s' could not be opened for reading\n", fname);
		return 1;
	}

	return 0;
}