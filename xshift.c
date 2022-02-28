/*
   Simple PRNG
*/

#include "xshift.h"

static uint64_t state;

void xs_seed(uint64_t s){
    state = s;
}

float xs_get()
{
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;

    return (float)state / UINT64_MAX; 
}
