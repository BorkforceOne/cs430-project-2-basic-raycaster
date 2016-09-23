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
	c[X] = a[X] + b[X];
	c[Y] = a[Y] + b[Y];
	c[Z] = a[Z] + b[Z];
}

static inline void v3_subtract(V3 a,V3 b, V3 c) {
	c[X] = a[X] - b[X];
	c[Y] = a[Y] - b[Y];
	c[Z] = a[Z] - b[Z];
}

static inline void v3_scale(V3 a, double s, V3 c) {
	c[X] = a[X] * s;
	c[Y] = a[Y] * s;
	c[Z] = a[Z] * s;
}

static inline void v3_dot(V3 a, V3 b, double *c) {
	*c = a[X]*b[X] + a[Y]*b[Y] + a[Z]*b[Z];
}

static inline void v3_cross(V3 a, V3 b, V3 c) {
	c[X] = a[Y]*b[Z] - a[Z]*b[Y];
	c[Y] = a[Z]*b[X] - a[X]*b[Z];
	c[Z] = a[X]*b[Y] - a[Y]*b[X];
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
