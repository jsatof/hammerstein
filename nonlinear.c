#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nonlinear.h>
#include <types.h>
#include <util.h>

void init_nonlinear(nonlinear_model_t *params) {
    const size_t mat_dim = 3;
    params->mat_a.rows = mat_dim;
    params->mat_a.cols = mat_dim;
    params->mat_a.rows = mat_dim;
    params->mat_a.cols = mat_dim;
    params->order = 4;

    ///
    /// ARMA (auto regressive moving average) filter params
    /// order = 2
    /// filters = 2
    /// size must be equal to mat_dim
    /// may need to handle dynamic square array sizes?
    ///
    double input_a[][3] = {
        {1.0, -1.9, 0.94},
        {1.0, -1.88, 0.92},
        {1.0, -1.83, 0.95},
    };
    double input_b[][3] = {
        {0.2, -0.38, 0.18},
        {0.2, -0.38, 0.18},
        {0.04, -0.082, 0.04},
    };

    //memcpy(params->mat_a.data, input_a, mat_dim * mat_dim * sizeof(double));

    printf("mat_a:\n");
    //print_matrix(&params->mat_a);
    //printf("mat_b:\n");
    //print_matrix(&params->mat_b);
}

///
/// a[0]*y[n] = b[0]*x[n] + b[1]*x[n-1] + ... + b[M]*x[n-M]
///                       - a[1]*y[n-1] - ... - a[N]*y[n-N]
///
void nonlinear_filter(buffer_t *output, buffer_t *input, nonlinear_model_t *model) {
    assert(output->size == input->size);

    for (size_t i = 1; i < input->size; ++i) {
        for (size_t j = 0; j < model->mat_dim; ++j) {
            //output->data[i] = -(model->mat_a[j]) * output->data[input->size - j - 1];
        }
    }
}
