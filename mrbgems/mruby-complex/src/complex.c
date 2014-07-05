/*
** complex.c - Complex class
**
** See Copyright Notice in mruby.h
*/


#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "mruby.h"

static mrb_value
cpx_new(mrb_state *mrb, mrb_value self)
{
  mrb_float real, imag;
  int argc = mrb_get_args(mrb, "|ff", &real, &imag);
  if (argc == 0) real = 0.0;
  if (argc < 2) imag = 0.0;
  return mrb_complex_value(mrb, real, imag);
}

static mrb_value
cpx_real(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mrb_real(self));
}

static mrb_value
cpx_imag(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mrb_imag(self));
}

/* ------------------------------------------------------------------------*/
void
mrb_mruby_complex_gem_init(mrb_state *mrb)
{
  struct RClass *complex, *numeric;
  
#ifdef MRB_USE_FLOAT
#ifdef MRB_INT64
  mrb_static_assert(0, "when using MRB_FLOAT and MRB_COMPLEX, do not use MRB_INT64");
#endif
  mrb_static_assert(sizeof(void*) == 4, "when using MRB_FLOAT and MRB_COMPLEX, sizeof pointer must be 4 bytes");
  mrb_static_assert(sizeof(mrb_value) == 8, "sizeof mrb_value is not 8");
#else
  mrb_static_assert(sizeof(mrb_value) == 16, "sizeof mrb_value is not 16");
#endif
  
  numeric = mrb_class_get(mrb, "Numeric");
  /* Complex() initializer */
  mrb_define_module_function(mrb, mrb->kernel_module, "Complex", cpx_new, MRB_ARGS_OPT(2));
  /* Complex Class */
  complex = mrb->complex_class = mrb_define_class(mrb, "Complex", numeric);
  mrb_undef_class_method(mrb,  complex, "new");
  mrb_undef_method(mrb,  complex, "<=>");
  mrb_define_method(mrb, complex, "real", cpx_real, MRB_ARGS_NONE());
  mrb_define_method(mrb, complex, "imag", cpx_imag, MRB_ARGS_NONE());
}

void
mrb_mruby_complex_gem_final(mrb_state* mrb)
{
}
