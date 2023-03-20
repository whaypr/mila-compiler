#include <stdio.h>

int write ( int x ) {
    printf("%d", x);
    return 0;
}

int writeln ( int x ) {
    write(x);
    printf("\n");
    return 0;
}

int readln ( int *x ) {
    scanf("%d", x);
    return 0;
}
