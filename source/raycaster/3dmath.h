//
// Created by Brandon Garling on 9/21/2016.
//

#ifndef CS430_PROJECT_2_BASIC_RAYCASTER_3DMATH_H
#define CS430_PROJECT_2_BASIC_RAYCASTER_3DMATH_H

#include <math.h>

#define X 0
#define Y 1
#define Z 2

typedef double V3[3];

static inline void v3_add(V3 a,V3 b, V3 c) {
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
	c[2] = a[2] + b[2];
}

static inline void v3_subtract(V3 a,V3 b, V3 c) {
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
	c[2] = a[2] - b[2];
}

static inline void v3_scale(V3 a, double s, V3 c) {
	c[0] = a[0] * s;
	c[1] = a[1] * s;
	c[2] = a[2] * s;
}

static inline void v3_dot(V3 a, V3 b, double *c) {
	*c = a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

static inline void v3_cross(V3 a, V3 b, V3 c) {
	c[0] = a[1]*b[2] - a[2]*b[1];
	c[1] = a[2]*b[0] - a[0]*b[2];
	c[2] = a[0]*b[1] - a[1]*b[0];
}

static inline void v3_magnitude(V3 a, double *b) {
	*b = sqrt(a[X]*a[X] + a[Y]*a[Y] + a[Z]*a[Z]);
}

static inline void v3_normalize(V3 a, V3 b) {
	double scale;
	v3_magnitude(a, &scale);
	v3_scale(a, 1/scale, b);
}

static inline void v3_copy(V3 a, V3 b) {
	b[X] = a[X];
	b[Y] = a[Y];
	b[Z] = a[Z];
}

#endif //CS430_PROJECT_2_BASIC_RAYCASTER_3DMATH_H
