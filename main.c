#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <nonlinear.h>
#include <types.h>
#include <util.h>

static const double PI = acosf(-1.f);

inline void init_start_buffer(float_buffer_t *buffer, double start_freq, double L, double total_steps);

int main() {
    double sampling_freq = 192000;
    double start_freq = 1e3;
    double end_freq = 20e3;
    double total_steps = 10;

    double L = total_steps / logf(end_freq / start_freq);

    double increment_step = total_steps - (1.f / sampling_freq);

    double sample_freq_recip = 1.0 / (double)sampling_freq;
    size_t length = (10 - sample_freq_recip) / sample_freq_recip;

    float_buffer_t start_buffer = {
        .data = (double*) calloc(length, sizeof(double)),
        .size = length,
    };
    if (!start_buffer.data) {
        fprintf(stderr, "ERROR: could not allocate start_buffer\n");
        return 1;
    }

    init_start_buffer(&start_buffer, start_freq, L, increment_step);

    // mat_dim must be set before init function
    nonlinear_model_t nonlinear_config = {
        .mat_dim = 3,
    };
    init_nonlinear(&nonlinear_config);


    free(start_buffer.data);
}

void init_start_buffer(float_buffer_t *buffer, double start_freq, double L, double increment_step) {
    printf("length: %lu\n", buffer->size);
    double value = 0;
    // prepopulate
    for (size_t i = 0; i < buffer->size; ++i) {
        buffer->data[i] = sinf(2.f * PI * start_freq * L * (expf(value / L)));
        value += increment_step;
    }
}
