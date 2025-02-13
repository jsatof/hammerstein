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
}
