#include <stdio.h>
#include <stdlib.h>
#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "sphere.h"
#include "material.h"

#define MAX_HITTABLES 100

int world_hit(hittable *world, int n, Ray *r, double t_min, double t_max, hit_record *rec) {
    hit_record temp_rec;
    int hit_anything = 0;
    double closest_so_far = t_max;
    for (int i = 0; i < n; i++) {
        if (world[i].hit(world[i].obj, r, t_min, closest_so_far, &temp_rec)) {
            hit_anything = 1;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }
    return hit_anything;
}

Vec3 ray_color(Ray *r, hittable *world, int world_size, int depth) {
    if (depth <= 0) return vec3(0,0,0);
    hit_record rec;
    if (world_hit(world, world_size, r, 0.001, 1e9, &rec)) {
        Ray scattered;
        Vec3 attenuation;
        if (scatter(rec.mat, r, &rec, &attenuation, &scattered)) {
            Vec3 c = ray_color(&scattered, world, world_size, depth-1);
            return mul(attenuation, c);
        }
        return vec3(0,0,0);
    }
    Vec3 unit_direction = normalize(r->dir);
    double t = 0.5*(unit_direction.y + 1.0);
    return add(scl(vec3(1.0,1.0,1.0), 1.0 - t), scl(vec3(0.5,0.7,1.0), t));
}

int main() {
    // image
    const int image_width = 800;
    const int image_height = 450;
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // world
    hittable world[MAX_HITTABLES];
    int world_size = 0;

    // materials
    struct material *ground_mat = malloc(sizeof(struct material));
    ground_mat->type = 0; // diffuse (grass-like)
    ground_mat->albedo = vec3(0.4,0.8,0.4); // greenish

    struct material *center_mat = malloc(sizeof(struct material));
    center_mat->type = 0;
    center_mat->albedo = vec3(0.1,0.2,0.5);

    struct material *left_mat = malloc(sizeof(struct material));
    left_mat->type = 2;
    left_mat->ref_idx = 1.5;

    struct material *right_mat = malloc(sizeof(struct material));
    right_mat->type = 1;
    right_mat->albedo = vec3(0.8,0.6,0.2);
    right_mat->fuzz = 0.0;

    Sphere *s0 = malloc(sizeof(Sphere));
    s0->center = vec3(0.0,-100.5,-1.0);
    s0->radius = 100.0;
    s0->mat = ground_mat;
    world[world_size++] = (hittable){s0, sphere_hit};

    Sphere *s1 = malloc(sizeof(Sphere));
    s1->center = vec3(0.0,0.0,-1.0);
    s1->radius = 0.5;
    s1->mat = center_mat;
    world[world_size++] = (hittable){s1, sphere_hit};

    Sphere *s2 = malloc(sizeof(Sphere));
    s2->center = vec3(-1.0,0.0,-1.0);
    s2->radius = 0.5;
    s2->mat = left_mat;
    world[world_size++] = (hittable){s2, sphere_hit};

    Sphere *s3 = malloc(sizeof(Sphere));
    s3->center = vec3(1.0,0.0,-1.0);
    s3->radius = 0.5;
    s3->mat = right_mat;
    world[world_size++] = (hittable){s3, sphere_hit};

    // Example: add another sphere (e.g. red diffuse) by allocating a new
    // material and sphere, then pushing to the world array:
    //
    // struct material *red = malloc(sizeof *red);
    // red->type = 0;
    // red->albedo = vec3(0.9, 0.1, 0.1);
    // Sphere *s4 = malloc(sizeof *s4);
    // s4->center = vec3(0.0, 0.5, -1.5);
    // s4->radius = 0.3;
    // s4->mat = red;
    // world[world_size++] = (hittable){s4, sphere_hit};

    // camera
    Vec3 origin = vec3(0,0,0);
    double viewport_height = 2.0;
    double viewport_width = (double)image_width / image_height * viewport_height;
    double focal_length = 1.0;

    Vec3 horizontal = vec3(viewport_width, 0, 0);
    Vec3 vertical = vec3(0, viewport_height, 0);
    Vec3 lower_left_corner = sub(sub(sub(origin, scl(horizontal,0.5)), scl(vertical,0.5)), vec3(0,0,focal_length));

    FILE *f = fopen("output.ppm", "w");
    fprintf(f, "P3\n%d %d\n255\n", image_width, image_height);
    for (int j = image_height-1; j >= 0; j--) {
        for (int i = 0; i < image_width; i++) {
            Vec3 color = vec3(0,0,0);
            for (int s = 0; s < samples_per_pixel; s++) {
                double u = (i + random_double()) / (image_width-1);
                double v = (j + random_double()) / (image_height-1);
                Ray r = ray(origin, sub(add(add(lower_left_corner, scl(horizontal,u)), scl(vertical,v)), origin));
                color = add(color, ray_color(&r, world, world_size, max_depth));
            }
            // write color
            double scale = 1.0 / samples_per_pixel;
            int ir = (int)(255.999 * sqrt(color.x * scale));
            int ig = (int)(255.999 * sqrt(color.y * scale));
            int ib = (int)(255.999 * sqrt(color.z * scale));
            fprintf(f, "%d %d %d\n", ir, ig, ib);
        }
    }
    fclose(f);

    return 0;
}
