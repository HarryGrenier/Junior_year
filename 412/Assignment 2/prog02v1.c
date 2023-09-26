#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    // Convert input strings to integers
    int F1 = atoi(argv[1]);
    int F2 = atoi(argv[2]);
    int n = atoi(argv[3]);

    // Check for positive integers
    if (F1 <= 0 || F2 <= 0 || n <= 0) {
        printf("usage: ./fibo F1 F2 n, with F2>F1>0 and n>0 all integers.\n");
        return 1;
    }else if(F1 > F2){
        printf("error: The second argument must be strictly larger than the first\n");
        return 1;
    }

    // Print header
    printf("%d terms of the Fibonacci series with F1=%d and F2=%d:\n", n, F1, F2);

    // Compute and print Fibonacci sequence
    for (int i = 1; i <= n; i++) {
        printf("F%d=%d", i, F1);
        if (i != n) {
            printf(", ");
        } else {
            printf("\n");
        }

        int next = F1 + F2;
        F1 = F2;
        F2 = next;
    }

    return 0;
}
