#include "m_pd.h"
#include <math.h>
#include <time.h>
#define NUM_STATES            127      // ASCII codes.
#define MAX_NUM_OBSERVATIONS   20      // Maximum word length.
#define MAX_NUM_TIMINGS       100      // Timings recorded before wrap-around.

t_float ztable[390] = {0.0000, 0.0040, 0.0080, 0.0120, 0.0160, 0.0199, 0.0239, 0.0279, 0.0319, 0.0359,
                       0.0398, 0.0438, 0.0478, 0.0517, 0.0557, 0.0596, 0.0636, 0.0675, 0.0714, 0.0753,
                       0.0793, 0.0832, 0.0871, 0.0910, 0.0948, 0.0987, 0.1026, 0.1064, 0.1103, 0.1141,
                       0.1179, 0.1217, 0.1255, 0.1293, 0.1331, 0.1368, 0.1406, 0.1443, 0.1480, 0.1517,
                       0.1554, 0.1591, 0.1628, 0.1664, 0.1700, 0.1736, 0.1772, 0.1808, 0.1844, 0.1879,
                       0.1915, 0.1950, 0.1985, 0.2019, 0.2054, 0.2088, 0.2123, 0.2157, 0.2190, 0.2224,
                       0.2257, 0.2291, 0.2324, 0.2357, 0.2389, 0.2422, 0.2454, 0.2486, 0.2517, 0.2549,
                       0.2580, 0.2611, 0.2642, 0.2673, 0.2704, 0.2734, 0.2764, 0.2794, 0.2823, 0.2852,
                       0.2881, 0.2910, 0.2939, 0.2967, 0.2995, 0.3023, 0.3051, 0.3078, 0.3106, 0.3133,
                       0.3159, 0.3186, 0.3212, 0.3238, 0.3264, 0.3289, 0.3315, 0.3340, 0.3365, 0.3389,
                       0.3413, 0.3438, 0.3461, 0.3485, 0.3508, 0.3531, 0.3554, 0.3577, 0.3599, 0.3621,
                       0.3643, 0.3665, 0.3686, 0.3708, 0.3729, 0.3749, 0.3770, 0.3790, 0.3810, 0.3830,
                       0.3849, 0.3869, 0.3888, 0.3907, 0.3925, 0.3944, 0.3962, 0.3980, 0.3997, 0.4015,
                       0.4032, 0.4049, 0.4066, 0.4082, 0.4099, 0.4115, 0.4131, 0.4147, 0.4162, 0.4177,
                       0.4192, 0.4207, 0.4222, 0.4236, 0.4251, 0.4265, 0.4279, 0.4292, 0.4306, 0.4319,
                       0.4332, 0.4345, 0.4357, 0.4370, 0.4382, 0.4394, 0.4406, 0.4418, 0.4429, 0.4441,
                       0.4452, 0.4463, 0.4474, 0.4484, 0.4495, 0.4505, 0.4515, 0.4525, 0.4535, 0.4545,
                       0.4554, 0.4564, 0.4573, 0.4582, 0.4591, 0.4599, 0.4608, 0.4616, 0.4625, 0.4633,
                       0.4641, 0.4649, 0.4656, 0.4664, 0.4671, 0.4678, 0.4686, 0.4693, 0.4699, 0.4706,
                       0.4713, 0.4719, 0.4726, 0.4732, 0.4738, 0.4744, 0.4750, 0.4756, 0.4761, 0.4767,
                       0.4772, 0.4778, 0.4783, 0.4788, 0.4793, 0.4798, 0.4803, 0.4808, 0.4812, 0.4817,
                       0.4821, 0.4826, 0.4830, 0.4834, 0.4838, 0.4842, 0.4846, 0.4850, 0.4854, 0.4857,
                       0.4861, 0.4864, 0.4868, 0.4871, 0.4875, 0.4878, 0.4881, 0.4884, 0.4887, 0.4890,
                       0.4893, 0.4896, 0.4898, 0.4901, 0.4904, 0.4906, 0.4909, 0.4911, 0.4913, 0.4916,
                       0.4918, 0.4920, 0.4922, 0.4925, 0.4927, 0.4929, 0.4931, 0.4932, 0.4934, 0.4936,
                       0.4938, 0.4940, 0.4941, 0.4943, 0.4945, 0.4946, 0.4948, 0.4949, 0.4951, 0.4952,
                       0.4953, 0.4955, 0.4956, 0.4957, 0.4959, 0.4960, 0.4961, 0.4962, 0.4963, 0.4964,
                       0.4965, 0.4966, 0.4967, 0.4968, 0.4969, 0.4970, 0.4971, 0.4972, 0.4973, 0.4974,
                       0.4974, 0.4975, 0.4976, 0.4977, 0.4977, 0.4978, 0.4979, 0.4979, 0.4980, 0.4981,
                       0.4981, 0.4982, 0.4982, 0.4983, 0.4984, 0.4984, 0.4985, 0.4985, 0.4986, 0.4986,
                       0.4987, 0.4987, 0.4987, 0.4988, 0.4988, 0.4989, 0.4989, 0.4989, 0.4990, 0.4990,
                       0.4990, 0.4991, 0.4991, 0.4991, 0.4992, 0.4992, 0.4992, 0.4992, 0.4993, 0.4993,
                       0.4993, 0.4993, 0.4994, 0.4994, 0.4994, 0.4994, 0.4994, 0.4995, 0.4995, 0.4995,
                       0.4995, 0.4995, 0.4995, 0.4996, 0.4996, 0.4996, 0.4996, 0.4996, 0.4996, 0.4997,
                       0.4997, 0.4997, 0.4997, 0.4997, 0.4997, 0.4997, 0.4997, 0.4997, 0.4997, 0.4998,
                       0.4998, 0.4998, 0.4998, 0.4998, 0.4998, 0.4998, 0.4998, 0.4998, 0.4998, 0.4998,
                       0.4998, 0.4998, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999,
                       0.4999, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999,
                       0.4999, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999, 0.4999};

static t_class * newtyper_class;

typedef struct _newtyper{
  t_object  x_obj;
  t_outlet * f_out, * f_out2;

  t_float bigram_table[NUM_STATES][NUM_STATES];   // transition probabilities.
  FILE * myfile;
  t_symbol * filepath;
  t_int first_letter, second_letter;              // tallying bigrams in text file.

  t_float tops[MAX_NUM_OBSERVATIONS][NUM_STATES];    // highest viterbi scores
  t_int paths[MAX_NUM_OBSERVATIONS-1][NUM_STATES];   // keys of higest scores
  t_float timings_table[NUM_STATES][NUM_STATES][MAX_NUM_TIMINGS];
  t_int timings_counter[NUM_STATES][NUM_STATES];     // num timings recorded so far
  t_int division_counter[NUM_STATES][NUM_STATES];    // timings denominator for mean (max = MAX_NUM_TIMINGS)
  t_float means[NUM_STATES][NUM_STATES];
  t_float stdevs[NUM_STATES][NUM_STATES];
  t_float key1, key2;
  t_float primed;

  t_int obscount;                                    // how many letters in typed word made so far
  t_float obs[MAX_NUM_OBSERVATIONS];                 // the letters in typed word

  clock_t time_then;
}t_newtyper;

void newtyper_viterbi(t_newtyper * x){

  int i, j, k;
  int max_state, max_row_state;                      //max_row_state is used to track the best viterbi path backwards
  float temp_max_score, max_score, temp_max_score_2, max_row_score, zscore;

  max_score = 0;
  max_state = 0;
  temp_max_score = 0;

  // Initialize tops.  The likelihood that a given letter will start a word
  // is the transition probability of it following a spacebar (ASCII: 32).

  for (i=0; i<NUM_STATES; i++){
    x->tops[0][i] = x->bigram_table[32][i];
  }

  /* Iterate through pairs of key states, use the timing to calculate the zscore.
     "temp_max_score": the total likelihood for a given pair of keys.
     "max_score": the max temp_max_score of a set of transitions.
     "max_state": the second key that produced the max_score transition.
     x->tops: the best possible transition probabilities for timing/transitions.
     x->paths: the keys that produced the best transitions.
  */

  for(i=1;i<x->obscount;i++){
          for(j=0;j<NUM_STATES;j++){
                  max_score = 0;
                  for(k=0;k<NUM_STATES;k++){
                      zscore = (x->obs[i] - x->means[k][j]) / x->stdevs[k][j];
                      zscore = sqrt(zscore*zscore);
                      zscore = ((int) (zscore * 100)) - 1;
                      if(zscore>389){
                          zscore = 389;
                      }
                      zscore = ztable[(int) zscore];
                      temp_max_score = x->tops[i-1][k] * x->bigram_table[k][j] * zscore;
                      if(temp_max_score>max_score){
                      max_score = temp_max_score;
                      max_state = k;
                      }
          }
          x->tops[i][j] = max_score;
          x->paths[i-1][j] = max_state;
        }
  }

  /*  Whichever row in the last column of x->paths is highest is the row that
      is the highest likelihood path of the observations.  So find it.
  */

    max_row_score = 0;
    max_row_state = 0;
    temp_max_score_2 = 0;

    outlet_float(x->f_out2, x->tops[x->obscount-1][j]);

    for(j=0; j<NUM_STATES; j++)
    {
      temp_max_score_2 = x->tops[x->obscount-1][j];
      if(temp_max_score_2 > max_row_score)
      {
        max_row_score = temp_max_score_2;
        max_row_state = j;
      }
    }

    post("NUMBER OF LETTERS: %d", x->obscount+1);

    // Then iterate through the whole path row from beginning to end and
    // print the character-translation of the states.

    for(i=x->obscount-1;i>=0;i--){
      post("LETTER: %c", (char) x->paths[i][max_row_state]);
      post("float: %f", x->tops[i][max_row_state]);
      outlet_float(x->x_obj.ob_outlet, (char) x->paths[i][max_row_state]);
      max_row_state = x->paths[i][max_row_state];
    }

  outlet_float(x->f_out, (char) max_row_state);
  x->obscount = 0;
}

void newtyper_float(t_newtyper * x, float latest_input){

  float time_diff = clock_gettimesincewithunits(x->time_then, 1, 0);

  if ( (latest_input >= NUM_STATES) | (latest_input < 0) ){
    post("Input out of range!");
  }
  // if the time difference is big, or no key1 value, record new key1 and reset
  // key2 to 127 (outside NUM_STATES range)
  else if ( (time_diff > 800) | (x->key2 == 127) ){
    x->time_then = clock_getsystime();
    x->key2 = latest_input;
    x->key1 = 127;
  }
  // if input is spacebar, do viterbi and reset key values.
  else if ( (int) latest_input == 32){
      post("--word finished --");
      newtyper_viterbi(x);
      x->key2 = 127;
      x->key1 = 127;
    }
  // if first key hasn't been pressed, record it.
  else if (x->key2 == 127){
    x->key2 = latest_input;
    x->time_then = clock_getsystime();
  }
  else {

    x->key1 = x->key2;
    x->key2 = latest_input;

    float sum1 = 0;
    float sum2 = 0;
    float temp_stdev;
    float temp_mean;
    int i;

    if (x->obscount >= MAX_NUM_OBSERVATIONS){
      post("Maximum number of letters reached!");
    }
    else {

      x->obs[x->obscount] = latest_input;
      x->obscount++;

      // put new timing value at next location in timings_table, update location tracker.
      int index_to_update = x->timings_counter[(int) x->key1][(int) x->key2] % MAX_NUM_TIMINGS;
      x->timings_table[(int) x->key1][(int) x->key2][index_to_update] = latest_input;
      x->timings_counter[(int)x->key1][(int)x->key2]++;

      // divide by the number of timings_table entries that are full (max = MAX_NUM_TIMINGS)
      // note that this isn't the same as total number received (timings_counter), which is
      // theoretically infinite.
      if (x->division_counter[(int)x->key1][(int)x->key2] < MAX_NUM_TIMINGS) {
        x->division_counter[(int)x->key1][(int)x->key2]++;
        }

      // calculate the mean of the timings for the pair of keys.
      for (i=0; i<x->division_counter[(int)x->key1][(int)x->key2]; i++){
        sum1 += x->timings_table[(int)x->key1][(int)x->key2][i];
      }
      temp_mean = sum1/x->division_counter[(int)x->key1][(int)x->key2];
      x->means[(int)x->key1][(int)x->key2] = temp_mean;

      // calculate the standard deviation.
      for (i=0; i<x->division_counter[(int)x->key1][(int)x->key2]; i++){
        float temp_difference = (x->timings_table[(int)x->key1][(int)x->key2][i] - x->means[(int)x->key1][(int)x->key2]);
        sum2 += temp_difference * temp_difference;
      }
      sum2 = sum2 / x->division_counter[(int)x->key1][(int)x->key2];
      temp_stdev = sqrt(sum2);
      x->stdevs[(int) x->key1][(int) x->key2] = temp_stdev;

      post("%c to %c: %f ms, %f mean, %f stdev.", (int) x->key1, (int) x->key2, time_diff, temp_mean, temp_stdev);

      x->time_then = clock_getsystime();
    }
  }
}


void newtyper_load(t_newtyper * x, t_symbol * s){
  char *path = (s && s->s_name) ? s->s_name : "\"\"";
  sys_vgui("pdtk_openpanel {%s} {%s}\n", x->filepath->s_name, path);
}


void newtyper_onbang(t_newtyper * x){
  newtyper_load(x, &s_);
}

static void newtyper_callback(t_newtyper *x, t_symbol *s){

    x->obscount = 0;
    outlet_symbol(x->x_obj.ob_outlet, s);
    post("%s", s->s_name);
    x->myfile = sys_fopen(s->s_name, "r");
    x->first_letter = (int) fgetc(x->myfile);
    post("This should be 32: %i", x->first_letter);

    while(1){
      if (feof(x->myfile)){
        break;
      }
      x->second_letter = (int) fgetc(x->myfile);
      x->bigram_table[x->first_letter][x->second_letter] += 1.0;
      x->first_letter = x->second_letter;
    }

    post("How many s to t's: %f", x->bigram_table[115][116]);

    for(int i=0;i<NUM_STATES;i++){
      float row_sum = 0;
      for(int j=0;j<NUM_STATES;j++){
        row_sum += x->bigram_table[i][j];
      }
      if ( (int) row_sum != 0){
        for (int k=0;k<NUM_STATES;k++){
          x->bigram_table[i][k] /= row_sum;
        }
      }
    }

    post("OK!");
    post("97-98: %f", x->bigram_table[115][116]);
}

void * newtyper_new(void) {
  char buff[50];
  t_newtyper *x = (t_newtyper *)pd_new(newtyper_class);
  sprintf(buff, "d%lx", (t_int)x);

  x->primed = 0;
  x->time_then = clock_getsystime();

  x->filepath = gensym(buff);
  pd_bind(&x->x_obj.ob_pd, x->filepath);

  x->f_out = outlet_new(&x->x_obj, &s_float);
  x->f_out2 = outlet_new(&x->x_obj, &s_float);

  x->key1 = 127;
  x->key2 = 127;

  return (void *) x;
}

void newtyper_setup(void) {
  newtyper_class = class_new(gensym("newtyper"),
        (t_newmethod)newtyper_new,
        0, sizeof(t_newtyper),
        0,
        0);

  class_addfloat (newtyper_class, newtyper_float);
  class_addsymbol (newtyper_class, newtyper_load);
  class_addbang (newtyper_class, newtyper_onbang);
  class_addmethod(newtyper_class, (t_method)newtyper_callback, gensym("callback"), A_SYMBOL, 0);
  class_addmethod(newtyper_class,(t_method)newtyper_viterbi, gensym("viterbi"), 0);
}
