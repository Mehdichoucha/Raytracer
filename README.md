# Raytracer in C

This project is a simple 3D ray tracer implemented in C, demonstrating basic
ray tracing features including:

- Spheres and planes
- Reflective and refractive materials
- Textured and reflective surfaces (via material albedos)
- Recursive ray tracing with reflections and refractions

**Tip:** edit the constants `image_width`/`image_height` in `src/main.c` to enlarge the output image.

The output is generated as a PPM image that can be viewed with most image
viewers.

## Building

A simple `Makefile` is provided. Run:

```sh
make
```

The resulting executable is `raytracer`. Running it will produce `output.ppm`.

## Usage

```sh
./raytracer
```

## Notes

The scene is hard-coded with four spheres: a grassy ground, a diffuse
center sphere, a glass sphere on the left, and a metallic sphere on the
right. You can modify `src/main.c` to add more objects or materials.

### Adding another sphere

Find the `// world` section near the top of `src/main.c` and add another
`Sphere` allocation and initialization. For example, to place a red diffuse
sphere above the center:

```c
struct material *red = malloc(sizeof *red);
red->type = 0;
red->albedo = vec3(0.9, 0.1, 0.1);

Sphere *s4 = malloc(sizeof *s4);
s4->center = vec3(0.0, 0.5, -1.5);
s4->radius = 0.3;
s4->mat = red;
world[world_size++] = (hittable){s4, sphere_hit};
```

Recompile with `make` and rerun `./raytracer` to see the new sphere.
