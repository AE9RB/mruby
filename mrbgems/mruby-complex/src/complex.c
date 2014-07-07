/*
** complex.c - Complex class
**
** See Copyright Notice in mruby.h
*/


#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "mruby.h"

mrb_noreturn static void
cpx_raise_not_numeric_error(mrb_state *mrb)
{
  mrb_raise(mrb, E_TYPE_ERROR, "non complex value");
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
  mrb_float oreal;

  mrb_get_args(mrb, "o", &other);
  switch (mrb_type(other)) {
  case MRB_TT_COMPLEX:
    return mrb_complex_value(mrb,
      mrb_real(self) + mrb_real(other),
      mrb_imag(self) + mrb_imag(other)
    );
    break;
  case MRB_TT_FIXNUM:
    oreal = (mrb_float)mrb_fixnum(other);
    break;
  case MRB_TT_FLOAT:
    oreal = mrb_float(other);
    break;
  default:
    cpx_raise_not_numeric_error(mrb);
  }
  return mrb_complex_value(mrb,
    mrb_real(self) + oreal,
    mrb_imag(self)
  );
}

static mrb_value
cpx_minus(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_float oreal;

  mrb_get_args(mrb, "o", &other);
  switch (mrb_type(other)) {
  case MRB_TT_COMPLEX:
    return mrb_complex_value(mrb,
      mrb_real(self) - mrb_real(other),
      mrb_imag(self) - mrb_imag(other)
    );
    break;
  case MRB_TT_FIXNUM:
    oreal = (mrb_float)mrb_fixnum(other);
    break;
  case MRB_TT_FLOAT:
    oreal = mrb_float(other);
    break;
  default:
    cpx_raise_not_numeric_error(mrb);
  }
  return mrb_complex_value(mrb,
    mrb_real(self) - oreal,
    mrb_imag(self)
  );
}

static mrb_value
cpx_mul(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_float oreal;

  mrb_get_args(mrb, "o", &other);
  switch (mrb_type(other)) {
  case MRB_TT_COMPLEX:
    return mrb_complex_value(mrb,
      mrb_real(self) * mrb_real(other) - mrb_imag(self) * mrb_imag(other),
      mrb_imag(self) * mrb_real(other) + mrb_real(self) * mrb_imag(other)
      );
    break;
  case MRB_TT_FIXNUM:
    oreal = (mrb_float)mrb_fixnum(other);
    break;
  case MRB_TT_FLOAT:
    oreal = mrb_float(other);
    break;
  default:
    cpx_raise_not_numeric_error(mrb);
  }
  return mrb_complex_value(mrb,
    mrb_real(self) * oreal,
    mrb_imag(self) * oreal
  );
}

static mrb_value
cpx_div(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_float oreal;

  mrb_get_args(mrb, "o", &other);
  switch (mrb_type(other)) {
  case MRB_TT_COMPLEX:
    if (mrb_float_abs(mrb_real(other)) > mrb_float_abs(mrb_imag(other))) {
      mrb_float r, n;
      r = mrb_imag(other) / mrb_real(other);
      n = mrb_real(other) * (r*r+1);
      return mrb_complex_value(mrb,
        (mrb_real(self) + (mrb_imag(self) * r)) / n,
        (mrb_imag(self) - (mrb_real(self) * r)) / n
      );
    } else {
      mrb_float r, n;
      r = mrb_real(other) / mrb_imag(other);
      n = mrb_imag(other) * (r*r+1);
      return mrb_complex_value(mrb,
        ((mrb_real(self) * r) + mrb_imag(self)) / n,
        ((mrb_imag(self) * r) - mrb_real(self)) / n
      );
    }
  case MRB_TT_FIXNUM:
    oreal = (mrb_float)mrb_fixnum(other);
    break;
  case MRB_TT_FLOAT:
    oreal = mrb_float(other);
    break;
  default:
    cpx_raise_not_numeric_error(mrb);
  }
  return mrb_complex_value(mrb,
    mrb_real(self) / oreal,
    mrb_imag(self) / oreal
  );
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
  mrb_define_method(mrb, complex, "*", cpx_mul, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, complex, "/", cpx_div, MRB_ARGS_REQ(1));
}

void
mrb_mruby_complex_gem_final(mrb_state* mrb)
{
}
