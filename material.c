#include "material.h"
#include "sphere.h"
#include "xshift.h"

// calculate schlick's approximation to the fresnel equations
static float schlick(float n1, float n2, float cos_ti, float cos_tr){
    float cosine = (n1 > n2) ? cos_tr : cos_ti,
          ro     = pow((n1 - n2) / (n1 + n2), 2);
    return ro + (1 - ro) * pow(1 - cosine, 5);
}

// scatter a ray after hitting a surface
bool mat_scatter(ray_t* rin, ray_t* rout, vec3_t* factor, const hitrec_t* hit)
{
    vec3_t target;
    bool ret;

    switch(hit->mat->type) {
        case MAT_TYPE_DIFFUSE: {
            target = vec3_norm(vec3_add(rand_in_sphere(), hit->normal));
            *factor = hit->mat->albedo;
            ret = true;
        } break;
        case MAT_TYPE_METALLIC: {
            float tmp = 2 * vec3_dot(rin->dir, hit->normal);
            vec3_t rfactor = vec3_scale(rand_in_sphere(), hit->mat->roughness);
            target = vec3_sub(rin->dir, vec3_scale(hit->normal, tmp));
            target = vec3_norm(vec3_add(target, rfactor));
            *factor = hit->mat->albedo;
            ret = true;
        } break;
        case MAT_TYPE_DIELECTRIC: {
            vec3_t norm; float mew, n1, n2;

            // are we going from object to air or air to object
            if (vec3_dot(rin->dir, hit->normal) > 0){
                mew = hit->mat->ior;
                n1 = mew; n2 = 1;
                norm = vec3_scale(hit->normal, -1);
            } else {
                mew = 1 / hit->mat->ior;
                n1 = 1; n2 = 1 / mew;
                norm = hit->normal;
            }

            // precompute some constants
            float cos_ti = -vec3_dot(rin->dir, norm),
                  sinsq_tr = (mew * mew) * (1 - cos_ti * cos_ti),
                  cos_tr = sqrt(1 - sinsq_tr), reflect;

            // calculate reflection probablity
            if (sinsq_tr > 1) reflect = 1;
            else reflect = schlick(n1, n2, cos_ti, cos_tr);

            // choose between reflection and refraction
            // based on reflectance
            if (xs_get() <= reflect) {
                float tmp = 2 * vec3_dot(rin->dir, norm);
                target = vec3_sub(rin->dir, vec3_scale(norm, tmp));
                *factor = vec3(1.0, 1.0, 1.0);
            } else {
                vec3_t r_para = vec3_scale(rin->dir, mew),
                       r_orth = vec3_scale(norm, (mew * cos_ti) - cos_tr);
                target = vec3_add(r_para, r_orth);
                *factor = hit->mat->albedo;
            }

            // apply roughness
            vec3_t rfactor = vec3_scale(rand_in_sphere(), hit->mat->roughness);
            target = vec3_norm(vec3_add(target, rfactor));
        } break;
        case MAT_TYPE_EMISSIVE: {
            *factor = vec3_scale(hit->mat->albedo, hit->mat->emission);
            ret = false;
        } break;
    }

    *rout = ray(hit->point, target);
    return ret;
}
