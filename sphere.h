/*
    Temporary sphere type before we have triangles and meshes
*/

#pragma once
#include "vec3.h"
#include "ray.h"

typedef struct {
    vec3_t centre;
    float radius;
    material_t mat;
} sphere_t;
#define sphere(c, r) (sphere_t) { .centre = (c), .radius = (r) }

// Calculates if a ray hits a sphere
float sphere_hitdist(sphere_t s, ray_t ray);
void sphere_hitrec(sphere_t* s, hitrec_t* hit, float dist, ray_t ray);

// gives a random vector inside the unit sphere
vec3_t rand_in_sphere();
