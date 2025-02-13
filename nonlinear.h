#pragma once

#include <stddef.h>

#include <types.h>

///
/// The nonlinear_model_t struct contains two NxN matrix pointers.
/// To index width these, the poitner arithmetic is (i * row_size + j).
///
typedef struct {
    double order;
    size_t mat_dim;
    matrix_t mat_a;
    matrix_t mat_b;
} nonlinear_model_t;

///
/// Functions involving nonlinear_model_t are meant to allocate their own memory.
/// The caller of these functions must free.
///
inline void init_nonlinear(nonlinear_model_t *params);
