#include <stdio.h>


void update_bigram_table(int bigram_table[200][200], int first, int second){
  bigram_table[first][second] += 1;
}

int main(void){
  int bigram_table[200][200];

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

  // next, percentages: do it by each starting point: odds that s goes to t (x), odds that s goes to z (y)
  // So go through each "first" and sum the seconds, then divide each by the sum.
  // If sum is 0, everything equals zero.  Or skip.

}
