#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>

using namespace std;

int main(int argc, char *argv[]) {

  if( argc != 6 ) {
    fprintf(stderr, "Usage: %s colours_list #colours #rules #cases maxSeqLen\n", argv[0]);
    return -1;
  }

  FILE *fd = fopen(argv[1], "r");
  if( !fd ) {
    fprintf(stderr, "Error loading file '%s'\n", argv[1]);
    return -1;
  }

  int NColours  = atoi(argv[2]);
  int NRules    = atoi(argv[3]);
  int NCases    = atoi(argv[4]);
  int maxSeqLen = atoi(argv[5]);

  if( NColours < 5 ) {
    fprintf(stderr, "Error: #colours must be >= 5\n");
    return -1;
  }
  if( NRules < 10 ) {
    fprintf(stderr, "Error: #rules must be >= 10\n");
    return -1;
  }
  if( NCases <= 0 ) {
    fprintf(stderr, "Error: #cases must be > 0\n");
    return -1;
  }
  if( maxSeqLen <= 1 ) {
    fprintf(stderr, "Error: maxSeqLen must be > 1\n");
    return -1;
  }

  /*fprintf(stderr, "%d colours\n", NColours);
  fprintf(stderr, "%d rules\n", NRules);
  fprintf(stderr, "%d cases\n", NCases);
  fprintf(stderr, "%d maxSeqLen\n", maxSeqLen);*/

  string *colours = new string[NColours];
  char aux[128];
  for(int i=0; i<NColours; i++) {
    if( fscanf(fd, "%s", aux) != 1 ) {
      fprintf(stderr, "Error: Not enough colours in file '%s' (%d colours)\n", 
	      argv[1], i);
      return -1;
    }
    colours[i] = aux;
    for(int j=0; j<colours[i].size(); j++)
      if( (colours[i][j] < 'a' || colours[i][j] > 'z' ) && 
	  (colours[i][j] < 'A' || colours[i][j] > 'Z' ) )
	printf("%c - %s\n", colours[i][j], colours[i].c_str());
  }

  //Naive checking for whether NColours and NRules are compatible
  if( NRules > NColours*NColours*NColours ) {
    fprintf(stderr, "Please, increase proportion between #colours and #rules\n");
    return -1;
  }

  //Set random seed according to present time
  srandom(time(NULL));

  //NColours x NColours matrix to produce the rules of the game
  int *links = new int[NColours*NColours];
  for(int i=0; i<NColours*NColours; i++)
    links[i] = -1;

  //Produce rules of the game
  for(int i=0; i<NRules; i++) {
    int c1 = random()%NColours;
    int c2 = random()%NColours;
    int c3 = random()%NColours;

    if( c1 == c2 || c1 == c3 || c2 == c3 || links[c1*NColours + c2] >= 0 )
      i--; //Try again
    else {
      links[c1*NColours + c2] = c3;
      links[c2*NColours + c1] = c3;
    }
  }

  printf("%d\n", NRules);
  for(int i=0; i<NColours; i++)
    for(int j=i+1; j<NColours; j++)
      if( links[i*NColours+j] >= 0 )
	printf("%s %s %s\n", colours[i].c_str(), colours[j].c_str(),
	       colours[links[i*NColours+j]].c_str() );

  int *colused = new int[NColours];

  printf("%d\n", NCases);
  for(int i=0; i<NCases; i++) {
    int size = 3+random()%(maxSeqLen-2);

    for(int j=0; j<NColours; j++)
      colused[j] = -1;

    printf("%d\n", size);
    for(int j=0; j<size; j++) {
      
      int prob=100; //1.00
      int nhyp = 1;
      while( prob > 0.0 ) {
	int col, pr  = random()%100 + 1;
	do {
	  col = random()%NColours;
	}while( colused[col] == j );

	//Maximum number of hypotheses per token
	if( nhyp == min(NColours, 7) ) {
	  printf("%s %.2f ", colours[col].c_str(), prob/100.0);
	  prob = 0;
	} 
	else if( prob - pr >= 0 && colused[col] != j ) {
	  printf("%s %.2f ", colours[col].c_str(), pr/100.0);
	  prob -= pr;
	  colused[col] = j; //Mark colour as used in element j
	  nhyp++;
	}
	
      }
      printf("END\n");
    }
  }

  return 0;
}
