#include "m_pd.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
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

  t_int bigram_count_table[NUM_STATES][NUM_STATES];
  t_float bigram_table[NUM_STATES][NUM_STATES];   // transition probabilities.
  FILE * myfile;
  t_symbol * filepath;
  t_int first_letter, second_letter;              // tallying bigrams in text file.

  t_float tops[MAX_NUM_OBSERVATIONS][NUM_STATES];    // highest viterbi scores
  t_int paths[MAX_NUM_OBSERVATIONS][NUM_STATES];   // keys of higest scores
  t_float timings_table[NUM_STATES][NUM_STATES][MAX_NUM_TIMINGS];
  t_int timings_counter[NUM_STATES][NUM_STATES];     // num timings recorded so far
  t_int division_counter[NUM_STATES][NUM_STATES];    // timings denominator for mean (max = MAX_NUM_TIMINGS)
  t_float means[NUM_STATES][NUM_STATES];
  t_float stdevs[NUM_STATES][NUM_STATES];
  t_int from_key, to_key;
  t_int key_1_full;

  t_int obscount;                                    // how many letters in typed word made so far
  t_float obs[MAX_NUM_OBSERVATIONS];                 // the timing of the letter in typed word

  clock_t time_then;
}t_newtyper;


void newtyper_mean(t_newtyper * x){
  // calculate the mean of the timings for the pair of keys.
  int sum1 = 0;
  for (int i=0; i < x->division_counter[x->from_key][x->to_key]; i++){
    sum1 += x->timings_table[x->from_key][x->to_key][i];
  }
  float temp_mean = sum1 / x->division_counter[x->from_key][x->to_key];
  x->means[x->from_key][x->to_key] = temp_mean;
}


void newtyper_stdev(t_newtyper * x){
  // calculate the standard deviation.
  float sum2 = 0;
  for (int i=0; i<x->division_counter[x->from_key][x->to_key]; i++){
    float temp_difference = (x->timings_table[x->from_key][x->to_key][i] - x->means[x->from_key][x->to_key]);
    sum2 += temp_difference * temp_difference;
  }
  sum2 = sum2 / (x->division_counter[x->from_key][x->to_key] - 1);
  float temp_stdev = sqrt(sum2);
  if (temp_stdev <= 0){
    temp_stdev = 10;
  }
  x->stdevs[x->from_key][x->to_key] = temp_stdev;
}

void newtyper_viterbi(t_newtyper * x){

  float max_score; // maximum score for a transition between keys (initial * transition * emission).
  int max_state; // the key letter (int) of the second key that yields the max score.

  // Initialize tops.  The likelihood that a given letter will start a word
  // is the transition probability of it following a spacebar (ASCII: 32).
  // Also initialize paths with the states themselves.
  for (int starting_prob=0; starting_prob<NUM_STATES; starting_prob++){
    x->tops[0][starting_prob] = x->bigram_table[32][starting_prob];
    x->paths[0][starting_prob] = starting_prob;
    post("x->tops[0][starting_prob] = %f", x->tops[0][starting_prob]);  // looking good.
  }

  // for each timing observation...
  for(int observation=0; observation < x->obscount; observation++){
    // go through the list of states we already have, and have two variables
    // ready for each.  Also be ready with whatever state is in the paths...
    for(int letter_one = 0; letter_one < NUM_STATES; letter_one++){
      //reset max score and state.
      max_score = 0;
      max_state = 0;
      // have the previous emission letter on hand
      int saved_letter_one = x->paths[observation][letter_one];

      // and then compare to states we could transition to...
      for(int potential_letter_two=0; potential_letter_two < NUM_STATES; potential_letter_two++){

        float zscore = (x->obs[observation] - x->means[saved_letter_one][potential_letter_two]) / x->stdevs[saved_letter_one][potential_letter_two];
        zscore = abs(zscore);
        zscore = ((int) (zscore * 100)) - 1;
        if(zscore > 389){
          zscore = 389;
          post("Too big!");
        }
        if (zscore < 0){
          zscore = 150;
        }

        // if zscore is zero, then it's zero stdevs away from mean, so it's good.
        zscore = ztable[(int) zscore];
        zscore = abs((zscore - 0.5));

        float temp_max_score = x->tops[observation][letter_one] * x->bigram_table[saved_letter_one][potential_letter_two] * zscore;
        if (temp_max_score > max_score){
          max_score = temp_max_score;
          max_state = potential_letter_two;
        }
      }
      x->tops[observation+1][letter_one] = max_score;
      x->paths[observation+1][letter_one] = max_state;
    }
  }

  /*  Whichever row in the last column of x->paths is highest is the row that
      is the highest likelihood path of the observations.  So find it.
  */

  float max_row_score = 0;
  int max_row_state = 0;
  float temp_max_score_2 = 0;

  for (int l=0; l<NUM_STATES; l++){

    temp_max_score_2 = x->tops[x->obscount][l];
    post("temp_max_score_2 -- %f", temp_max_score_2);

    if(temp_max_score_2 > max_row_score){
      max_row_score = temp_max_score_2;
      max_row_state = l;
      post("Greater!");
    }
  }

  // Then iterate through the whole path row from beginning to end and
  // print the character-translation of the states.

  for(int m=0; m <= x->obscount; m++){
    post("LETTER: %i", x->paths[m][max_row_state]);
    post("float: %f", x->tops[m][max_row_state]);
  }
  x->obscount = 0;
}


void newtyper_float(t_newtyper * x, float latest_input){

  float time_diff = clock_gettimesincewithunits(x->time_then, 1, 0);

  if (time_diff > 800){
    x->time_then = clock_getsystime();
    x->obscount = 0;
  }

  if ( (latest_input >= NUM_STATES) | (latest_input < 0) ){
    post("Input out of range!");
  }
  else if ((int) latest_input == 32){
    post("Word finished.");
    newtyper_viterbi(x);
    x->key_1_full = 0;
    x->obscount = 0;
  }
  else if (x->key_1_full == 0){
    x->from_key = latest_input;
    x->time_then = clock_getsystime();
    x->obscount = 0; // maybe unnecessary, but just to be safe...
    x->key_1_full = 1;
  }
  else{
    post("We doing this?");
    x->obs[x->obscount] = time_diff;  // record timing observation
    x->to_key = latest_input;
    // put new timing value at next location in timings_table, update location tracker.
    int index_to_update = x->timings_counter[x->from_key][x->to_key] % MAX_NUM_TIMINGS;
    x->timings_table[x->from_key][x->to_key][index_to_update] = time_diff;
    x->timings_counter[x->from_key][x->to_key]++;
    // increment division_counter (used for calculating mean)
    if (x->division_counter[x->from_key][x->to_key] < MAX_NUM_TIMINGS) {
      x->division_counter[x->from_key][x->to_key]++;
    }

    newtyper_mean(x);
    newtyper_stdev(x);

    post("%c to %c: %f ms, %f mean, %f stdev.", x->from_key, x->to_key, time_diff,
    x->means[x->from_key][x->to_key], x->stdevs[x->from_key][x->to_key]);

    x->time_then = clock_getsystime();

    x->from_key = x->to_key;

    if (x->obscount >= MAX_NUM_OBSERVATIONS){
      post("Maximum number of letters reached!");
      newtyper_viterbi(x);
      x->key_1_full = 0;
      x->obscount = 0;
    }
    else{
      x->obscount++;
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
  // load textfile and extract bigram frequencies.

  outlet_symbol(x->x_obj.ob_outlet, s);
  x->myfile = sys_fopen(s->s_name, "r");
  post("%s", s->s_name);

  // prime the pump with first letter.
  x->first_letter = (int) fgetc(x->myfile);
  post("This is the first letter: %i / %c", x->first_letter, x->first_letter);

  // go through rest of letters and count bigram frequencies.
  while(1){
    if (feof(x->myfile)){
      break;
    }
    x->second_letter = (int) fgetc(x->myfile);
    x->bigram_count_table[x->first_letter][x->second_letter] += 1.0;
    x->first_letter = x->second_letter;
  }

  post("# of s to t: %f", x->bigram_table[((int) 's')][((int) 't')]);

  int row_sum;

  // count total transitions per starting letter and normalize each transition.
  for(int i=0; i<NUM_STATES; i++){
    row_sum = 0;
    for(int j=0;j<NUM_STATES;j++){
      row_sum += x->bigram_count_table[i][j];
    }
    post("row sum for %c: %i", i, row_sum );
    if (row_sum != 0){
      for (int k=0;k<NUM_STATES;k++){
        x->bigram_table[i][k] = x->bigram_count_table[i][k] / (float) row_sum;
      }
    }
  }
  post("Transition probability for s to t: %f", x->bigram_table[((int) 's')][((int) 't')]);

  float row_sum2 = 0;

  for (int i=0; i<NUM_STATES; i++){
    row_sum2 += x->bigram_table[((int) 's')][i];
  }
  post("Sum of transition probabilities for bigrams starting with 's': %f", row_sum2);
}


void * newtyper_new(void) {
  char buff[50];
  t_newtyper *x = (t_newtyper *)pd_new(newtyper_class);
  sprintf(buff, "d%lx", (t_int)x);

  x->time_then = clock_getsystime();

  x->filepath = gensym(buff);
  pd_bind(&x->x_obj.ob_pd, x->filepath);

  x->f_out = outlet_new(&x->x_obj, &s_float);
  x->f_out2 = outlet_new(&x->x_obj, &s_float);

  x->from_key = 127;
  x->to_key = 127;

  x->key_1_full = 0;

  // initialize all table values to zero.
  for (int i=0; i<NUM_STATES; i++){
    for (int j=0; j<NUM_STATES; j++){
      x->timings_counter[i][j] = 0;
      x->division_counter[i][j] = 0;
      x->means[i][j] = 100;
      x->stdevs[i][j] = 20;
      x->bigram_count_table[i][j] = 1;
      x->bigram_table[i][j] = 0.001; // just seeing what happens if I initialize this (and type before uploading text file)
      for (int k=0; k<MAX_NUM_TIMINGS; k++){
        x->timings_table[i][j][k] = 100;
      }
    }
  }

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
