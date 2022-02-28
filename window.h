#pragma once
#include "vec3.h"

void win_init(int w, int h);
void win_putpixel(int x, int y, vec3_t clr);
void win_clear();
void win_update();
