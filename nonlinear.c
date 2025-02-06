#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nonlinear.h>
#include <types.h>
#include <util.h>

void init_nonlinear(nonlinear_model_t *params) {
    params->mat_a = (double*) malloc(params->mat_dim * params->mat_dim * sizeof(double));
    if (!params->mat_a) {
        fprintf(stderr, "ERROR: not enough memory for mat_a\n");
        return;
    }
    params->mat_b = (double*) malloc(params->mat_dim * params->mat_dim * sizeof(double));
    if (!params->mat_b) {
        fprintf(stderr, "ERROR: not enough memory for mat_b\n");
        return;
    }

    // TODO: This should be handled a different way
    double input_a[] = {
        1.0, -1.9, 0.94,
        1.0, -1.88, 0.92,
        1.0, -1.83, 0.95,
    };
    assert(sizeof(input_a) / sizeof(input_a[0]) == params->mat_dim * params->mat_dim);

    double input_b[] = {
        0.2, -0.38, 0.18,
        0.2, -0.38, 0.18,
        0.04, -0.082, 0.04,
    };
    assert(sizeof(input_b) / sizeof(input_b[0]) == params->mat_dim * params->mat_dim);

    memcpy(params->mat_a, input_a, params->mat_dim * params->mat_dim);
    memcpy(params->mat_b, input_b, params->mat_dim * params->mat_dim);
}

void nonlinear_filter(float_buffer_t *output, float_buffer_t *input, nonlinear_model_t *model) {
    assert(output->size == input->size);

    for (size_t i = 1; i < input->size; ++i) {
        for (size_t j = 0; j < model->mat_dim; ++j) {
            output->data[i] = -model->mat_a[j] * output->data[input->size - j - 1];
        }
    }
}
