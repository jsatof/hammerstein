#include <stdio.h>

#include <util.h>

void print_matrix(matrix_t *matrix) {
    printf("[");
    for (size_t i = 0; i < matrix->dim; ++i) {
        for (size_t j = 0; j < matrix->dim; ++j) {
            if (j > 0) {
                printf(", ");
            } else {
                char *padding = (i == 0) ? "[" : " [";
                printf("%s", padding);
            }
            printf("%.1f", matrix->data[(i * matrix->dim) + j]);
        }
        char *postfix = (i == matrix->dim - 1) ? "]" : "],\n";
        printf("%s", postfix);
    }
    printf("]\n\n");
}

void print_buffer(float_buffer_t *buffer) {
    printf("[");
    for (size_t i = 0; i < buffer->size; ++i) {
        if (i > 0) {
            printf(", ");
        }
        printf("%f", buffer->data[i]);
    }
    printf("]\n\n");
}

void dump_buffer_to_file(char *filename, float_buffer_t *buffer) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "ERROR: could not open file %s\n", filename);
        return;
    }
    fclose(file);
}

void square_buffer(float_buffer_t *buffer) {
    for (size_t i = 0; i < buffer->size; ++i) {
        buffer->data[i] *= buffer->data[i];
    }
}

void cube_buffer(float_buffer_t *buffer) {
    for (size_t i = 0; i < buffer->size; ++i) {
        buffer->data[i] *= buffer->data[i] * buffer->data[i];
    }
}
