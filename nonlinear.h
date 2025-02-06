#pragma once

#include <stddef.h>

#include <types.h>

///
/// The nonlinear_model_t struct contains two NxN matrix pointers.
/// To index width these, the poitner arithmetic is (i * row_size + j).
///
typedef struct {
    double *mat_a;
    double *mat_b;
    size_t mat_dim;
} nonlinear_model_t;

///
/// Functions involving nonlinear_model_t are meant to allocate their own memory.
/// The caller of these functions must free.
///
inline void init_nonlinear(nonlinear_model_t *params);

///
/// Producing the same output as the following matlab code:
///     NL_system = @(x) filter(B(1,:), A(1,:), x) + filter(B(2,:), A(2,:), x.^2) + filter(B(3,:), A(3,:), x.^3);
/// where A and B are 3x3 matrices of coefficients, x is the input signal
///     y[n] = − a1*y[n−1] − a2*y[n−2] − ... − aM*y[n−M] + b0*x[n] + b1*x[n−1] + b2*x[n−2] + ... + bN*x[n−N]
///
inline void nonlinear_filter(float_buffer_t *output, float_buffer_t *input, nonlinear_model_t *model);
