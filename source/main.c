#include <stdio.h>
#include "json/json.h"

int main() {
	char* inputFname = "resources/input.json";
    printf("Info: Reading input scene file '%s'...\n", inputFname);

	JSONValue JSONRoot;
	return read_json(inputFname, &JSONRoot);
}