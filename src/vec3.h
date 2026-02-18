#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdlib.h>

typedef struct {
    double x, y, z;
} Vec3;

static inline Vec3 vec3(double x, double y, double z) {
    Vec3 v; v.x = x; v.y = y; v.z = z; return v;
}

static inline Vec3 add(Vec3 a, Vec3 b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline Vec3 sub(Vec3 a, Vec3 b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline Vec3 mul(Vec3 a, Vec3 b) {
    return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

static inline Vec3 scl(Vec3 v, double t) {
    return vec3(v.x * t, v.y * t, v.z * t);
}

static inline double dot(Vec3 a, Vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

static inline Vec3 cross(Vec3 a, Vec3 b) {
    return vec3(
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    );
}

static inline double length(Vec3 v) {
    return sqrt(dot(v,v));
}

static inline Vec3 normalize(Vec3 v) {
    return scl(v, 1.0/length(v));
}

static inline Vec3 reflect(Vec3 v, Vec3 n) {
    // reflect v around normal n
    return sub(v, scl(n, 2*dot(v,n)));
}

#endif // VEC3_H
