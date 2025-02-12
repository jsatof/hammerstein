#pragma once

#include <complex.h>

///
/// MSVC has their own complex api and types
/// https://learn.microsoft.com/en-us/cpp/c-runtime-library/complex-math-support?view=msvc-170#types-used-in-complex-math
///
#ifdef _MSC_VER
  #define complex_t _Complex
  #define real(re) _FCbuild(re, 0)
  #define imag(im) _FCbuild(0, im)
  #define mulcc _FCmulcc
  #define addcc(a, b) _FCbuild(crealf(a) + crealf(b), cimagf(a) + cimagf(b))
  #define subcc(a, b) _FCbuild(crealf(a) - crealf(b), cimagf(a) - cimagf(b))
#else
  #define complex_t double complex
  #define real(re) (re)
  #define imag(im) ((im)*I)
  #define mulcc(a, b) ((a) * (b))
  #define addcc(a, b) ((a) + (b))
  #define subcc(a, b) ((a) - (b))
#endif

///
/// General purpose types to store buffers, matrices, and other common small struct defintions
///

typedef struct {
    double *data;
    size_t size;
} buffer_t;

typedef struct {
    _Complex *(data);
    size_t size;
} complex_buffer_t;

typedef struct {
    double *data;
    size_t rows;
    size_t cols;
} matrix_t;

