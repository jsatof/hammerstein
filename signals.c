#include <assert.h>
#include <math.h>

#include <signals.h>

static double PI = acos(-1.0);

///
/// Dont call this directly
///
void fft_inner(complex_t *output, double *input, size_t stride, size_t n) {
    assert(n > 0);
    if (n == 1) {
        output[0] = real(input[0]);
        return;
    }

    fft_inner(output,         input,          stride * 2, n / 2);
    fft_inner(output + n / 2, input + stride, stride * 2, n / 2);

    for (size_t k = 0; k < n / 2; ++k) {
        double t = (double)k / (double)n;
        complex_t v = mulcc(cexp(imag(-2 * PI * t)), output[k + n/2]);
        complex_t e = output[k];
        output[k]       = addcc(e, v);
        output[k + n/2] = subcc(e, v);
    }
}

///
/// The "public" fft function to use with buffer type params
///
void fft(complex_buffer_t *output, buffer_t *input) {
    assert(output->size == input->size);
    fft_inner(output->data, input->data, 1, input->size);
}


///
/// IIR Filtering algorithm
///
///         M                      N
///  y[n] = E{  b[k] * x[n-k]  } - E{  a[k] * y[n-k]  }
///        k=0                    k=1
///
/// @param(output): a preallocated matrix, where output->rows == the filter order
///
void filter(double *output, size_t offset, double kernel_b[3], double kernel_a[3], buffer_t *input) {
    size_t kernel_size = 3;
    size_t start_i = kernel_size - 1;
    for (size_t i = start_i; i < input->size; ++i) {
        double b_acc = 0.0;
        double a_acc = 0.0;
        for (size_t m = 0; m < kernel_size; ++m) {
            b_acc += input->data[i - start_i + m] * kernel_b[m];
            a_acc += output[i - start_i + m] * kernel_a[m];
        }
        output[i - start_i] = b_acc - a_acc;
    }
}

///
/// Calculating the frequency response given filters is performed by the following:
///                jw                 -jw              -jwM
///       jw    B(e  )    b[0] + b[1]e    + ... + b[M]e
///    H(e  ) = ------ = -----------------------------------
///                jw                 -jw              -jwN
///             A(e  )    a[0] + a[1]e    + ... + a[N]e
///
/// @param(input): the input signal to convolve with
/// @param(offset): the row offfset to index into the matrix the 
///
/// TODO: verify arithmetic
///
void freq_response(complex_t *output, complex_t *input, size_t offset, size_t size, double *kernel_a, double *kernel_b, size_t kernel_size) {
    for (size_t i = 0; i < size; ++i) {
        complex_t filter_acc_a = 0.0;
        complex_t filter_acc_b = 0.0;
        for (size_t m = 0; m < kernel_size; ++m) {
            filter_acc_b += kernel_b[m] * cexp(-m);
            filter_acc_a += kernel_a[m] * cexp(-m);
        }
        output[i] = filter_acc_b / filter_acc_a;
    }
}
