#include <string.h>

int x[4], w[4] = {1, 2};

void copy(int *dst, int *src, int count) {
    memcpy(dst, src, count * sizeof(int));
}

int zero_counter(int *first, int count) {
    int result = 0;
    for (int i = 0; i < count; ++i) {
        int value = *(first + sizeof(int) * i);
        if (value == 0) {
            result++;
        }
    }
    return result;
}