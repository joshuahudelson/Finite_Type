#include "m_pd.h"
#include "math.h"

static t_class *typer_class;

char alphabet[26];

//LETTER BIGRAM FREQUENCY TABLE:
float transitions[26][26] = {
//A     B     C     D     E     F     G     H     I     J     K     L     M     N     O     P     Q     R     S     T     U     V     W     X     Y     Z
{7.38, 11.5, 12.2, 12.1, 9.25, 10.6, 11.6, 9.03, 12.4, 9.03, 11.0, 13.1, 11.9, 13.8, 7.79, 11.4, 7.95, 13.3, 13.0, 13.5, 11.1, 11.6, 10.5, 9.64, 12.0, 9.23},
//B
{11.4, 9.00, 6.57, 6.50, 12.4, 3.85, 3.56, 5.33, 10.9, 8.14, 3.56, 11.5, 6.93, 5.61, 11.5, 5.25, 0.00, 10.6, 10.2, 8.52, 11.3, 7.05, 6.57, 0.00, 11.4, 2.71},
//C
{12.4, 4.08, 10.4, 5.76, 12.7, 3.00, 0.69, 12.5, 11.7, 1.10, 11.4, 11.1, 4.89, 4.90, 12.8, 3.76, 7.87, 11.2, 9.47, 12.1, 11.2, 1.95, 3.00, 0.69, 9.93, 5.82},
//D
{11.6, 7.33, 7.73, 10.1, 12.8, 7.46, 9.83, 7.55, 12.2, 7.54, 5.46, 9.76, 9.44, 9.16, 11.4, 6.09, 6.83, 10.7, 11.0, 7.30, 11.0, 9.31, 8.73, 0.00, 9.98, 5.09},
//E
{12.8, 10.2, 12.4, 13.3, 12.2, 11.2, 11.0, 9.36, 11.3, 7.58, 9.97, 12.4, 12.0, 13.4, 10.6, 11.4, 9.52, 13.9, 13.4, 12.3, 9.23, 11.6, 11.5, 11.3, 11.4, 8.43},
//F
{11.1, 4.32, 4.78, 3.50, 11.5, 11.3, 6.02, 3.64, 11.9, 4.23, 5.42, 10.1, 6.27, 5.31, 12.4, 3.26, 0.00, 11.5, 8.16, 10.8, 10.5, 3.81, 4.91, 1.39, 8.18, 2.56},
//G
{11.4, 5.36, 3.58, 6.81, 12.2, 6.20, 9.57, 11.7, 11.0, 3.09, 4.68, 9.99, 7.66, 10.4, 11.0, 5.28, 1.95, 11.3, 10.3, 9.35, 10.6, 2.94, 5.96, 2.08, 9.12, 3.64},
//H
{13.1, 8.20, 6.35, 7.84, 14.1, 6.60, 5.24, 6.21, 12.6, 3.50, 5.91, 9.00, 8.58, 9.71, 12.4, 5.91, 5.12, 10.6, 9.32, 11.2, 10.2, 5.18, 8.07, 0.00, 9.54, 2.64},
//I
{11.9, 10.4, 12.8, 12.4, 12.1, 11.1, 11.8, 6.54, 7.10, 7.21, 10.4, 12.5, 11.8, 14.0, 12.7, 10.5, 8.09, 11.9, 13.1, 13.1, 8.71, 11.7, 6.71, 9.25, 6.69, 10.2},
//J
{8.52, 1.61, 3.43, 3.56, 9.39, 2.08, 0.00, 1.10, 7.24, 2.30, 3.76, 2.40, 3.93, 3.37, 10.0, 1.61, 0.00, 1.39, 2.30, 2.56, 10.1, 0.69, 0.00, 0.00, 1.39, 1.79},
//K
{9.21, 6.78, 4.55, 6.64, 11.9, 6.73, 6.35, 7.68, 11.0, 3.85, 6.42, 9.22, 6.53, 9.80, 8.42, 6.30, 0.00, 8.04, 10.5, 6.95, 7.46, 5.42, 7.05, 0.00, 8.70, 2.56},
//L
{12.5, 8.60, 8.44, 11.9, 12.9, 10.0, 7.95, 7.18, 12.7, 4.22, 9.54, 12.7, 9.68, 7.78, 12.0, 9.50, 3.43, 8.53, 11.3, 10.9, 10.9, 9.58, 8.49, 3.40, 12.1, 6.25},
//M
{12.3, 10.7, 5.82, 5.65, 12.8, 7.59, 4.80, 5.64, 12.0, 3.56, 4.30, 7.21, 10.8, 7.95, 12.0, 11.8, 1.39, 5.81, 10.6, 5.84, 10.7, 4.43, 4.88, 0.00, 9.84, 2.89},
//N
{12.0, 8.42, 12.2, 13.3, 12.8, 10.2, 13.2, 9.07, 12.1, 8.40, 10.5, 10.1, 9.81, 10.9, 12.1, 7.93, 7.36, 8.38, 12.4, 13.1, 10.7, 10.3, 8.30, 7.55, 11.1, 7.75},
//O
{10.5, 10.6, 11.2, 11.3, 9.82, 13.0, 10.6, 9.43, 10.8, 8.47, 10.6, 12.1, 12.6, 13.6, 11.7, 11.7, 6.27, 13.4, 11.9, 12.2, 12.9, 11.5, 12.0, 8.62, 9.85, 8.30},
//P
{12.0, 6.81, 5.74, 5.81, 12.3, 6.47, 7.80, 10.4, 11.0, 4.91, 6.47, 11.8, 8.67, 5.60, 12.0, 11.1, 2.08, 12.2, 10.2, 10.4, 10.9, 1.10, 5.85, 0.00, 8.97, 3.58},
//Q
{3.30, 1.95, 0.00, 0.00, 0.69, 0.69, 0.00, 0.00, 6.64, 0.00, 0.00, 1.10, 0.00, 0.00, 0.00, 0.00, 1.10, 0.00, 1.10, 1.95, 10.9, 1.79, 0.69, 0.00, 0.00, 0.00},
//R
{12.8, 9.63, 11.2, 11.6, 13.7, 9.61, 11.0, 8.83, 12.7, 5.58, 11.4, 10.8, 11.3, 11.5, 12.8, 10.1, 6.18, 11.0, 12.5, 12.2, 11.0, 11.0, 8.79, 5.47, 11.6, 6.92},
//S
{12.1, 8.59, 11.1, 9.10, 12.9, 8.62, 6.66, 12.0, 12.5, 4.16, 10.2, 10.4, 10.1, 9.18, 12.0, 11.3, 8.25, 8.72, 12.2, 13.2, 11.7, 6.44, 9.15, 0.00, 9.75, 5.97},
//T
{12.5, 8.43, 9.81, 7.32, 13.3, 8.08, 7.71, 14.2, 13.2, 4.17, 6.14, 10.7, 9.83, 8.95, 13.2, 7.33, 1.95, 12.2, 12.2, 11.4, 11.4, 6.49, 10.5, 2.83, 11.5, 8.49},
//U
{10.9, 10.8, 11.3, 10.9, 11.2, 9.19, 11.1, 6.29, 10.8, 5.98, 8.08, 11.9, 11.0, 12.1, 8.53, 11.2, 5.73, 12.3, 12.3, 12.4, 5.42, 7.31, 6.43, 7.56, 8.94, 7.43},
//V
{10.8, 0.00, 6.05, 4.20, 12.9, 0.00, 4.52, 1.39, 12.0, 0.69, 3.18, 5.15, 2.40, 5.34, 10.4, 0.00, 0.00, 6.23, 6.43, 3.71, 6.76, 4.74, 0.00, 0.00, 8.11, 1.10},
//W
{12.1, 6.89, 6.09, 7.88, 11.8, 6.22, 3.71, 12.0, 12.1, 0.00, 7.07, 8.85, 7.31, 10.7, 11.7, 5.53, 1.95, 9.53, 10.0, 7.87, 4.69, 0.69, 2.94, 0.00, 8.43, 2.20},
//X
{9.11, 3.78, 9.11, 0.00, 9.61, 5.59, 1.10, 7.76, 9.11, 0.00, 0.00, 5.06, 4.39, 2.94, 7.47, 10.3, 4.76, 1.39, 4.55, 9.75, 7.96, 4.62, 6.11, 5.06, 6.00, 0.00},
//Y
{9.16, 8.22, 8.48, 7.77, 11.4, 6.35, 6.34, 6.24, 9.92, 3.09, 5.96, 9.07, 9.49, 9.11, 10.6, 8.63, 1.61, 8.97, 10.7, 8.86, 6.57, 5.64, 8.04, 4.42, 3.43, 6.56},
//Z
{9.41, 5.86, 3.69, 4.73, 10.1, 2.56, 5.00, 6.32, 8.93, 0.69, 5.29, 6.95, 6.00, 4.67, 8.13, 4.98, 3.14, 4.75, 4.98, 4.84, 6.85, 4.84, 5.21, 0.00, 7.23, 8.10},
};

#define NUMSTATES 26
#define NUMOBS 20
#define MAXENTRIES 50 //LENGTH OF THIRD DIMENSION IN "TIMINGS" ARRAY (DATA POINTS)

typedef struct _typer {
  t_object  x_obj;
  t_outlet *f_out, *f_out2;
  t_float xval, yval;


  t_float obs[NUMOBS]; //SAME AS "WORD"?
  t_int obscount; //HOW MANY LETTERS HAVE BEEN TYPED?

  t_float transitions[NUMSTATES][NUMSTATES]; //TRANSITION TABLE

  t_float init[NUMSTATES]; //WORD-START VALUES
  t_float tops[NUMOBS][NUMSTATES]; //TOP VITERBI PICKS
  t_int paths[NUMOBS-1][NUMSTATES]; //VITERBI PATH MEMORY

  t_float timings[NUMSTATES][NUMSTATES][NUMOBS]; //3-D ARRAY OF TIMINGS FOR EACH BIGRAM
  t_int placetracker[NUMSTATES][NUMSTATES]; // BIGRAM DATA FILLED (MAX = NUMOBS)
  t_int timestracker[NUMSTATES][NUMSTATES]; // TOTAL COUNT (%20 = PLACE FOR NEW DATA)
  t_float means[NUMSTATES][NUMSTATES]; // AVERAGES, OBVIOUSLY
  t_float stdev[NUMSTATES][NUMSTATES]; //STANDARD DEVIATIONS, OBVIOUSLY

  t_float ztable[35];

} t_typer;


void typer_initialize(t_typer *x)
{
    int i, j;
    float highest;

//MAKE ALL PATH VALUES ZERO (CAN PROBABLY REMOVE THIS, NOT NECESSARY)
    for(i=0;i<NUMOBS-1;i++){
        for(j=0;j<NUMSTATES;j++){
            x->paths[i][j] = 0;
        }
    }
//LOAD TRANSITION VALUES INTO INTERNAL TRANSITION TABLE, DO MATH TO MAKE IT PROBABILITIES
highest = 0;
    for(i=0;i<NUMSTATES;i++){
        for(j=0;j<NUMSTATES;j++){
        if (transitions[i][j]>highest){
            highest = transitions[i][j];
        }
    }
}
    for(i=0;i<NUMSTATES;i++){
        for(j=0;j<NUMSTATES;j++){
            x->transitions[i][j] = transitions[i][j]/(10*highest); //TABLE STILL HAS TOO MUCH WEIGHT?
        }
    }

//INITIALIZE ALL THE STANDARD DEVIATIONS TO BE TINY, BUT NOT ZERO

    for(i=0;i<NUMSTATES;i++){
        for(j=0;j<NUMSTATES;j++){
            x->stdev[i][j] = 10;
            //INITIAL STDEV NEEDS TO BE HIGH TO PREVENT PATH FROM BEING CHOSEN (?)
    }
}

    alphabet[0] = 'a';
    alphabet[1] = 'b';
    alphabet[2] = 'c';
    alphabet[3] = 'd';
    alphabet[4] = 'e';
    alphabet[5] = 'f';
    alphabet[6] = 'g';
    alphabet[7] = 'h';
    alphabet[8] = 'i';
    alphabet[9] = 'j';
    alphabet[10] = 'k';
    alphabet[11] = 'l';
    alphabet[12] = 'm';
    alphabet[13] = 'n';
    alphabet[14] = 'o';
    alphabet[15] = 'p';
    alphabet[16] = 'q';
    alphabet[17] = 'r';
    alphabet[18] = 's';
    alphabet[19] = 't';
    alphabet[20] = 'u';
    alphabet[21] = 'v';
    alphabet[22] = 'w';
    alphabet[23] = 'x';
    alphabet[24] = 'y';
    alphabet[25] = 'z';

    x->init[0] = .11602;
    x->init[1] = .04702;
    x->init[2] = .03511;
    x->init[3] = .02670;
    x->init[4] = .02007;
    x->init[5] = .03779;
    x->init[6] = .01950;
    x->init[7] = .07232;
    x->init[8] = .06286;
    x->init[9] = .00597;
    x->init[10] = .00590;
    x->init[11] = .02705;
    x->init[12] = .04374;
    x->init[13] = .02365;
    x->init[14] = .06264;
    x->init[15] = .02545;
    x->init[16] = .00173;
    x->init[17] = .01653;
    x->init[18] = .07755;
    x->init[19] = .16671;
    x->init[20] = .01487;
    x->init[21] = .00649;
    x->init[22] = .06753;
    x->init[23] = .00037;
    x->init[24] = .01620;
    x->init[25] = .00034;

    x->ztable[0] = .5;
    x->ztable[1] = .5398;
    x->ztable[2] = .5793;
    x->ztable[3] = .6179;
    x->ztable[4] = .6554;
    x->ztable[5] = .6915;
    x->ztable[6] = .7257;
    x->ztable[7] = .7580;
    x->ztable[8] = .7881;
    x->ztable[9] = .8159;
    x->ztable[10] = .8413;
    x->ztable[11] = .8643;
    x->ztable[12] = .8849;
    x->ztable[13] = .9032;
    x->ztable[14] = .9192;
    x->ztable[15] = .9332;
    x->ztable[16] = .9452;
    x->ztable[17] = .9554;
    x->ztable[18] = .9641;
    x->ztable[19] = .9713;
    x->ztable[20] = .9772;
    x->ztable[21] = .9821;
    x->ztable[22] = .9861;
    x->ztable[23] = .9893;
    x->ztable[24] = .9918;
    x->ztable[25] = .9938;
    x->ztable[26] = .9953;
    x->ztable[27] = .9965;
    x->ztable[28] = .9974;
    x->ztable[29] = .9981;
    x->ztable[30] = .9987;
    x->ztable[31] = .9990;
    x->ztable[32] = .9993;
    x->ztable[33] = .9995;
    x->ztable[34] = .9997;

    x->obscount = 0;

post("Initialization Complete");
}


void typer_float(t_typer *x, t_float val)
{
    if((x->xval>=0 && x->xval<NUMSTATES)&&((x->yval>=0 && x->yval<NUMSTATES))){
    float sum, sum2;
    int i;

    if(x->obscount<NUMOBS){
    x->obs[x->obscount] = val;
    x->obscount++;

//PUT NUM IN ARRAY/UPDATE TOTAL COUNT(timetracker)/UPDATE PLACE IN ARRAY(placetracker)
  x->timings[(int)x->xval][(int)x->yval][x->timestracker[(int)x->xval][(int)x->yval]%MAXENTRIES] = val;
  x->timestracker[(int)x->xval][(int)x->yval]++;
  if(x->placetracker[(int)x->xval][(int)x->yval]<MAXENTRIES){
    x->placetracker[(int)x->xval][(int)x->yval]++;
  }

//CALCULATE MEAN
  for (i=0;i<x->placetracker[(int)x->xval][(int)x->yval];i++){
    sum += x->timings[(int)x->xval][(int)x->yval][i];
  }
  x->means[(int)x->xval][(int)x->yval] = sum/x->placetracker[(int)x->xval][(int)x->yval];
//CALCULATE STANDARD DEVIATION
  for (i=0;i<x->placetracker[(int)x->xval][(int)x->yval];i++){
  sum2 += ((x->timings[(int)x->xval][(int)x->yval][i] - x->means[(int)x->xval][(int)x->yval]) * (x->timings[(int)x->xval][(int)x->yval][i] - x->means[(int)x->xval][(int)x->yval]) );
  }
  sum2 = sum2 /x->placetracker[(int)x->xval][(int)x->yval];
  x->stdev[(int)x->xval][(int)x->yval] = sqrt(sum2);
}
else{
 post("WORD-LENGTH MAX REACHED! (TYPER)");
}
}
else{
 post("OUT OF RANGE! (PROBABLY BECAUSE OF THE 1000 TRICK TO RESET WITH SPACEBAR");
}
}


void typer_viterbi(t_typer *x)
{
    int i, j, k, maxstate, maxstate2; //maxstate2 is used to track the best viterbi path backwards
    float holder, maxnum, holder2, max2, zscore;
    maxnum = 0;
    maxstate = 0;
    holder = 0;
    zscore = 0;

//INITIALIZE TOPS
        for(i=0;i<NUMSTATES;i++){
        //For each letter, what are the odds that it is the first letter of a word
        //given the timing: I need to put in some ZSCORE thing here when I figure it out
        x->tops[0][i] = x->init[i];
        }

//DO REST OF VITERBI
    for(i=1;i<x->obscount;i++){
            for(j=0;j<NUMSTATES;j++){
                    maxnum = 0;
                    for(k=0;k<NUMSTATES;k++){
                        zscore = 10*((x->obs[i] - x->means[k][j])/x->stdev[k][j]);
                        zscore = sqrt(zscore*zscore);
                        if(zscore>34){
                            zscore = 34;
                        }
                        zscore = x->ztable[(int)zscore];
                        zscore = (zscore - 1)/0.5;
                        zscore = sqrt(zscore*zscore);
                        holder = x->tops[i-1][k] * x->transitions[k][j] * zscore;
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

outlet_float(x->f_out2, x->tops[x->obscount-1][j]);

for(j=0;j<NUMSTATES;j++){
        holder2 = x->tops[x->obscount-1][j];
        if(holder2>max2){
            max2 = holder2;
            maxstate2 = j;
     }
}

post("NUMBER OF LETTERS: %d", x->obscount+1);
outlet_float(x->f_out, alphabet[maxstate2]);
//outlet_float(x->x_obj.ob_outlet, alphabet[maxstate2]);
//outlet_float(x->x_obj.ob_outlet, maxstate2);

post("LETTER: %c", alphabet[maxstate2]);

for(i=x->obscount-1;i>=0;i--){
    post("LETTER: %c", alphabet[x->paths[i][maxstate2]]);
    outlet_float(x->x_obj.ob_outlet, alphabet[x->paths[i][maxstate2]]);
    maxstate2 = x->paths[i][maxstate2];
}

outlet_float(x->f_out, 2014);
x->obscount = 0;

}


void typer_printstdevs(t_typer *x)
{
    int i;
    for(i=0;i<NUMSTATES;i++){
    post("%d", i);
    post("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
         x->stdev[i][0], x->stdev[i][1], x->stdev[i][2], x->stdev[i][3], x->stdev[i][4], x->stdev[i][5],
         x->stdev[i][6], x->stdev[i][7], x->stdev[i][8], x->stdev[i][9], x->stdev[i][10], x->stdev[i][11],
         x->stdev[i][12], x->stdev[i][13], x->stdev[i][14], x->stdev[i][15], x->stdev[i][16], x->stdev[i][17],
         x->stdev[i][18], x->stdev[i][19], x->stdev[i][20], x->stdev[i][21], x->stdev[i][22], x->stdev[i][23],
         x->stdev[i][24], x->stdev[i][25]);

    }
}

void typer_printmeans(t_typer *x)
{
    int i;
    for(i=0;i<NUMSTATES;i++){
    post("%d", i);
    post("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
         x->means[i][0], x->means[i][1], x->means[i][2], x->means[i][3], x->means[i][4], x->means[i][5],
         x->means[i][6], x->means[i][7], x->means[i][8], x->means[i][9], x->means[i][10], x->means[i][11],
         x->means[i][12], x->means[i][13], x->means[i][14], x->means[i][15], x->means[i][16], x->means[i][17],
         x->means[i][18], x->means[i][19], x->means[i][20], x->means[i][21], x->means[i][22], x->means[i][23],
         x->means[i][24], x->means[i][25]);
}
}

void typer_printtops(t_typer *x)
{
    int i;
    for(i=0;i<NUMSTATES;i++){
    post("%d", i);
    post("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
         x->tops[i][0], x->tops[i][1], x->tops[i][2], x->tops[i][3], x->tops[i][4], x->tops[i][5],
         x->tops[i][6], x->tops[i][7], x->tops[i][8], x->tops[i][9], x->tops[i][10], x->tops[i][11],
         x->tops[i][12], x->tops[i][13], x->tops[i][14], x->tops[i][15], x->tops[i][16], x->tops[i][17],
         x->tops[i][18], x->tops[i][19]);
}
}


void *typer_new()
{
  t_typer *x = (t_typer *)pd_new(typer_class);

  floatinlet_new(&x->x_obj, &x->xval);
  floatinlet_new(&x->x_obj, &x->yval);

  x->f_out = outlet_new(&x->x_obj, &s_float);
  x->f_out2 = outlet_new(&x->x_obj, &s_float);

  return (void *)x;
}


void typer_setup(void) {
  typer_class = class_new(gensym("typer"),
        (t_newmethod)typer_new,
        0, sizeof(t_typer),
        CLASS_DEFAULT,
        A_GIMME, 0);

  class_addfloat (typer_class, typer_float);

  class_addmethod(typer_class,(t_method)typer_initialize, gensym("initialize"), 0);
  class_addmethod(typer_class,(t_method)typer_viterbi, gensym("viterbi"), 0);
  class_addmethod(typer_class,(t_method)typer_printstdevs, gensym("printstdevs"), 0);
  class_addmethod(typer_class,(t_method)typer_printmeans, gensym("printmeans"), 0);

  class_sethelpsymbol(typer_class, gensym("help-typer"));
}
