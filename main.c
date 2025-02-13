#include <assert.h>
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <nonlinear.h>
#include <types.h>
#include <signals.h>
#include <util.h>

static const double PI = acosf(-1.f);

typedef enum {
    GOOD = 0,
    BAD_ALLOC,
    BAD_FILE,
} error_state_enum;

inline void init_input_buffer(buffer_t *buffer, double start_freq, double L, double total_steps);
inline char *error_string(int state);

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


char *error_string(int state) {
    switch (state) {
        case GOOD: return "GOOD";
        case BAD_ALLOC: return "BAD_ALLOC";
    }
    return "unreachable switch in error_string(error_state)";
}

