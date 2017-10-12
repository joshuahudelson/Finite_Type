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
