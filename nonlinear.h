#pragma once

#include <stddef.h>

typedef struct {
    float *mat_a;
    float *mat_b;
    size_t mat_dim;
} nonlinear_param_t;


inline void init_nonlinear(nonlinear_param_t *params);
