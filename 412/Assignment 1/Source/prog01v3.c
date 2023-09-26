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

void list_common_factors(int num){

    printf("The list of divisors of %d is: ", num);
    
    for (int i = 1; i <= num; i++) {
        if (num % i == 0) {
            if (i == 1)
            {
                printf("%d", i);
            }else{
                printf(", %d", i);
            }
            
            
        }

    }
    printf(".");
}

int main(int argc, char** argv) {

    if (argc < 2 ) {
        printf("program launched with no argument.\nProper usage: ./prog m [n]");
        return 1;
    }else if (argc == 2)
    {
        int num = atoi(argv[1]);
        list_common_factors(num);
    }else if (argc == 3)
    {
        int num1 = atoi(argv[1]);
        int num2 = atoi(argv[2]);
        findGCD(num1,num2);
        int GCD = findGCD(num1,num2);
        printf("The gcd of %d and %d is %d.", num1, num2,GCD);
    }else
    {
        printf("program launched with too many arguments.\nProper usage: ./prog m [n]");
        return 1;
    }
    

    int num = atoi(argv[1]); // Convert the command line argument to an integer
    

    
    
    
    return 0;
}