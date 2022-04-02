#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
    for (int i = 0; i < n; ++i) {
        scanf("%lf", x++);
    }
}

void print_vector(double x[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%.4f ", x[i]);
    }
    printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%lf", &A[i][j]);
        }
    }
}

void print_mat(double A[][SIZE], int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%.4f ", A[i][j]);
        }
        printf("\n");
    }
}

void print_mat_ind(double A[][SIZE], int m, int n, const int indices[]) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%.4f ", A[indices[i]][j]);
        }
        printf("\n");
    }
}

// 5.1
// Calculate matrix product, AB = A X B
// A[m][p], B[p][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
    for (int column = 0; column < n; ++column) {
        for (int row = 0; row < m; ++row) {
            double sum = 0;
            for (int i = 0; i < p; ++i) {
                sum += A[row][i] * B[i][column];
            }
            AB[row][column] = sum;
        }
    }
}

// Calculate matrix - vector product,  Ab = A X b
// A[m][p], b[p]
void mat_vec_product(double A[][SIZE], const double b[], double Ab[], int m, int p) {
    for (int row = 0; row < m; ++row) {
        double sum = 0;
        for (int i = 0; i < p; ++i) {
            sum += A[row][i] * b[i];
        }
        Ab[row] = sum;
    }
}

// 5.2
// Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
    for (int pivot = 0; pivot < n; ++pivot) {
        if (A[pivot][pivot] == 0) {
            return NAN;
        }

        for (int row = pivot + 1; row < n; ++row) {
            double factor = A[row][pivot] / A[pivot][pivot];
            for (int column = pivot; column < n; column++) {
                A[row][column] -= factor * A[pivot][column];
            }
        }
    }

    double determinant = 1;
    for (int i = 0; i < n; ++i) {
        determinant *= A[i][i];
    }

    return determinant;
}

// 5.3
// Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
    int index[n];
    double tempX[n];
    for (int i = 0; i < n; ++i) {
        index[i] = i;
        tempX[i] = b[i];
    }

    int signum = 1;
    for (int pivot = 0; pivot < n; ++pivot) {
        // find max pivot
        int max_row = pivot;
        for (int row = pivot + 1; row < n; ++row) {
            if (fabs(A[index[row]][pivot]) > fabs(A[index[max_row]][pivot])) {
                max_row = row;
            }
        }

        if (fabs(A[index[max_row]][pivot]) < eps) {
            return 0;
        }

        if (max_row != pivot) {
            signum *= -1;
            int temp = index[max_row];
            index[max_row] = index[pivot];
            index[pivot] = temp;
        }

        // convert to triangular matrix
        for (int row = pivot + 1; row < n; ++row) {
            double factor = A[index[row]][pivot] / A[index[pivot]][pivot];
            tempX[index[row]] -= factor * tempX[index[pivot]];
            for (int column = pivot; column < n; column++) {
                A[index[row]][column] -= factor * A[index[pivot]][column];
            }
        }
    }

    double determinant = 1;
    for (int i = 0; i < n; ++i) {
        determinant *= A[index[i]][i];
    }

    // convert to diagonal matrix
    for (int pivot = n - 1; pivot > 0; --pivot) {
        for (int row = pivot - 1; row >= 0; --row) {
            double factor = A[index[row]][pivot] / A[index[pivot]][pivot];
            tempX[index[row]] -= factor * tempX[index[pivot]];
            A[index[row]][pivot] -= factor * A[index[pivot]][pivot];
        }
    }

    // convert to unitary matrix
    for (int pivot = n - 1; pivot >= 0; --pivot) {
        tempX[index[pivot]] /= A[index[pivot]][pivot];
        A[index[pivot]][pivot] /= A[index[pivot]][pivot];
    }

    // copy result to x in order
    for (int i = 0; i < n; ++i) {
        x[i] = tempX[index[i]];
    }

    return signum * determinant;
}

// 5.4
// Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
    int index[n];
    double tempB[n][SIZE];
    // setup index vector and unitary matrix tempB
    for (int i = 0; i < n; ++i) {
        index[i] = i;
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                tempB[i][j] = 1;
            } else {
                tempB[i][j] = 0;
            }
        }
    }

    int signum = 1;
    for (int pivot = 0; pivot < n; ++pivot) {
        // find max pivot
        int max_row = pivot;
        for (int row = pivot + 1; row < n; ++row) {
            if (fabs(A[index[row]][pivot]) > fabs(A[index[max_row]][pivot])) {
                max_row = row;
            }
        }

        if (fabs(A[index[max_row]][pivot]) < eps) {
            return 0;
        }

        if (max_row != pivot) {
            signum *= -1;
            int temp = index[max_row];
            index[max_row] = index[pivot];
            index[pivot] = temp;
        }

        // convert to triangular matrix
        for (int row = pivot + 1; row < n; ++row) {
            double factor = A[index[row]][pivot] / A[index[pivot]][pivot];
            for (int column = pivot; column < n; ++column) {
                A[index[row]][column] -= factor * A[index[pivot]][column];
            }
            for (int column = 0; column < n; ++column) {
                tempB[index[row]][column] -= factor * tempB[index[pivot]][column];
            }
        }
    }

    double determinant = 1;
    for (int i = 0; i < n; ++i) {
        determinant *= A[index[i]][i];
    }

    // convert to diagonal matrix
    for (int pivot = n - 1; pivot > 0; --pivot) {
        for (int row = pivot - 1; row >= 0; --row) {
            double factor = A[index[row]][pivot] / A[index[pivot]][pivot];
            for (int column = 0; column < n; column++) {
                tempB[index[row]][column] -= factor * tempB[index[pivot]][column];
            }
            A[index[row]][pivot] -= factor * A[index[pivot]][pivot];
        }
    }

    // convert to unitary matrix
    for (int pivot = n - 1; pivot >= 0; --pivot) {
        for (int column = 0; column < n; column++) {
            tempB[index[pivot]][column] /= A[index[pivot]][pivot];
        }
    }


    for (int pivot = n - 1; pivot >= 0; --pivot) {
        A[index[pivot]][pivot] /= A[index[pivot]][pivot];
    }

    // copy result to B in order
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            B[i][j] = tempB[index[i]][j];
        }
    }

    return signum * determinant;
}

int main(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;

    int to_do;
    int m, n, p;

    scanf("%d", &to_do);

    switch (to_do) {
        case 1:
            scanf("%d %d %d", &m, &p, &n);
            read_mat(A, m, p);
            read_mat(B, p, n);
            mat_product(A, B, C, m, p, n);
            print_mat(C, m, n);
            break;
        case 2:
            scanf("%d", &n);
            read_mat(A, n, n);
            printf("%.4f\n", gauss_simplified(A, n));
            break;
        case 3:
            scanf("%d", &n);
            read_mat(A, n, n);
            read_vector(b, n);
            det = gauss(A, b, x, n, eps);
            printf("%.4f\n", det);
            if (det) print_vector(x, n);
            break;
        case 4:
            scanf("%d", &n);
            read_mat(A, n, n);
            printf("%.4f\n", matrix_inv(A, B, n, eps));
            print_mat(B, n, n);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

