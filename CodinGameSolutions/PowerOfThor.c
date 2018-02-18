#include <stdlib.h>
#include <stdio.h>

int main( int argc, char** argv ) 
{
    int lX, lY, tX, tY; // Coordinates of the light and Thor
    scanf( "%d%d%d%d", &lX, &lY, &tX, &tY );

    for ( ;; ) {
        if ( tY < lY ) {
            printf("S");
            tY++;
        }
        else if ( tY > lY ) {
            printf("N");
            tY--;
        }

        if ( tX < lX ) {
            printf("E");
            tX++;
        }
        else if ( tX > lX ) {
            printf("W");
            tX--;
        }
        printf("\n");        
    }
    return EXIT_SUCCESS;
}