#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct {
    Vec3 origin;
    Vec3 dir;
} Ray;

static inline Ray ray(Vec3 origin, Vec3 dir) {
    Ray r; r.origin = origin; r.dir = dir; return r;
}

static inline Vec3 at(Ray *r, double t) {
    return add(r->origin, scl(r->dir, t));
}

#endif // RAY_H
