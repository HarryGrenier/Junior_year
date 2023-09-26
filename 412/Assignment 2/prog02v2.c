#include <stdio.h>
#include <stdlib.h>


int isInArray(int val, int arr[], int size);

int main(int argc, char *argv[]) {
    // Creates an array to store unique values
    int uniqueValues[1000];
    // Keeps track of # of values in the array
    int uniqueCount = 0;
    // Calculates the number to series to calculate
    int numberOfTriplets = (argc - 1) / 3;

    // Starts the main loop to execute the fibo series
    for (int i = 0; i < numberOfTriplets; i++) {
        int F1 = atoi(argv[3*i + 1]);
        int F2 = atoi(argv[3*i + 2]);
        int n = atoi(argv[3*i + 3]);

        // Checks to make sure arguments are correct
        if (F1 <= 0 || F2 <= 0 || n <= 0 || ((argc - 1) % 3 != 0)) {
        printf("usage: ./fibo [F1 F2 n]+, with F2>F1>0 and n>0 all integers");
            return 1;
        }

        // Print header
        printf("%d terms of the Fibonacci sequence with F1=%d and F2=%d:\n", n, F1, F2);
        // Compute and print Fibonacci sequence
        for (int j = 1; j <= n; j++) {
            printf("F%d=%d", j, F1);
            // Checks for fibo # is already in array if not add it to array
            if (!isInArray(F1, uniqueValues, uniqueCount)) {
                uniqueValues[uniqueCount++] = F1;
            }

            // Outputs Fibo number
            if (j != n) {
                printf(", ");
            } else {
                printf("\n");
            }

            // Calculates next number
            int next = F1 + F2;
            F1 = F2;
            F2 = next;
        }
    }

    // outputs the unique values array
    printf("\nThe values encountered in the sequences are\n{");
    for (int i = 0; i < uniqueCount; i++) {
        printf("%d", uniqueValues[i]);
        if (i != uniqueCount - 1) {
            printf(", ");
        }
    }
    printf("}\n");

    return 0;
}

// Checks for element in an array
int isInArray(int val, int arr[], int size) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == val) return 1;
    }
    return 0;
}