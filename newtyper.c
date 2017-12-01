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
  t_float bigram_table[NUM_STATES][NUM_STATES];
  FILE * myfile;
  t_symbol * filepath;
  t_int first, second;                  // For counting bigrams.
  t_outlet * f_out, * f_out2;
  t_float tops[MAX_NUM_OBSERVATIONS][NUM_STATES]; //TOP VITERBI PICKS
  t_int paths[MAX_NUM_OBSERVATIONS-1][NUM_STATES]; //VITERBI PATH MEMORY
  t_float timings_table[NUM_STATES][NUM_STATES][MAX_NUM_TIMINGS];
  t_int timings_counter[NUM_STATES][NUM_STATES];  // total timings recorded so far
  t_int division_counter[NUM_STATES][NUM_STATES]; // amt to divide by for mean (max = MAX_NUM_OBSERVATIONS)
  t_float means[NUM_STATES][NUM_STATES];
  t_float stdevs[NUM_STATES][NUM_STATES];
  t_float xval, yval;
  t_float primed;
  t_int obscount;
  t_float obs[MAX_NUM_OBSERVATIONS];
  clock_t time_then;
}t_newtyper;

void newtyper_viterbi(t_newtyper * x);

void newtyper_float(t_newtyper * x, float latest_input){

  // need a case for spacebar to reset numobs and the x/y vals!

  float time_diff = clock_gettimesincewithunits(x->time_then, 1, 0);
  post("time is: %f", time_diff);

  // if the time difference is big, restart procedure.
  if (time_diff > 500){
    x->time_then = clock_getsystime();
    x->xval = latest_input;
    x->yval = 0;
    x->primed = 1;
  }
  // otherwise, check to make sure there's an xval already loaded:
  else if (x->primed){
    x->time_then = clock_getsystime();

    x->yval = x->xval;
    x->xval = latest_input;

    // then, as long as both values are within range:
    if (x->xval>=0 && x->xval<NUM_STATES && x->yval>=0 && x->yval<NUM_STATES){
      float sum = 0;
      float sum2 = 0;
      int i;

      // and IF THE MAX WORD LENGTH HASN'T BEEN REACHED:
      if (x->obscount<MAX_NUM_OBSERVATIONS){
        x->obs[x->obscount] = latest_input;
        x->obscount++;

        //PUT NUM IN ARRAY/UPDATE TOTAL COUNT(timetracker)/UPDATE PLACE IN ARRAY(placetracker)
        x->timings_table[(int)x->xval][(int)x->yval][x->timings_counter[(int)x->xval][(int)x->yval]%MAX_NUM_TIMINGS] = latest_input;
        x->timings_counter[(int)x->xval][(int)x->yval]++;

        if (x->division_counter[(int)x->xval][(int)x->yval] < MAX_NUM_TIMINGS) {
          x->division_counter[(int)x->xval][(int)x->yval]++;
          }

        //CALCULATE MEAN
        for (i=0;i<x->division_counter[(int)x->xval][(int)x->yval];i++){
          sum += x->timings_table[(int)x->xval][(int)x->yval][i];
        }

        x->means[(int)x->xval][(int)x->yval] = sum/x->division_counter[(int)x->xval][(int)x->yval];

        //CALCULATE STANDARD DEVIATION
        for (i=0;i<x->division_counter[(int)x->xval][(int)x->yval];i++){
        sum2 += ((x->timings_table[(int)x->xval][(int)x->yval][i] - x->means[(int)x->xval][(int)x->yval]) * (x->timings_table[(int)x->xval][(int)x->yval][i] - x->means[(int)x->xval][(int)x->yval]) );
        }
        sum2 = sum2 /x->division_counter[(int)x->xval][(int)x->yval];
        x->stdevs[(int)x->xval][(int)x->yval] = sqrt(sum2);

        post("everything updated");
        if ((int)latest_input == 32){
          post("spacebar!");
          newtyper_viterbi(x);
        }
      }
      else{
       post("WORD-LENGTH MAX REACHED! (TYPER)");
      }
    }
    else{
      post("OUT OF RANGE! (PROBABLY BECAUSE OF THE 1000 TRICK TO RESET WITH SPACEBAR");
    }
  }
  else{
    post("somehow xval isn't primed...");
  }
}

void newtyper_viterbi(t_newtyper * x){
  int i, j, k, maxstate, maxstate2; //maxstate2 is used to track the best viterbi path backwards
  float holder, maxnum, holder2, max2, zscore;
  maxnum = 0;
  maxstate = 0;
  holder = 0;
  zscore = 0;

  // Initialize tops.
  // Likelihood that a given letter will start a word = transition probability
  // of it following a spacebar (ASCII: 32).
  for (i=0; i<NUM_STATES; i++){
    x->tops[0][i] = x->bigram_table[32][i];
  }

  for(i=1;i<x->obscount;i++){
          for(j=0;j<NUM_STATES;j++){
                  maxnum = 0;
                  for(k=0;k<NUM_STATES;k++){
                      zscore = (x->obs[i] - x->means[k][j]) / x->stdevs[k][j];
                      zscore = sqrt(zscore*zscore);
                      zscore = ((int) (zscore * 100)) - 1;
                      if(zscore>389){
                          zscore = 389;
                      }
                      zscore = ztable[(int) zscore];
                      // what are these doing??...
                      // zscore = (zscore - 1) / 0.5;
                      // zscore = sqrt(zscore*zscore);
                      holder = x->tops[i-1][k] * x->bigram_table[k][j] * zscore;
                      if(holder>maxnum){
                      maxnum = holder;
                      maxstate = k;
                      }
          }
          x->tops[i][j] = maxnum;
          x->paths[i-1][j] = maxstate;
        }
  }

  //CHOOSE BEST PATH, WORKING BACKWARDS
    max2 = 0;
    maxstate2 = 0;

    outlet_float(x->f_out2, x->tops[x->obscount-1][j]);

    for(j=0;j<NUM_STATES;j++){
            holder2 = x->tops[x->obscount-1][j];
            if(holder2>max2){
                max2 = holder2;
                maxstate2 = j;
         }
    }

    post("NUMBER OF LETTERS: %d", x->obscount+1);

    for(i=x->obscount-1;i>=0;i--){
      post("LETTER: %c", x->paths[i][maxstate2]);
      outlet_float(x->x_obj.ob_outlet, (char) x->paths[i][maxstate2]);
      maxstate2 = x->paths[i][maxstate2];
    }

  outlet_float(x->f_out, (char) maxstate2);
  x->obscount = 0;
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
