#include "sphere.h"
#include "math.h"
#include "xshift.h"

#define EPSILON 0.0001

/* Calculates if a ray hits a sphere
 Formula:
        |a⃗ - x⃗|² - ((a⃗ - x⃗) ⋅ p̂)² ≤ r²
 where,
     a⃗ = centre of the sphere
     x⃗ = origin of ray
     p̂ = direction of ray
     r = radius of sphere
*/
float sphere_hitdist(sphere_t s, ray_t ray)
{
    vec3_t oc = vec3_sub(s.centre, ray.org);
    float odist = vec3_dot(oc, ray.dir),
          rdist = vec3_lensq(oc) - (odist * odist),
          delta = (s.radius * s.radius) - rdist,
          sqrtd = sqrt(delta);

    // if does not intersect, return -1
    if (delta < 0)
        return -1;
    
    // return the distance along the ray of intersection
    float dista = odist + sqrtd, distb = odist - sqrtd;
    if (dista < EPSILON) return distb;
    else if (distb < EPSILON) return dista;
    else return (distb < dista) ? distb : dista;
}

// calculate hit record for a sphere
void sphere_hitrec(sphere_t* s, hitrec_t* hit, float dist, ray_t ray) {
    hit->dist = dist;
    hit->mat = &(s->mat);
    hit->point = ray_getp(ray, dist);
    hit->normal = vec3_scale(vec3_sub(hit->point, s->centre), 1 / s->radius);
}

// returns a random vector in a unit sphere
vec3_t rand_in_sphere()
{
    vec3_t out;
    do{
        out = vec3(2 * xs_get() - 1, 2 * xs_get() - 1, 2 * xs_get() - 1);
    } while(vec3_lensq(out) >= 1);
    return out;
}
