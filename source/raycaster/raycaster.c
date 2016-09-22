//
// Created by Brandon Garling on 9/21/2016.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "3dmath.h"
#include "raycaster.h"
#include "../json/json_parsers.h"
#include "../json/json.h"
#include "../imaging/imaging.h"
#include "../imaging/ppm.h"

int raycast(JSONArray *JSONSceneArrayRef, int width, int height) {
	Image outputImage;
	Scene scene;
	if (create_scene(JSONSceneArrayRef, &scene) != 0){
		return 1;
	}

	outputImage.width = width;
	outputImage.height = height;
	outputImage.pixmapRef = malloc(sizeof(RGBApixel) * width * height);

	int M = width;
	int N = height;
	double h = scene.camera.height;
	double w = scene.camera.width;

	V3 c;
	c[X] = w/2.0;
	c[Y] = h/2.0;

	double pixel_height = h/M;
	double pixel_width = w/N;

	V3 p;
	V3 Rd; // The direction of our ray
	V3 Ro; // The origin of our ray
	V3 Rh; // The point that we hit

	Primitive *primitiveHit;

	p[Z] = 0;
	for (int i=0; i<M; i++) {
		p[Y] = c[Y] - h/2.0 + pixel_height * (i + 0.5);
		for (int j=0; j<N; j++) {
			p[X] = c[X] - w/2.0 + pixel_width * (j + 0.5);
			v3_normalize(p, Rd); // normalization, ray direction
			v3_copy(p, Ro); // origin of the ray
			primitiveHit = NULL;
			shoot(Ro, Rd, &scene, &primitiveHit); // intersection is x
			shade(primitiveHit, &outputImage.pixmapRef[i*N + j]);
		}
	}

	if (write_ppm_p6_image("out.ppm", &outputImage) != 1) {
		return 1;
	}

	return 0;
}

int shade(Primitive* primitiveHitRef, RGBApixel *pixel) {
	if (primitiveHitRef != NULL) {
		if (primitiveHitRef->type == SPHERE_T) {
			pixel->r = (uint8_t)(primitiveHitRef->data.sphere.color[0]*255);
			pixel->g = (uint8_t)(primitiveHitRef->data.sphere.color[1]*255);
			pixel->b = (uint8_t)(primitiveHitRef->data.sphere.color[2]*255);
			pixel->a = 255;
			return 0;
		}
		if (primitiveHitRef->type == PLANE_T) {
			pixel->r = (uint8_t)(primitiveHitRef->data.plane.color[0]*255);
			pixel->g = (uint8_t)(primitiveHitRef->data.plane.color[1]*255);
			pixel->b = (uint8_t)(primitiveHitRef->data.plane.color[2]*255);
			pixel->a = 255;
			return 0;
		}
	}
	pixel->r = 0;
	pixel->g = 0;
	pixel->b = 0;
	pixel->a = 255;
	return 0;
}

int shoot(V3 Ro, V3 Rd, Scene *sceneRef, Primitive **primitiveHit) {
	Primitive *primitiveTmpRef;
	Plane *planeRef;
	Sphere *sphereRef;
	*primitiveHit = NULL;
	int i = 0;
	double t = INFINITY;
	double t_possible = INFINITY;
	double tmpDouble;
	V3 tmpVector;

	for (i = 0; i < sceneRef->primitivesLength; i++) {
		primitiveTmpRef = sceneRef->primitives[i];
		if (primitiveTmpRef->type == PLANE_T) {
			planeRef = &primitiveTmpRef->data.plane;

			double numerator, denominator;
			v3_dot(planeRef->normal, Rd, &denominator);

			if (denominator == 0) {
				// No intersection!
				continue;
			}

			v3_subtract(Ro, planeRef->position, tmpVector);
			v3_dot(planeRef->normal, tmpVector, &numerator);

			t_possible = -(numerator/denominator);
			if (t_possible < t) {
				t = t_possible;
				*primitiveHit = primitiveTmpRef;
			}
		}
		else if (primitiveTmpRef->type == SPHERE_T) {

		}
	}

	return 0;
}

int JSONArray_to_V3(JSONArray *JSONArrayRef, V3 vector) {
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

		vector[i] = JSONValueTempRef->data.dataNumber;
	}
}

int create_scene(JSONArray *JSONSceneArrayRef, Scene* SceneRef) {
	JSONObject *JSONObjectTempRef;
	JSONValue *JSONValueTempRef;
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

				JSONArray_to_V3(JSONValueTempRef->data.dataArray, SceneRef->primitives[j]->data.sphere.color);

				// Read the position
				if (JSONObject_get_value("position", JSONObjectTempRef, &JSONValueTempRef) != 0) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}
				if (JSONValueTempRef->type != ARRAY_T) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}

				JSONArray_to_V3(JSONValueTempRef->data.dataArray, SceneRef->primitives[j]->data.sphere.position);

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

				JSONArray_to_V3(JSONValueTempRef->data.dataArray, SceneRef->primitives[j]->data.plane.color);

				// Read the position
				if (JSONObject_get_value("position", JSONObjectTempRef, &JSONValueTempRef) != 0) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}
				if (JSONValueTempRef->type != ARRAY_T) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}

				JSONArray_to_V3(JSONValueTempRef->data.dataArray, SceneRef->primitives[j]->data.plane.position);

				// Read the position
				if (JSONObject_get_value("normal", JSONObjectTempRef, &JSONValueTempRef) != 0) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}
				if (JSONValueTempRef->type != ARRAY_T) {
					fprintf(stderr, "Error: Input scene JSON file contains invalid entries\n");
					return 1;
				}

				JSONArray_to_V3(JSONValueTempRef->data.dataArray, SceneRef->primitives[j]->data.plane.normal);
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