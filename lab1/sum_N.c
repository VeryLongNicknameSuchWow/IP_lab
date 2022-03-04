#include <stdio.h>

int main(void) {
    int n, l, sum = 0;
    printf("\n");
    printf("Enter n: ");
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        printf("Enter an int: ");
        scanf("%d", &l);
        sum += l;
    }
    printf("\n");
    printf("Sum = %d\n", sum);
    printf("\n");
    return 0;
}

