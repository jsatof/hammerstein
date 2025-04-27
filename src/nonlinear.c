#include <nonlinear.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util.h>

void init_nonlinear(nonlinear_param_t *params) {
    params->mat_a = (float*) malloc(params->mat_dim * params->mat_dim * sizeof(float));
    if (!params->mat_a) {
        fprintf(stderr, "ERROR: not enough memory for mat_a\n");
        return;
    }
    params->mat_b = (float*) malloc(params->mat_dim * params->mat_dim * sizeof(float));
    if (!params->mat_b) {
        fprintf(stderr, "ERROR: not enough memory for mat_b\n");
        return;
    }

    // TODO: This should be handled a different way
    float input_a[] = {
        1, 2, 3,
        1, 2, 3,
        1, 2, 3,
    };
    assert((sizeof(input_a) / sizeof(input_a[0])) == params->mat_dim * params->mat_dim);

    float input_b[] = {
        1, 2, 3,
        1, 2, 3,
        1, 2, 3,
    };
    assert((sizeof(input_b) / sizeof(input_b[0])) == params->mat_dim * params->mat_dim);

    memcpy(params->mat_a, input_a, params->mat_dim * params->mat_dim);
    memcpy(params->mat_b, input_b, params->mat_dim * params->mat_dim);

    print_matrix(input_a, params->mat_dim);
    print_matrix(input_b, params->mat_dim);
}
