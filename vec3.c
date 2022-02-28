#include "vec3.h"

float vec3_lensq(vec3_t v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float vec3_len(vec3_t v)
{
    return sqrt(vec3_lensq(v));
}

vec3_t vec3_add(vec3_t v1, vec3_t v2)
{
    return (vec3_t) {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
        .z = v1.z + v2.z
    };
}

vec3_t vec3_sub(vec3_t v1, vec3_t v2)
{
    return (vec3_t) {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
        .z = v1.z - v2.z
    };
}

vec3_t vec3_scale(vec3_t v, float s)
{
    return (vec3_t) {
        .x = v.x * s,
        .y = v.y * s,
        .z = v.z * s
    };
}

vec3_t vec3_norm(vec3_t v)
{
    return vec3_scale(v, 1 / vec3_len(v));
}

float vec3_dot(vec3_t v1, vec3_t v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3_t vec3_cross(vec3_t v1, vec3_t v2)
{
    return (vec3_t) {
        .x = ((v1).y * (v2).z) - ((v2).y * (v1).z),
        .y = ((v2).x * (v1).z) - ((v1).x * (v2).z),
        .z = ((v1).x * (v2).y) - ((v2).x * (v1).y)
    };
}
