#include "ray.h"
#include "window.h"
#include "sphere.h"
#include "xshift.h"
#include "material.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 400
#define NUM_SAMPLES 64
#define MAX_BOUNCES 10

// hits closer than this value will be ignored
// removes float precision artifacts
#define EPSILON 0.0001

// color constants
static const vec3_t COLOR_WHITE = vec3(1.0, 1.0, 1.0);
static const vec3_t COLOR_LBLUE = vec3(0.5, 0.7, 1.0);
static const vec3_t COLOR_RED   = vec3(0.8, 0.4, 0.4);

// camera constants
static const vec3_t ORIGIN     = vec3(0.0, 0.0, 0.0);
static const vec3_t LOWER_LEFT = vec3(-2.0, -1.0, -1.0);
static const vec3_t CAMERA_X   = vec3(4.0, 0.0, 0.0);
static const vec3_t CAMERA_Y   = vec3(0.0, 2.0, 0.0);

// array of spheres to be rendered
// will eventually replaced with meshes
static sphere_t spheres[20] = {
    [0] = {
        .centre = { 0, -1000.5, -1 },
        .radius = 1000,
        .mat = {
            .type = MAT_TYPE_DIFFUSE,
            .albedo = vec3(0.8, 0.8, 0.8)
        }
    }
};
#define NUM_SPHERES (sizeof(spheres) / sizeof(sphere_t))

/*
   Calculates final color for a ray
*/
static vec3_t get_color(ray_t ray, int bounces)
{
    // check if ray hits any of the spheres
    // and get the intersection which is closest
    float mindist = FLT_MAX;
    int mini = -1;
    for(int i = 0; i < NUM_SPHERES; i++){
        float dist = sphere_hitdist(spheres[i], ray);
        if(dist < EPSILON) continue;
        if(dist < mindist){
            mindist = dist;
            mini = i;
        }
    }

    // return sky if no collision or too many bounces
    if (mini == -1 || bounces > MAX_BOUNCES){
        float t = 0.5 * (ray.dir.y + 1.0);
        return vec3_add(vec3_scale(COLOR_WHITE, 1 - t),
                        vec3_scale(COLOR_LBLUE, t));    // lerp from blue to white*/
    }

    // calculate ray intersection information
    hitrec_t hit;
    sphere_hitrec(&spheres[mini], &hit, mindist, ray);

    // calculate where the ray should go next based on the material
    ray_t newray; vec3_t factor, color;
    if(!mat_scatter(&ray, &newray, &factor, &hit)) {
        return factor;
    }

    // mix the material color and the incident color
    color = get_color(newray, bounces + 1);
    return vec3(color.r * factor.r, color.g * factor.g, color.b * factor.b);
}

int main(void)
{
    // seed the prng with current time
    struct timespec tm;
    clock_gettime(CLOCK_MONOTONIC, &tm);
    xs_seed(tm.tv_sec);

    int i = 1;
    for(float z = 1; z < 11; z+=0.5) {
        for(float x = -z; x <= z; x+=0.5){
            spheres[i] = (sphere_t) {
                .centre = { x, -0.275, -z },
                .radius = 0.225,
            };
            spheres[i].mat.albedo = vec3(xs_get(), xs_get(), xs_get());
            spheres[i].mat.roughness = xs_get() / 5;
            spheres[i].mat.ior = 1.532;
            spheres[i].mat.emission = xs_get() + 2;

            float flip = xs_get();
            if (flip < 0.33) {
                spheres[i].mat.type = MAT_TYPE_DIFFUSE;
            } else if (flip < 0.66) {
                spheres[i].mat.type = MAT_TYPE_DIELECTRIC;
            } else {
                spheres[i].mat.type = MAT_TYPE_METALLIC;
            }
            i++;
            if(i >= 20) break;
        }
    }

    // create the window
    win_init(SCREEN_WIDTH, SCREEN_HEIGHT);

    // loop through pixels
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            vec3_t color = vec3(0, 0, 0);

            // shoot NUM_SAMPLES rays randomly through the pixel
            for(int i = 0; i < NUM_SAMPLES; i++){
                // randomize and scale coordinates
                float xn = ((float)x + xs_get()) / SCREEN_WIDTH;
                float yn = ((float)y + xs_get()) / SCREEN_HEIGHT;

                // get color of ray
                ray_t ray = {
                    .org = ORIGIN,
                    .dir = vec3_norm(vec3_add(LOWER_LEFT, vec3_add(vec3_scale(CAMERA_X, xn),
                                                                   vec3_scale(CAMERA_Y, yn))))
                };
                color = vec3_add(color, get_color(ray, 0));
            }
            // average out the colors over all samples
            color = vec3_scale(color, 1.0f / NUM_SAMPLES);
            
            // put the pixel on screen, inverting y coordinate
            win_putpixel(x, SCREEN_HEIGHT - y - 1, color);
        }

        // print progress so that it doesn't seem like
        // we've stalled
        printf("\r%f%% done", (y * 100.0f) / SCREEN_HEIGHT);
        fflush(stdout);
    }

    // show the window indefinitely
    while (true)
        win_update();
}
