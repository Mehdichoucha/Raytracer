#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include <stdlib.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// forward
struct material {
    int type; // 0=lambertian,1=metal,2=dielectric
    Vec3 albedo;
    double fuzz;
    double ref_idx;
};

static inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

static inline Vec3 random_in_unit_sphere() {
    while (1) {
        Vec3 p = vec3(random_double(), random_double(), random_double());
        p = sub(scl(p, 2.0), vec3(1,1,1));
        if (dot(p,p) >= 1) continue;
        return p;
    }
}

static inline Vec3 random_unit_vector() {
    double a = random_double() * 2*M_PI;
    double z = random_double() * 2 - 1;
    double r = sqrt(1 - z*z);
    return vec3(r*cos(a), r*sin(a), z);
}

static inline int scatter_lambertian(struct material *m, Ray *r_in, hit_record *rec, Vec3 *attenuation, Ray *scattered) {
    Vec3 scatter_dir = add(rec->normal, random_unit_vector());
    if (fabs(scatter_dir.x) < 1e-8 && fabs(scatter_dir.y) < 1e-8 && fabs(scatter_dir.z) < 1e-8)
        scatter_dir = rec->normal;
    *scattered = ray(rec->p, scatter_dir);
    *attenuation = m->albedo;
    return 1;
}

static inline int scatter_metal(struct material *m, Ray *r_in, hit_record *rec, Vec3 *attenuation, Ray *scattered) {
    Vec3 reflected = reflect(normalize(r_in->dir), rec->normal);
    *scattered = ray(rec->p, add(reflected, scl(random_in_unit_sphere(), m->fuzz)));
    *attenuation = m->albedo;
    return dot(scattered->dir, rec->normal) > 0;
}

static inline double schlick(double cosine, double ref_idx) {
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow(1-cosine,5);
}

static inline int scatter_dielectric(struct material *m, Ray *r_in, hit_record *rec, Vec3 *attenuation, Ray *scattered) {
    *attenuation = vec3(1.0,1.0,1.0);
    double etai_over_etat = rec->front_face ? (1.0 / m->ref_idx) : m->ref_idx;
    Vec3 unit_direction = normalize(r_in->dir);
    double cos_theta = fmin(dot(scl(unit_direction,-1), rec->normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
    int cannot_refract = etai_over_etat * sin_theta > 1.0;
    Vec3 direction;
    if (cannot_refract || schlick(cos_theta, etai_over_etat) > random_double()) {
        direction = reflect(unit_direction, rec->normal);
    } else {
        // refract
        double cos_i = dot(scl(unit_direction,-1), rec->normal);
        double discriminant = 1.0 - etai_over_etat*etai_over_etat*(1 - cos_i*cos_i);
        direction = add(scl(unit_direction, etai_over_etat), scl(rec->normal, etai_over_etat * cos_i - sqrt(discriminant)));
    }
    *scattered = ray(rec->p, direction);
    return 1;
}

static inline int scatter(struct material *m, Ray *r_in, hit_record *rec, Vec3 *attenuation, Ray *scattered) {
    if (m->type == 0)
        return scatter_lambertian(m, r_in, rec, attenuation, scattered);
    else if (m->type == 1)
        return scatter_metal(m, r_in, rec, attenuation, scattered);
    else if (m->type == 2)
        return scatter_dielectric(m, r_in, rec, attenuation, scattered);
    return 0;
}

#endif // MATERIAL_H
