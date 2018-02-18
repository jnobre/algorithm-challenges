#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
// limits.h contains the macro INT_MAX which is the maximum capacity of an int

int main(int argc, char** argv)
{
    int N;
    scanf("%d\n", &N);

    if (N == 0)
        printf("0");
    else {
        char str[10000];
        fgets(str,10000,stdin);
        char *iter = str;

        int T, len, min=INT_MAX;
        /* Search the temperature of minimum absolute valueReads
        Note a subtilty of the exercice : a temperature T>0 is prefered over -T*/
        while (sscanf(iter, "%d%n", &T, &len) == 1) {
          if (abs(T) < abs(min) || (T == -min && T > 0))
              min = T;
          iter+=len;
        }

        printf("%d", min);
    }
    return EXIT_SUCCESS;
    /* sscanf takes a char* pointer on the string to read
       %d means an integer is expected
       %n means the nb of characters consumed is stored in len
       which is then used to move forward in str
       sscanf returns the number of item read (here 1 or in the end 0)*/
}

