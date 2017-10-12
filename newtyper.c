#include "m_pd.h"
#include <math.h>

static t_class * newtyper_class;

typedef struct _newtyper{
  t_object  x_obj;
  t_float bigram_table[200][200];
  FILE * myfile;
  t_int first, second;
  t_symbol * filepath;
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
    post("97-98: %f", x->bigram_table[97][115]);
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
