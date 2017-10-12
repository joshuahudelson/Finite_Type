#include <stdio.h>
//#include <espeak/speak_lib.h>

void update_bigram_table(float bigram_table[200][200], int first, int second){
  bigram_table[first][second] += 1.0;
}

int main(void){

  float bigram_table[200][200];

  FILE * myfile;
  int first, second;

  myfile = fopen("thisisatest.txt", "r");

  first = (int) fgetc(myfile);

  while(1){
    if (feof(myfile)){
      break;
    }
    second = (int) fgetc(myfile);
    update_bigram_table(bigram_table, first, second);
    first = second;
  }

  for(int i=0;i<200;i++){
    float row_sum = 0;
    for(int j=0;j<200;j++){
      row_sum += bigram_table[i][j];
    }
    if (row_sum != 0.0){
      for (int k=0;k<200;k++){
        bigram_table[i][k] /= row_sum;
      }
    }
  }

  for(int i=64;i<127;i++){
    for(int j=64;j<127;j++){
      printf("%f", bigram_table[i][j]);
    }
    printf("\n");
  }

// should the zeroes of things that never happen be replaced with a tiny
// number?  Because it seems like just because they never happen doesn't
// mean they might not end up getting used?

}
