#include <stdio.h>

int write(int x) {
    printf("%d", x);
    return 0;
}

int writeln(int x) {
    printf("%d\n", x);
    return 0;
}

int readln(int *x) {
    scanf("%d", x);
    return 0;
}
