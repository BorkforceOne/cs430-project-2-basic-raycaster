//
// Created by Brandon Garling on 9/22/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "3dmath.h"
#include "raycaster.h"

/**
 * Converts a JSONArray to a V3 vector with error checking
 * @param JSONArrayRef - A reference to an JSONArray which contains 3 number elements
 * @param vectorRef - The vector to write into
 * @return 0 if success, otherwise a failure occurred
 */
int JSONArray_to_V3(JSONArray *JSONArrayRef, V3 *vectorRef) {
	JSONValue *JSONValueTempRef;

	if (JSONArrayRef->length != 3){
		fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
		return 1;
	}

	int i = 0;
	for (i = 0; i < 3; i ++) {
		if (JSONArray_get_value(i, JSONArrayRef, &JSONValueTempRef) != 0) {
			fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
			return 1;
		}

		if (JSONValueTempRef->type != NUMBER_T) {
			fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
			return 1;
		}

		if (i == 0)
			vectorRef->X = JSONValueTempRef->data.dataNumber;
		else if (i == 1)
			vectorRef->Y = JSONValueTempRef->data.dataNumber;
		else if (i == 2)
			vectorRef->Z = JSONValueTempRef->data.dataNumber;
	}

	return 0;
}

/**
 * Populates a scene based on the input JSONRootValue
 * @param JSONValueSceneRef - The JSON value containing a JSONArray to be used to populate the scene
 * @param SceneRef - A reference to the scene to populate
 * @return 0 if success, otherwise a failure occurred
 */
int create_scene_from_JSON(JSONValue *JSONValueSceneRef, Scene* SceneRef) {
	JSONObject *JSONObjectTempRef;
	JSONValue *JSONValueTempRef;
	JSONArray *JSONSceneArrayRef;

	// Make sure that we were passed a JSONArray
	if (JSONValueSceneRef->type != ARRAY_T) {
		fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
		return 1;
	}
	JSONSceneArrayRef = JSONValueSceneRef->data.dataArray;

	int size = JSONSceneArrayRef->length - 1;
	SceneRef->primitives = malloc(sizeof(Primitive*) * size);
	SceneRef->primitivesLength = size;

	int j = 0;
	for (int i = 0; i < JSONSceneArrayRef->length; i++) {
		// Look at the objects we loaded in JSON
		if (JSONSceneArrayRef->values[i]->type == OBJECT_T) {
			// Everything should have a type
			JSONObjectTempRef = JSONSceneArrayRef->values[i]->data.dataObject;

			if (JSONObject_get_value("type", JSONObjectTempRef, &JSONValueTempRef) != 0) {
				fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
				return 1;
			}
			if (JSONValueTempRef->type != STRING_T) {
				fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
				return 1;
			}


			if (strcmp(JSONValueTempRef->data.dataString, "camera") == 0) {
				// We found a camera

				// Read the height
				if (JSONObject_get_value("height", JSONObjectTempRef, &JSONValueTempRef) != 0) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}
				if (JSONValueTempRef->type != NUMBER_T) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}

				SceneRef->camera.height = JSONValueTempRef->data.dataNumber;

				// Read the width

				if (JSONObject_get_value("width", JSONObjectTempRef, &JSONValueTempRef) != 0) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}
				if (JSONValueTempRef->type != NUMBER_T) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}

				SceneRef->camera.width = JSONValueTempRef->data.dataNumber;
			}
			else if (strcmp(JSONValueTempRef->data.dataString, "sphere") == 0) {
				// We found a sphere
				SceneRef->primitives[j] = malloc(sizeof(Primitive));
				SceneRef->primitives[j]->type = SPHERE_T;

				// Read the color
				if (JSONObject_get_value("color", JSONObjectTempRef, &JSONValueTempRef) != 0) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}
				if (JSONValueTempRef->type != ARRAY_T) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}

				JSONArray_to_V3(JSONValueTempRef->data.dataArray, &SceneRef->primitives[j]->data.sphere.color);

				// Read the position
				if (JSONObject_get_value("position", JSONObjectTempRef, &JSONValueTempRef) != 0) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}
				if (JSONValueTempRef->type != ARRAY_T) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}

				JSONArray_to_V3(JSONValueTempRef->data.dataArray, &SceneRef->primitives[j]->data.sphere.position);

				// Read the radius
				if (JSONObject_get_value("radius", JSONObjectTempRef, &JSONValueTempRef) != 0) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}
				if (JSONValueTempRef->type != NUMBER_T) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}

				SceneRef->primitives[j]->data.sphere.radius = JSONValueTempRef->data.dataNumber;
				j++;
			}
			else if (strcmp(JSONValueTempRef->data.dataString, "plane") == 0) {
				// We found a plane
				SceneRef->primitives[j] = malloc(sizeof(Primitive));
				SceneRef->primitives[j]->type = PLANE_T;

				// Read the color
				if (JSONObject_get_value("color", JSONObjectTempRef, &JSONValueTempRef) != 0) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}
				if (JSONValueTempRef->type != ARRAY_T) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}

				JSONArray_to_V3(JSONValueTempRef->data.dataArray, &SceneRef->primitives[j]->data.plane.color);

				// Read the position
				if (JSONObject_get_value("position", JSONObjectTempRef, &JSONValueTempRef) != 0) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}
				if (JSONValueTempRef->type != ARRAY_T) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}

				JSONArray_to_V3(JSONValueTempRef->data.dataArray, &SceneRef->primitives[j]->data.plane.position);

				// Read the position
				if (JSONObject_get_value("normal", JSONObjectTempRef, &JSONValueTempRef) != 0) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}
				if (JSONValueTempRef->type != ARRAY_T) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}

				JSONArray_to_V3(JSONValueTempRef->data.dataArray, &SceneRef->primitives[j]->data.plane.normal);
				j++;
			}
			else {
				fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
				return 1;
			}
		}
		else {
			fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
			return 1;
		}
	}

	return 0;
}