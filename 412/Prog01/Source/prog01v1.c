#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc < 2 ) {
        printf("program launched with no argument.\nProper usage: ./prog m");
        return 1;
    }else if (argc > 2)
    {
        printf("program launched with too many arguments.\nProper usage: ./prog m");
        return 1;
    }
    

    int num = atoi(argv[1]); // Convert the command line argument to an integer
        
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
    
    return 0;
}
