#include "m_pd.h"
#include <math.h>

#define NUM_STATES = 127;               // ASCII.
#define MAX_NUM_OBSERVATIONS = 20;      // Maximum word length.
#define MAX_NUM_TIMINGS = 100;          // Timings recorded before wrap-around.

static t_class * newtyper_class;

typedef struct _newtyper{

  t_object  x_obj;

  t_float bigram_table[NUM_STATES][NUM_STATES];
  FILE * myfile;
  t_symbol * filepath;
  t_int first, second;                  // For counting bigrams.

  t_float timings_table[NUM_STATES][NUM_STATES][MAX_NUM_TIMINGS];
  t_int timings_counter[NUM_STATES][NUM_STATES];  // total timings recorded so far.
  t_int timings_placer[NUM_STATES][NUM_STATES]; // what position to add new timing.
  t_float means[NUM_STATES][NUM_STATES];
  t_float stdevs[NUM_STATES][NUM_STATES];

  t_float ztable[4][10][10];                    //000 to 3.99

  }t_newtyper;


void * newtyper_new(void) {

  char buff[50];
  t_newtyper *x = (t_newtyper *)pd_new(newtyper_class);
  sprintf(buff, "d%lx", (t_int)x);
  x->filepath = gensym(buff);
  pd_bind(&x->x_obj.ob_pd, x->filepath);
  outlet_new(&x->x_obj, &s_symbol);
  return (void *) x;

}

void do_viterbi(t_newtyper * x){

  // the z-score is the number (float) of stdevs away from mean.
  // the z-table translates z-scores into percentiles.
  // the farther from the mean a timing measurement, the less weight it should have.
  // but zscore is signed.  So make it always positive.
  // then get the percentile from ztable.
  // then subtract from 1.
  // the result, if it's perfectly on the mean, will be 0.5
  // So translate this, I guess, by multiplying by 2
  // Now that's the likelihood that a given timing equals some letter, given the preceding one.

}


void newtyper_load(t_newtyper * x, t_symbol * s){
  char *path = (s && s->s_name) ? s->s_name : "\"\"";
  sys_vgui("pdtk_openpanel {%s} {%s}\n", x->filepath->s_name, path);
}


void newtyper_onbang(t_newtyper * x){
  newtyper_load(x, &s_);
}


static void newtyper_callback(t_newtyper *x, t_symbol *s)
{
    outlet_symbol(x->x_obj.ob_outlet, s);
    post("%s", s->s_name);
    x->myfile = sys_fopen(s->s_name, "r");
    x->first = (int) fgetc(x->myfile);
    post("This should be 32: %i", x->first);

    while(1){
      if (feof(x->myfile)){
        break;
      }
      x->second = (int) fgetc(x->myfile);
      x->bigram_table[x->first][x->second] += 1.0;
      x->first = x->second;
    }

    post("How many s to t's: %f", x->bigram_table[115][116]);

    for(int i=0;i<200;i++){
      float row_sum = 0;
      for(int j=0;j<200;j++){
        row_sum += x->bigram_table[i][j];
      }
      if (row_sum != 0.0){
        for (int k=0;k<200;k++){
          x->bigram_table[i][k] /= row_sum;
        }
      }
    }

    post("OK!");
    post("97-98: %f", x->bigram_table[115][116]);
}

void newtyper_setup(void) {
  newtyper_class = class_new(gensym("newtyper"),
        (t_newmethod)newtyper_new,
        0, sizeof(t_newtyper),
        0,
        0);

  class_addsymbol (newtyper_class, newtyper_load);
  class_addbang (newtyper_class, newtyper_onbang);
  class_addmethod(newtyper_class, (t_method)newtyper_callback,
gensym("callback"), A_SYMBOL, 0);
}
