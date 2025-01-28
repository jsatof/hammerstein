#include <stdio.h>

#include <util.h>

void print_matrix(float *buffer, size_t mat_dim) {
    printf("[");
    for (size_t i = 0; i < mat_dim; ++i) {
        for (size_t j = 0; j < mat_dim; ++j) {
            if (j > 0) {
                printf(", ");
            } else {
                char *padding = (i == 0) ? "[" : " [";
                printf("%s", padding);
            }
            printf("%.1f", buffer[(i * mat_dim) + j]);
        }
        char *postfix = (i == mat_dim - 1) ? "]" : "],\n";
        printf("%s", postfix);
    }
    printf("]\n\n");
}
