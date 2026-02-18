#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include <math.h>

typedef struct {
    Vec3 center;
    double radius;
    struct material *mat;
} Sphere;

int sphere_hit(void *obj, Ray *r, double t_min, double t_max, hit_record *rec) {
    Sphere *s = (Sphere*)obj;
    Vec3 oc = sub(r->origin, s->center);
    double a = dot(r->dir, r->dir);
    double half_b = dot(oc, r->dir);
    double c = dot(oc, oc) - s->radius*s->radius;
    double discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return 0;
    double sqrtd = sqrt(discriminant);
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || root > t_max)
            return 0;
    }
    rec->t = root;
    rec->p = at(r, rec->t);
    Vec3 outward_normal = scl(sub(rec->p, s->center), 1.0/s->radius);
    set_face_normal(rec, r, outward_normal);
    rec->mat = s->mat;
    return 1;
}

#endif // SPHERE_H
