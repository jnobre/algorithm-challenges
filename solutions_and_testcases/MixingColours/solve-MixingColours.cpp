#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cmath>
#include <cfloat>
#include <map>

using namespace std;

//Data structures
struct rule{
  int c1, c2, c3;
};

class cell{
  int N;
  double *hyp;

public:
  cell(int ncolours) {
    N = ncolours;
    hyp = new double[N];
    for(int i=0; i<N; i++)
      hyp[i] = -DBL_MAX;
  }

  ~cell() {
    delete[] hyp;
  }

  void set(int colour, double pr) {
    hyp[colour] = pr;
  }

  double get(int colour) {
    return hyp[colour];
  }
};

//Functions
int solve_game(rule *R, int NR, cell **S, int NS, int NC) {

  //Create and initialize the parsing table
  cell ***table = new cell**[NS];
  table[0] = S;
  for(int i=1; i<NS; i++) {
    table[i] = new cell*[NS-i];
    for(int j=0; j<NS-i; j++)
      table[i][j] = NULL;
  }
  
  //Cocke-Younger-Kasami algorithm
  for(int i=1; i<NS; i++)
    for(int j = 0; j<NS-i; j++)
      for(int k = 0; k<i; k++) {
    cell *cell1 = table[k][j];
    cell *cell2 = table[i-k-1][j+k+1];

    if( cell1 && cell2 ) {
      for(int r=0; r<NR; r++)
        if( cell1->get(R[r].c1) > -DBL_MAX && cell2->get(R[r].c2) > -DBL_MAX ) {
          
          //Add hypothesis
          cell *aux = table[i][j];
          double  pr = cell1->get(R[r].c1) + cell2->get(R[r].c2);

          if( !aux ) {
        table[i][j] = new cell(NC);
        table[i][j]->set( R[r].c3, pr );
          }
          else {
        if( table[i][j]->get( R[r].c3 ) < pr ) //Maximize probability
          table[i][j]->set( R[r].c3, pr );
          }
          
        }
    }
      }

  //Compute Most Likely Colour (MLC)
  int MLC=-1; //-1 = GAMEOVER

  if( table[NS-1][0] ) {
    double MLCpr = -DBL_MAX;
    
    for(int i=0; i<NC; i++) {
      if( table[NS-1][0]->get(i) > MLCpr ) {
    MLCpr = table[NS-1][0]->get(i);
    MLC = i;
      }
    }
  }

  //Free memory
  for(int i=1; i<NS; i++) {
    for(int j=0; j<NS-i; j++)
      if( table[i][j] ) 
    delete table[i][j];
    delete[] table[i];
  }
  delete[] table;
  
  //Return Most Likely Colour
  return MLC;
}


int main(int argc, char *argv[]) {

  int NRules, NColours;
  char tok1[64], tok2[64], tok3[64];

  scanf("%d", &NRules);
  if( NRules <=0 ) {
    fprintf(stderr, "Error reading number of rules!\n");
    return -1;
  }

  map<string,int> col2key;

  //Read the game rules
  rule *RULES = new rule[NRules*2];
  for(int nr=0; nr<NRules; nr++) {
    scanf("%s %s %s", tok1, tok2, tok3);

    if( col2key.find(tok1) == col2key.end() )
      col2key[tok1] = col2key.size()-1;
    if( col2key.find(tok2) == col2key.end() )
      col2key[tok2] = col2key.size()-1;
    if( col2key.find(tok3) == col2key.end() )
      col2key[tok3] = col2key.size()-1;

    //Set the rule (c1 + c2 -> c3)
    RULES[2*nr].c1 = col2key[tok1];
    RULES[2*nr].c2 = col2key[tok2];
    RULES[2*nr].c3 = col2key[tok3];
    //and (c2 + c1 -> c3) for direct parsing
    RULES[2*nr+1].c1 = col2key[tok2];
    RULES[2*nr+1].c2 = col2key[tok1];
    RULES[2*nr+1].c3 = col2key[tok3];
  }

  //Update the number of total rules
  NRules *= 2;
  //Save the number of colours loaded
  NColours = col2key.size();

  //Keep the correspondent string for each color key
  string *key2col = new string[NColours];
  for(map<string,int>::iterator it=col2key.begin(); it!=col2key.end(); it++)
    key2col[ it->second ] = it->first;

  int NCases, SeqLen;
  scanf("%d", &NCases);
  if( NCases <=0 ) {
    fprintf(stderr, "Error reading number of cases!\n");
    return -1;
  }

  //Read the test cases
  for(int nc=0; nc<NCases; nc++) {

    //Read sequence length
    scanf("%d", &SeqLen);
    if( SeqLen <=0 ) {
      fprintf(stderr, "Error reading sequence length (test case %d)!\n", nc);
      return -1;
    }

    cell **SEQ = new cell*[SeqLen];

    //Read Hypotheses for each element of the sequence
    for(int elem=0; elem < SeqLen; elem++) {
      double pr;

      SEQ[elem] = new cell(NColours);

      //Read first token
      scanf("%s", tok1);
      while( strcmp(tok1, "END") ) {
    //Read probability
    scanf("%lf", &pr);

    //Insert element hypothesis
    SEQ[elem]->set( col2key[tok1], log(pr) );
    
    //Read next token
    scanf("%s", tok1);
      }
    }

    int solution = solve_game(RULES, NRules, SEQ, SeqLen, NColours);

    if( solution < 0 )
      printf("GAMEOVER\n");
    else
      printf("%s\n", key2col[solution].c_str() );

    delete[] SEQ;
  }

  //delete[] RULES;
  //delete[] key2col;

  return 0;
}
