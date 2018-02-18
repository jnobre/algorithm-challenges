#include <cstdio>
#include <cstring>

#define MAX 100

using namespace std;

int N;
bool edge[MAX*MAX];

bool vivopark(int *v, int p);


int main(int argc, char *argv[]) {

  int a, b;
  int solution[MAX];

  scanf("%d", &N);

  memset(edge, 0, N*N*sizeof(bool));

  //Read edges
  while( scanf("%d-%d", &a, &b) == 2 )
    edge[(a-1)*N + b-1] = edge[(b-1)*N + a-1] = true;

  //Compute animals assignation
  if( vivopark(solution, 0) ) {

    //Print solution
    for(int i=0; i<N; i++)
      printf("%d %d\n", i+1, solution[i]);

  }
  else
    printf("Impossible assignation\n");

  return 0;
}

bool vivopark(int *solution, int p) {

  if( p==N )
    return true; //Solution found

  for(int animal=1; animal<=4; animal++) {
    bool possible=true;
    
    for(int i=0; i<p; i++)
      if( edge[p*N + i] && solution[i]==animal ) {
	possible = false;
	break;
      }

    if( possible ) {
      solution[p] = animal;      
      if( vivopark(solution, p+1) )
	return true;
    }
  }

  return false;
}
