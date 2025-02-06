#pragma once

///
/// General purpose types to store buffers, matrices, and other common small struct defintions
///

typedef struct {
    double *data;
    size_t size;
} float_buffer_t;

typedef struct {
    double *data;
    size_t dim;
} matrix_t;
