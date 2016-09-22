#include <stdio.h>
#include "json/json.h"
#include "raycaster/raycaster.h"

int main() {
	char* inputFname = "resources/input.json";
    printf("Info: Reading input scene file '%s'...\n", inputFname);

	JSONValue JSONRoot;
	if (read_json(inputFname, &JSONRoot) != 0)
		return 1;

	if (raycast(JSONRoot.data.dataArray, 100, 100) != 0)
		return 1;

	return 0;
}