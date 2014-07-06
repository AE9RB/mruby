/*
** complex.c - Complex class
**
** See Copyright Notice in mruby.h
*/


#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "mruby.h"

#define MRB_TO_COMPLEX(mrbvalue, oreal, oimag) \
switch (mrb_type(mrbvalue)) { \
case MRB_TT_COMPLEX: \
  oreal = mrb_real(mrbvalue); \
  oimag = mrb_imag(mrbvalue); \
  break; \
case MRB_TT_FIXNUM: \
  oreal = (mrb_float)mrb_fixnum(mrbvalue); \
  oimag = 0; \
  break; \
case MRB_TT_FLOAT: \
  oreal = mrb_float(mrbvalue); \
  oimag = 0; \
  break; \
default: \
  mrb_raise(mrb, E_TYPE_ERROR, "non complex value"); \
}

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

static mrb_value
cpx_equal(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_float oreal, oimag;

  mrb_get_args(mrb, "o", &other);
  switch (mrb_type(other)) {
  case MRB_TT_COMPLEX:
    oreal = mrb_real(other);
    oimag = mrb_imag(other);
    break;
  case MRB_TT_FIXNUM:
    oreal = (mrb_float)mrb_fixnum(other);
    oimag = 0;
    break;
  case MRB_TT_FLOAT:
    oreal = mrb_float(other);
    oimag = 0;
    break;
  default:
    return mrb_false_value();
  }
  return mrb_bool_value(mrb_imag(self) == oimag && mrb_real(self) == oreal);
}

static mrb_value
cpx_eql(mrb_state *mrb, mrb_value self)
{
  mrb_value other;

  mrb_get_args(mrb, "o", &other);
  if (!mrb_complex_p(other)) return mrb_false_value();
  return mrb_bool_value(mrb_imag(self) == mrb_imag(other) && mrb_real(self) == mrb_real(other));
}

static mrb_value
cpx_plus(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_float oreal, oimag;

  mrb_get_args(mrb, "o", &other);
  MRB_TO_COMPLEX(other, oreal, oimag);
  return mrb_complex_value(mrb, mrb_real(self) + oreal, mrb_imag(self) + oimag);
}

static mrb_value
cpx_minus(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_float oreal, oimag;

  mrb_get_args(mrb, "o", &other);
  MRB_TO_COMPLEX(other, oreal, oimag);
  return mrb_complex_value(mrb, mrb_real(self) - oreal, mrb_imag(self) - oimag);
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
  
  /* Complex() initializer */
  mrb_define_module_function(mrb, mrb->kernel_module, "Complex", cpx_new, MRB_ARGS_OPT(2));
  /* Complex Class */
  numeric = mrb_class_get(mrb, "Numeric");
  complex = mrb->complex_class = mrb_define_class(mrb, "Complex", numeric);
  mrb_undef_class_method(mrb,  complex, "new");
  mrb_undef_method(mrb,  complex, "<=>");
  mrb_define_method(mrb, complex, "==", cpx_equal, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, complex, "eql?", cpx_eql, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, complex, "real", cpx_real, MRB_ARGS_NONE());
  mrb_define_method(mrb, complex, "imag", cpx_imag, MRB_ARGS_NONE());
  mrb_define_method(mrb, complex, "+", cpx_plus, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, complex, "-", cpx_minus, MRB_ARGS_REQ(1));
}

void
mrb_mruby_complex_gem_final(mrb_state* mrb)
{
}
