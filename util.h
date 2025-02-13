#pragma once

#include <stddef.h>
#include <types.h>

inline double min(double a, double b);
inline double max(double a, double b);

///
/// Will pretty print the contents of a matrix or buffer.
///
inline void print_matrix(matrix_t *matrix);
inline void print_buffer(buffer_t *buffer);
inline void print_complex_buffer(complex_buffer_t *buffer);

///
/// Perform element-wise pow() operation on a buffer
///
inline void buffer_pow(buffer_t *buffer, double power);
void plot_simple_curve(char *title, buffer_t *input);

void buffer_append_zeros(buffer_t *output, buffer_t *input, size_t append_size);
