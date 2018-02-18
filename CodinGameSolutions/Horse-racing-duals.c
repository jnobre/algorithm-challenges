#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

/* compares the two integers pointed by e1 and e2, 
   returns -1 / 0 / 1, if e1 < / = / > e2 */ 
int comp ( const void * e1 , const void * e2 ) {
    int f = *( ( int* ) e1 );
    int s = *( ( int* ) e2 );
    if ( f > s ) return  1;
    if ( f < s ) return -1;
    return 0;
}

int main()
{
    int N;
    scanf( "%d\n" , &N );
    int horses[N];
    for( int i = 0 ; i < N ; i++ )
        scanf( "%d\n" , horses + i );
    
    //sorts the powers
    qsort( horses , N , sizeof( int ) , comp );
    
    int min = INT_MAX;
    int diff;
    for( int i = 0 ; i < N-1 ; i++ ) {
        diff = horses[ i + 1 ] - horses [ i ];
        if( diff < min )
            min = diff;
    }
    
    printf( "%d\n" , min );
    return EXIT_SUCCESS;
}