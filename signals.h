#pragma once

#include <types.h>

void fft(complex_buffer_t *output, buffer_t *input);
inline void freq_response(double complex *output, double complex *input, size_t offset, size_t size, double *kernel_a, double *kernel_b, size_t kernel_size);
inline void filter(double *output, size_t offset, double kernel_b[3], double kernel_a[3], buffer_t *input);
