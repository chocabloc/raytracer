#pragma once
#include "vec3.h"

typedef struct {
    vec3_t org, dir;
} ray_t;
#define ray(o, d) (ray_t) { .org = (o), .dir = (d) }

// forward declaration
typedef struct _material_t material_t;

// record of intersection of ray with a surface
typedef struct{
    float dist;
    vec3_t normal;
    vec3_t point;
    material_t* mat;
} hitrec_t;

// enumerates types of materials
typedef enum {
    MAT_TYPE_DIFFUSE,
    MAT_TYPE_METALLIC,
    MAT_TYPE_DIELECTRIC,
    MAT_TYPE_EMISSIVE
} mat_type_t;

// defines material of a surface
struct _material_t {
    vec3_t albedo;
    float roughness;
    float ior;
    float emission;
    mat_type_t type;
};

// get position along ray
#define ray_getp(r, t) vec3_add((r).org, vec3_scale((r).dir, t))
