#include <stdlib.h>
#include <string.h>

typedef struct _matrix {
    int rows;
    int cols;
    double *data;
} Matrix;

void inplace_add_scalar(Matrix *pm, double v) {
    double **data = pm->data;
    int cols = pm->cols;
    int rows = pm->rows;
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            data[j][i] += v;
        }
    }
}

Matrix *create_matrix(int rows, int cols) {
    if (rows < 1 || cols < 1) {
        return NULL;
    }
    Matrix *matrix = malloc(sizeof(Matrix));
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = malloc(rows * cols * sizeof(double));
    memset(matrix->data, 0, rows * cols * sizeof(double));
}

Matrix *copy_matrix(const Matrix *src) {
    Matrix *dst = malloc(sizeof(Matrix));
    dst->rows = src->rows;
    dst->cols = src->cols;
    int size = src->rows * src->cols;
    dst->data = malloc(size * sizeof(double));
    memcpy(dst->data, src->data, size);
}

Matrix *multiply(const Matrix *pm, double v) {
    Matrix *res = copy_matrix(pm);
    int size = res->rows * res->cols;
    double *data = res->data;
    for (int i = 0; i < size; ++i) {
        data[i] *= v;
    }
    return res;
}

void inplace_add_scalar(Matrix *pm, double v) {
    int size = pm->rows * pm->cols;
    double *data = pm->data;
    for (int i = 0; i < size; ++i) {
        data[i] += v;
    }
}

int swap_rows(Matrix *m, int r1, int r2) {
    if (r1 < 0 || r2 < 0 || r1 >= m->rows || r2 >= m->rows) {
        return 0;
    }
    //przyjmuje, że dane są zlinearyzowane według wierszy
    //0 1 2
    //3 4 5
    //6 7 8
    int cols = m->cols;
    double *data = m->data;
    int r1start = r1 * cols;
    int r2start = r2 * cols;
    for (int i = 0; i < cols; ++i) {
        double tmp = data[i + r1start];
        data[i + r1start] = data[i + r2start];
        data[i + r2start] = tmp;
    }
    return 1;
}

Matrix *transpose(const Matrix *psrc) {
    Matrix *result = create_matrix(psrc->cols, psrc->rows);
    if (result == NULL) {
        return NULL;
    }
    double *resData = result->data;
    double *origData = psrc->data;
    int rows = psrc->rows;
    int cols = psrc->cols;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int oldIndex = row * cols + col;
            int newIndex = col * rows + row;
            resData[newIndex] = origData[oldIndex];
        }
    }
    return result;
}