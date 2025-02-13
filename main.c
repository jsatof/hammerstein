#include <assert.h>
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <nonlinear.h>
#include <types.h>
#include <util.h>

static const double PI = acosf(-1.f);

typedef enum {
    GOOD = 0,
    BAD_ALLOC,
    BAD_FILE,
} error_state_enum;

inline void init_input_buffer(buffer_t *buffer, double start_freq, double L, double total_steps);
inline void freq_response(double complex *output, double complex *input, size_t offset, size_t size, double *kernel_a, double *kernel_b, size_t kernel_size);
inline char *error_string(int state);
inline void filter(double *output, size_t offset, double kernel_b[3], double kernel_a[3], buffer_t *input);

void fft(complex_buffer_t *output, buffer_t *input);
void fft_inner(complex_t *output, double *input, size_t stride, size_t n);

int main() {
    error_state_enum error_state = GOOD;

    double sampling_freq = 192000;
    double start_freq = 1e3;
    double end_freq = 20e3;
    double total_steps = 10;
    double L = total_steps / logf(end_freq / start_freq);
    double increment_step = total_steps - (1.f / sampling_freq);
    double sample_freq_recip = 1.0 / (double)sampling_freq;
    size_t buffer_length = (10 - sample_freq_recip) / sample_freq_recip + 1;

    buffer_t input_buffer = {
        .data = calloc(buffer_length, sizeof(double)),
        .size = buffer_length,
    };
    if (!input_buffer.data) {
        error_state = BAD_ALLOC;
        fprintf(stderr, "ERROR: Could not allocate input_buffer\n", error_string(error_state));
        return 1;
    }

    init_input_buffer(&input_buffer, start_freq, L, increment_step);

    ///
    /// 
    size_t hammer_kernel_size = (1 << 12) / 2 + 1;
    size_t filter_order = 4;
    printf("hammer_kernel_size: %zu\n", hammer_kernel_size);

    double arma_filter_a[][3] = {
        {1.0, -1.8996, 0.9025},
        {1.0, -1.9075, 0.9409},
        {1.0, -1.8471, 0.8649},
        {1.0, -1.7642, 0.8464},
    };
    double arma_filter_b[][3] = {
        {1.0, -1.9027, 0.9409},
        {1.0, -1.8959, 0.9025},
        {0.5, -0.9176, 0.4512},
        {0.1, -0.1836, 0.0846},
    };

    /*
    double complex *hammer_kernel = calloc(kernel_count * hammer_kernel_size, sizeof(*hammer_kernel));
    if (!hammer_kernel) {
        fprintf(stderr, "ERROR: Could not allocate hammer_kernel\n");
        return 1;
    }

    double complex *freq_response_buffer = calloc(buffer_length, sizeof(double complex));
    if (!freq_response_buffer) {
        fprintf(stderr, "ERROR: Could not allocate freq_response_buffer\n");
        return 1;
    }

    double acc = 0.0;
    for (size_t i = 0; i < kernel_count; ++i) {
        for (size_t j = 0; j < hammer_kernel_size; ++j) {
            size_t offset = i * kernel_count;
            /////////
            freq_response(freq_response_buffer, hammer_kernel, offset, hammer_kernel_size, arma_filter_a, arma_filter_b, );
        }
    }
    */

    ///
    /// hammerstein_data is storing a matrix where each row will have the output data from the nonlinear model
    /// for each iteration
    ///
    matrix_t hammerstein_matrix = {
        .data = calloc(filter_order * buffer_length, sizeof(double)),
        .rows = filter_order,
        .cols = buffer_length,
    };
    if (!hammerstein_matrix.data) {
        error_state = BAD_ALLOC;
        fprintf(stderr, "ERROR: Could not allocate hammerstein_matrix\n", error_string(error_state));
        return 1;
    }

    size_t new_size = input_buffer.size + (buffer_length - 1);
    buffer_t scratch_buffer = {
        .data = calloc(new_size, sizeof(double)),
        .size = new_size,
    };
    if (!scratch_buffer.data) {
        error_state = BAD_ALLOC;
        fprintf(stderr, "ERROR: Could not allocate scratch_buffer for filtering: %s\n", error_string(error_state));
        return 1;
    }
    for (size_t i = 0; i < hammerstein_matrix.rows; ++i) {
        memset(scratch_buffer.data, 0, scratch_buffer.size);
        buffer_pow(&scratch_buffer, i + 1);
        filter(hammerstein_matrix.data, (i * buffer_length), arma_filter_b[i], arma_filter_a[i], &scratch_buffer);
    }
    free(scratch_buffer.data);

    buffer_t hammerstein_buffer = {
        .data = calloc(hammerstein_matrix.rows * hammerstein_matrix.cols, sizeof(double)),
        .size = hammerstein_matrix.rows * hammerstein_matrix.cols,
    };
    if (!hammerstein_buffer.data) {
        error_state = BAD_ALLOC;
        fprintf(stderr, "ERROR: Could not allocate hammerstein_buffer: %s\n", error_string(error_state));
        return 1;
    }

    complex_buffer_t fft_buffer = {
        .data = calloc(hammerstein_buffer.size, sizeof(double complex)),
        .size = hammerstein_buffer.size,
    };
    if (!fft_buffer.data) {
        error_state = BAD_ALLOC;
        fprintf(stderr, "ERROR: Could not allocate fft_buffer\n", error_string(error_state));
        return 1;
    }
    memcpy(hammerstein_buffer.data, hammerstein_matrix.data, hammerstein_buffer.size);
    free(hammerstein_matrix.data);

    fft(&fft_buffer, &hammerstein_buffer);
    printf("FFT:\n");

    plot_simple_curve("input_buffer.plot", &input_buffer);
    plot_simple_curve("hammerstein_buffer.plot", &hammerstein_buffer);

    ///
    /// End of main, proceed with cleanup
    ///
    if (error_state != GOOD) {
        fprintf(stderr, "Exiting with error state: %s", error_string(error_state));
    }

    free(input_buffer.data);
    free(hammerstein_buffer.data);
    free(fft_buffer.data);

    ///
    /// wait for user input from console
    ///
    printf("Enter to continue:\n");
    getc(stdin);
}

void init_input_buffer(buffer_t *buffer, double start_freq, double L, double increment_step) {
    double value = 0;
    // prepopulate
    for (size_t i = 0; i < buffer->size; ++i) {
        buffer->data[i] = sin(2.0 * PI * start_freq * L * (exp(value / L)));
        value += increment_step;
    }
    printf("length: %lu\n", buffer->size);
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

char *error_string(int state) {
    switch (state) {
        case GOOD: return "GOOD";
        case BAD_ALLOC: return "BAD_ALLOC";
    }
    return "unreachable switch in error_string(error_state)";
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
            b_acc += input->data[i - 2 + m] * kernel_b[m];
            a_acc += output[i - 2 + m] * kernel_a[m];
        }
        output[i - start_i] = b_acc - a_acc;
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
