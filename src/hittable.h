#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"

struct material;

typedef struct {
    Vec3 p;
    Vec3 normal;
    double t;
    int front_face;
    struct material *mat;
} hit_record;

static inline void set_face_normal(hit_record *rec, Ray *r, Vec3 outward_normal) {
    rec->front_face = dot(r->dir, outward_normal) < 0;
    rec->normal = rec->front_face ? outward_normal : scl(outward_normal, -1);
}

typedef struct hittable {
    void *obj;
    int (*hit)(void *obj, Ray *r, double t_min, double t_max, hit_record *rec);
} hittable;

#endif // HITTABLE_H
