#include <stdio.h>
#include <math.h>

typedef double (*f_1d)(double);

typedef double (*f_2d)(double, double);

typedef double (*f_3d)(double, double, double);

typedef int (*boundary_2d)(double, double);

typedef double (*int_1d)(double, double, int, f_1d);

// 6.1
double int_left_rectangle(double x1, double x2, int n, f_1d f) {
    double result = 0;
    double step = (x2 - x1) / (n - 1);
    for (int i = 0; i < n - 1; ++i) {
        double curr = x1 + i * step;
        result += f(curr) * step;
    }
    return result;
}

double int_right_rectangle(double x1, double x2, int n, f_1d f) {
    double result = 0;
    double step = (x2 - x1) / (n - 1);
    for (int i = 0; i < n - 1; ++i) {
        double curr = x1 + (i + 1) * step;
        result += f(curr) * step;
    }
    return result;
}

double int_mid_rectangle(double x1, double x2, int n, f_1d f) {
    double result = 0;
    double step = (x2 - x1) / n;
    for (int i = 0; i < n; ++i) {
        double curr = ((x1 + i * step) * 2 + step) / 2;
        result += f(curr) * step;
    }
    return result;
}

double int_trapezoid(double x1, double x2, int n, f_1d f) {
    double result = 0;
    double step = (x2 - x1) / n;
    for (int i = 0; i < n; ++i) {
        double curr = (x1 + i * step);
        result += (f(curr) + f(curr + step)) / 2 * step;
    }
    return result;
}

// 6.2
double integral_2d(double x1, double x2, int nx, double y1, double y2, int ny,
                   f_2d f, boundary_2d boundary) {
    double result = 0;
    double stepX = (x2 - x1) / nx;
    double stepY = (y2 - y1) / ny;
    for (int ix = 0; ix < nx; ++ix) {
        double currX = x1 + ix * stepX;
        for (int iy = 0; iy < ny; ++iy) {
            double currY = y1 + iy * stepY;
            if (boundary(currX, currY)) {
                result += f(currX, currY) * stepX * stepY;
            }
        }
    }
    return result;
}

//6.3
double integral_3d(double x1, double x2, int nx, double y1, double y2, int ny,
                   double z1, double z2, int nz, f_3d f) {
    double result = 0;
    double stepX = (x2 - x1) / nx;
    double stepY = (y2 - y1) / ny;
    double stepZ = (z2 - z1) / nz;
    for (int ix = 0; ix < nx; ++ix) {
        double currX = x1 + ix * stepX;
        for (int iy = 0; iy < ny; ++iy) {
            double currY = y1 + iy * stepY;
            for (int iz = 0; iz < ny; ++iz) {
                double currZ = z1 + iz * stepZ;
                result += f(currX, currY, currZ) * stepX * stepY * stepZ;
            }
        }
    }
    return result;
}

double f_1d_1(double x) {
    return x;
}

double f_1d_2(double x) {
    return x * x / 2;
}

double f_1d_3(double x) {
    return 1 / (x + 1);
}

double f_1d_4(double x) {
    return log10(x + 1);
}

double f_2d_1(double x, double y) {
    return x * y * y;
}

double f_2d_2(double x, double y) {
    return x * x + y * y;
}

double f_2d_3(double x, double y) {
    return 1 / pow((1 - x * x - y * y), 2);
}

int f_2d_1_boundary(double x, double y) {
    return x * x + y * y <= 4 && x >= 0;
}

int f_2d_2_boundary(double x, double y) {
    return x * x + x * y - 2 * y <= 0 || x * x + x * y - 2 * y >= 1;
}

int f_2d_3_boundary(double x, double y) {
    return x * x + y * y <= x && x * x + y * y <= y;
}

double f_3d_1(double x, double y, double z) {
    return x + 3 * y + 5 * z;
}

double f_3d_2(double x, double y, double z) {
    return 8 * sin(x) + pow(5 * cos(y), 2) + 2 * z;
}

int main(void) {
    int to_do;
    double x1, x2, y1, y2, z1, z2;
    int n, nx, ny, nz;
    scanf("%d", &to_do);
    switch (to_do) {
        case 1: // 6.1
            scanf("%lf %lf %d", &x1, &x2, &n);
            int_1d int_f[] = {int_left_rectangle, int_right_rectangle,
                              int_mid_rectangle, int_trapezoid};
            f_1d p_function_1d[] = {f_1d_1, f_1d_2, f_1d_3, f_1d_4};
            for (int i = 0; i < sizeof(p_function_1d) / sizeof(f_1d); i++) {
                for (int j = 0; j < sizeof(int_f) / sizeof(int_1d); j++) {
                    printf("%.2f ", int_f[j](x1, x2, n, p_function_1d[i]));
                }
                printf("\n");
            }
            break;
        case 2: // 6.2
            scanf("%lf %lf %d", &x1, &x2, &nx);
            scanf("%lf %lf %d", &y1, &y2, &ny);
            f_2d p_function_2d[] = {f_2d_1, f_2d_2, f_2d_3};
            boundary_2d p_boundaries[] = {f_2d_1_boundary, f_2d_2_boundary,
                                          f_2d_3_boundary};
            for (int i = 0; i < sizeof(p_function_2d) / sizeof(f_2d); ++i) {
                printf("%.2f ",
                       integral_2d(x1, x2, nx, y1, y2, ny,
                                   p_function_2d[i], p_boundaries[i]));
            }
            printf("\n");
            break;
        case 3: // 6.3
            scanf("%lf %lf %d", &x1, &x2, &nx);
            scanf("%lf %lf %d", &y1, &y2, &ny);
            scanf("%lf %lf %d", &z1, &z2, &nz);
            f_3d function_3d[] = {f_3d_1, f_3d_2};
            for (int i = 0; i < sizeof(function_3d) / sizeof(f_3d); ++i) {
                printf("%.2f ",
                       integral_3d(x1, x2, nx, y1, y2, ny, z1, z2, nz, function_3d[i]));
            }
            printf("\n");
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

