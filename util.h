#pragma once

#include <stddef.h>
#include <types.h>

///
/// Will pretty print the contents of a matrix or buffer.
///
inline void print_matrix(matrix_t *matrix);
inline void print_buffer(float_buffer_t *buffer);

///
/// Perform element-wise square operation on a buffer
///
inline void square_buffer(float_buffer_t *buffer);
inline void cube_buffer(float_buffer_t *buffer);

