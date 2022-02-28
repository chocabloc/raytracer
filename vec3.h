#pragma once
#include <math.h>
#include <stdint.h>

typedef struct {
    union {
        struct { float x, y, z; };
        struct { float r, g, b; };
    };
} vec3_t;
#define vec3(_x, _y, _z) (vec3_t) { .x = _x, .y = _y, .z = _z }

float  vec3_lensq(vec3_t v);
float  vec3_len(vec3_t v);
float  vec3_dot(vec3_t v1, vec3_t v2);
vec3_t vec3_add(vec3_t v1, vec3_t v2);
vec3_t vec3_sub(vec3_t v1, vec3_t v2);
vec3_t vec3_scale(vec3_t v, float s);
vec3_t vec3_norm(vec3_t v);
vec3_t vec3_cross(vec3_t v1, vec3_t v2);
