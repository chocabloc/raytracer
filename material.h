#pragma once
#include "vec3.h"
#include "ray.h"
#include <stdbool.h>

bool mat_scatter(ray_t* rin, ray_t* rout, vec3_t* factor, const hitrec_t* hit);
