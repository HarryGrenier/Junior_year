#include <stdio.h>
#include <stdlib.h>

int findGCD(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main(int argc, char** argv) {

    if (argc < 3 ) {
        printf("program launched with too few argument.\nProper usage: ./prog m n");
        return 1;
    }else if (argc > 3)
    {
        printf("program launched with too many arguments.\nProper usage: ./prog m n");
        return 1;
    }

    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]); // Convert the command line argument to an integer
    
    // Check if the input is a positive integer
    if (num1 <= 0 || num2 <= 0) {
        printf("Please enter a positive integer.\n");
        return 1;
    }

    int GCD = findGCD(num1,num2);
 
    
    
    
    printf("The gcd of %d and %d is %d.", num1, num2,GCD);

    return 0;
}
