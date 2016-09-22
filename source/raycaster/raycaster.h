//
// Created by Brandon Garling on 9/21/2016.
//

#ifndef CS430_PROJECT_2_BASIC_RAYCASTER_RAYTRACER_H
#define CS430_PROJECT_2_BASIC_RAYCASTER_RAYTRACER_H

#include "3dmath.h"

typedef enum PrimitiveType_t {
	CAMERA_T,
	SPHERE_T,
	PLANE_T
} PrimitiveType_t;

typedef struct Ray {
	double o[3];
	double d[3];
} Ray;

typedef struct Camera {
	double width;
	double height;
} Camera;

typedef struct Sphere {
	double radius;
	V3 color;
	V3 position;
} Sphere;

typedef struct Plane {
	V3 color;
	V3 position;
	V3 normal;
} Plane;

typedef struct Primitive {
	PrimitiveType_t type;
	union {
		Camera camera;
		Plane plane;
		Sphere sphere;
	} data;
} Primitive;

typedef struct Scene {
	Camera camera;
	Primitive** primitives;
	int primitivesLength;
} Scene;

typedef struct RGBApixel RGBApixel;
typedef struct JSONArray JSONArray;

int raycast(JSONArray *JSONSceneArrayRef, int width, int height);
int shade(Primitive* primitiveHitRef, RGBApixel *pixel);
int shoot(V3 Ro, V3 Rd, Scene *sceneRef, Primitive **primitiveHit);
int JSONArray_to_V3(JSONArray *JSONArrayRef, V3 vector);
int create_scene(JSONArray *JSONSceneArrayRef, Scene* SceneRef);

#endif //CS430_PROJECT_2_BASIC_RAYCASTER_RAYTRACER_H
