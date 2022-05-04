#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

double get(int cols, int row, int col, const double *A) {
    return A[row * cols + col];
}

void set(int cols, int row, int col, double *A, double value) {
    A[row * cols + col] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB) {
    for (int row = 0; row < rowsA; ++row) {
        for (int col = 0; col < colsB; ++col) {
            double sum = 0;
            for (int i = 0; i < colsA; ++i) {
                sum += get(colsA, row, i, A) * get(colsB, i, col, B);
            }
            set(colsB, row, col, AB, sum);
        }
    }
}

void read_mat(int rows, int cols, double *t) {
    double value;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            scanf("%lf", &value);
            set(cols, row, col, t, value);
        }
    }
}

void print_mat(int rows, int cols, double *t) {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            printf("%.2f ", get(cols, row, col, t));
        }
        printf("\n");
    }
}

int read_char_lines(char *tab[]) {
    int lines = 0;
    while (1) {
        char *buffer = malloc(BUF_SIZE * sizeof(char));
        if (fgets(buffer, BUF_SIZE, stdin) == NULL) {
            break;
        }
        tab[lines++] = buffer;
    }
    return lines;
}

void write_char_line(char *tab[], int n) {
    char *line = tab[n - 1];
    for (int j = 0; j < BUF_SIZE; ++j) {
        if (line[j] == '\0') {
            break;
        }
        printf("%c", line[j]);
    }
}

void delete_lines(char *tab[], int line_count) {
    for (int i = 0; i < line_count; ++i) {
        free(tab[i]);
    }
}

int read_dbl_lines_v1(double *ptr_tab[]) {
    char buffer[BUF_SIZE];
    int lines = 0;
    int doubles = 0;
    double *series_table = ptr_tab[0];

    while (1) {
        ptr_tab[lines++] = &series_table[doubles];
        if (fgets(buffer, BUF_SIZE, stdin) == NULL) {
            break;
        }

        char *start = buffer;
        char *end;
        while (1) {
            double value = strtod(start, &end);
            series_table[doubles++] = value;
            if (start == end) {
                break;
            }
            start = end;
        }
    }

    return lines;
}

void write_dbl_line_v1(double *ptr_tab[], int n) {
    double *start = ptr_tab[n];
    double *stop = ptr_tab[n + 1];
    int i = 0;
    while (&start[i] != stop - 1) {
        printf("%.2f ", start[i++]);
    }
}

int main(void) {
    int to_do;

    scanf("%d", &to_do);

    double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
    int n, lines_counter, rowsA, colsA, rowsB, colsB;
    char *char_lines_table[TAB_SIZE];
    double series_table[TAB_SIZE];
    double *ptr_table[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d", &rowsA, &colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d", &rowsB, &colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA, colsA, colsB, A, B, C);
            print_mat(rowsA, colsB, C);
            break;
        case 2:
            scanf("%d", &n);
            ptr_table[0] = series_table;
            lines_counter = read_dbl_lines_v1(ptr_table);
            write_dbl_line_v1(ptr_table, n);
            break;
        case 3:
            scanf("%d\n", &n);
            lines_counter = read_char_lines(char_lines_table);
            write_char_line(char_lines_table, n);
            delete_lines(char_lines_table, lines_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}
