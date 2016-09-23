#include <stdio.h>
#include "json/json.h"
#include "raycaster/raycaster.h"
#include "imaging/ppm.h"
#include "raycaster/raycaster_helpers.h"

int main() {
	char *inputFname = "resources/input.json";
	char *outputFname = "out.ppm";
	int imageWidth = 1000;
	int imageHeight = 1000;

	// Read the input JSON file
	JSONValue JSONRoot;
	printf("[INFO] Reading input scene file '%s'\n", inputFname);
	if (read_json(inputFname, &JSONRoot) != 0)
		return 1;

	// Convert the JSON file to a scene
	Scene scene;
	printf("[INFO] Creating scene from input scene file\n");
	if (create_scene_from_JSON(&JSONRoot, &scene) != 0)
		return 1;

	// Raycast the scene into an image
	Image image;
	printf("[INFO] Raycasting scene into image\n");
	if (raycast(&scene, &image, imageWidth, imageHeight) != 0)
		return 1;

	// Write the image out to the specified file
	printf("[INFO] Saving image (PPM P6) to output file '%s'\n", outputFname);
	if (save_ppm_p6_image(&image, outputFname) != 0)
		return 1;

	printf("[INFO] Finished!");
	return 0;
}