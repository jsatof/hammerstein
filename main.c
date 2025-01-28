#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <nonlinear.h>

static const float PI = acosf(-1.f);

typedef struct {
    float *data;
    size_t size;
} float_buffer_t;

inline void init_start_buffer(float_buffer_t *buffer, float start_freq, float L, float total_steps);

int main() {
    float sampling_freq = 192000;
    float start_freq = 1e3;
    float end_freq = 20e3;
    float total_steps = 10;

    float L = total_steps / logf(end_freq / start_freq);

    float increment_step = total_steps - (1.f / sampling_freq);
    size_t length = sampling_freq / (increment_step);
    float_buffer_t start_buffer = {
        .data = (float*) calloc(length, sizeof(float)),
        .size = length,
    };
    if (!start_buffer.data) {
        fprintf(stderr, "ERROR: could not allocate start_buffer\n");
        return 1;
    }

    init_start_buffer(&start_buffer, start_freq, L, increment_step);
    free(start_buffer.data);
    printf("increment_step: %f\tpi: %f\n", increment_step, PI);
    printf("\n");

    // mat_dim must be set before init function
    nonlinear_param_t nonlinear_config = {
        .mat_dim = 3,
    };
    init_nonlinear(&nonlinear_config);
}

void init_start_buffer(float_buffer_t *buffer, float start_freq, float L, float increment_step) {
    printf("length: %lu\n", buffer->size);
    float value = 0;
    // prepopulate
    printf("[");
    for (size_t i = 0; i < buffer->size; ++i) {
        buffer->data[i] = sinf(2.f * PI * start_freq * L * (expf(value / L)));
        value += increment_step;
        if (i > 0) {
            printf(", ");
        }
        printf("%f", buffer->data[i]);
    }
    printf("]\n");

}
