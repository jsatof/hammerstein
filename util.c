#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <util.h>

double min(double a, double b) {
    return (a < b) ? a : b;
}

double max(double a, double b) {
    return (a > b) ? a : b;
}

void print_matrix(matrix_t *matrix) {
    printf("[");
    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->cols; ++j) {
            if (j > 0) {
                printf(", ");
            } else {
                char *padding = (i == 0) ? "[" : " [";
                printf("%s", padding);
            }
            printf("%.1f", matrix->data[i * matrix->rows + j]);
        }
        char *postfix = (i == matrix->rows - 1) ? "]" : "],\n";
        printf("%s", postfix);
    }
    printf("]\n\n");
}

void print_buffer(buffer_t *buffer) {
    printf("[");
    for (size_t i = 0; i < buffer->size; ++i) {
        if (i > 0) {
            printf(", ");
        }
        printf("%f", buffer->data[i]);
    }
    printf("]\n\n");
}

void print_complex_buffer(complex_buffer_t *buffer) {
    printf("[");
    for (size_t i = 0; i < buffer->size; ++i) {
        if (i > 0) {
            printf(", ");
        }
        printf("{%f + %fi}", real(buffer->data[i]), imag(buffer->data[i]));
    }
    printf("]\n\n");
}

void dump_buffer_to_file(char *filename, buffer_t *buffer) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "ERROR: could not open file %s\n", filename);
        return;
    }
    fclose(file);
}

void buffer_pow(buffer_t *buffer, double power) {
    for (size_t i = 0; i < buffer->size; ++i) {
        buffer->data[i] = pow(buffer->data[i], power);
    }
}

void buffer_append_zeros(buffer_t *output, buffer_t *input, size_t append_size) {
    size_t new_size = input->size + append_size;
    output->data = calloc(new_size, sizeof(double));
    output->size = new_size;
    if (!output->data) {
        fprintf(stderr, "ERROR: buffer_append_zeros(): Could not allocate new buffer\n");
        return;
    }
    memcpy(output->data, input->data, input->size);
}

void plot_simple_curve(char *title, buffer_t *buffer) {
    FILE *plot_file = fopen(title, "w");
    if (!plot_file) {
        fprintf(stderr, "ERROR: Could not open file %s\n", title);
        return;
    }

    for (size_t i = 0; i < buffer->size; ++i) {
        char line[128];
        snprintf(line, sizeof(line), "%zu {%f, %f}\n\0", i, real(buffer->data[i]), imag(buffer->data[i]));
        size_t chars_to_write = min(strlen(line), 128); // why is strnlen not found?
        fwrite(line, sizeof(*line), chars_to_write, plot_file);
    }

    fclose(plot_file);
}
