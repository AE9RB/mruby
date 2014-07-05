/*
** mruby/complex.h - mrb_value for complex numbers
**
** See Copyright Notice in mruby.h
*/

#ifndef MRUBY_COMPLEX_H
#define MRUBY_COMPLEX_H

#ifdef MRB_WORD_BOXING
# error MRB_COMPLEX not compatible with MRB_WORD_BOXING
#endif
#ifdef MRB_NAN_BOXING
# error MRB_COMPLEX not compatible with MRB_NAN_BOXING
#endif

#define MRB_TT_HAS_BASIC  MRB_TT_OBJECT

enum mrb_vtype {
  MRB_TT_FALSE = 0,   /*   0 */
  MRB_TT_FREE,        /*   1 */
  MRB_TT_TRUE,        /*   2 */
  MRB_TT_FIXNUM,      /*   3 */
  MRB_TT_SYMBOL,      /*   4 */
  MRB_TT_UNDEF,       /*   5 */
  MRB_TT_FLOAT,       /*   6 */
  MRB_TT_COMPLEX,     /*   7 */
  MRB_TT_CPTR,        /*   8 */
  MRB_TT_OBJECT,      /*   9 */
  MRB_TT_CLASS,       /*  10 */
  MRB_TT_MODULE,      /*  11 */
  MRB_TT_ICLASS,      /*  12 */
  MRB_TT_SCLASS,      /*  13 */
  MRB_TT_PROC,        /*  14 */
  MRB_TT_ARRAY,       /*  15 */
  MRB_TT_HASH,        /*  16 */
  MRB_TT_STRING,      /*  17 */
  MRB_TT_RANGE,       /*  18 */
  MRB_TT_EXCEPTION,   /*  19 */
  MRB_TT_FILE,        /*  20 */
  MRB_TT_ENV,         /*  21 */
  MRB_TT_DATA,        /*  22 */
  MRB_TT_FIBER,       /*  23 */
  MRB_TT_MAXDEFINE    /*  24 */
};

#ifdef MRB_ENDIAN_BIG
#define MRB_ENDIAN_LOHI(a,b) a b
#else
#define MRB_ENDIAN_LOHI(a,b) b a
#endif

#ifdef MRB_USE_FLOAT

typedef struct mrb_value {
  union {
    mrb_float f;
    void *p;
    mrb_int i;
    mrb_sym sym;
  } value;
  union {
    mrb_float fi;
    struct {
      MRB_ENDIAN_LOHI (
        uint16_t tt_nan;,
        uint16_t tt;
        )
    };
  };
} mrb_value;

#define mrb_complex_p(o) (0xFFFF != (o).tt_nan)
#define mrb_type(o)     (0xFFFF == (o).tt_nan ? (o).tt : MRB_TT_COMPLEX)

#define MRB_SET_VALUE(o, ttt, attr, v) do {\
  (o).tt_nan = 0xFFFF;\
  (o).tt = ttt;\
  (o).attr = v;\
} while (0)

#define MRB_SET_COMPLEX(o, real, imag) do {\
  if (imag != imag) {\
    (o).tt_nan = 0x7FC0;\
    (o).tt = 0;\
  } else {\
    (o).fi = imag;\
  }\
  (o).value.f = real;\
} while(0)

#else

typedef struct mrb_value {
  union {
    mrb_float f;
    void *p;
    mrb_int i;
    mrb_sym sym;
  } value;
  union {
    mrb_float fi;
    struct {
      MRB_ENDIAN_LOHI (
        uint32_t tt_nan;,
        uint32_t tt;
        )
    };
  };
} mrb_value;

#define mrb_complex_p(o) (0xFFFFFFFF != (o).tt_nan)
#define mrb_type(o)     (0xFFFFFFFF == (o).tt_nan ? (o).tt : MRB_TT_COMPLEX)

#define MRB_SET_VALUE(o, ttt, attr, v) do {\
  (o).tt_nan = 0xFFFFFFFF;\
  (o).tt = ttt;\
  (o).attr = v;\
} while (0)

#define MRB_SET_COMPLEX(o, real, imag) do {\
  if (imag != imag) {\
    (o).tt_nan = 0x7FF80000;\
    (o).tt = 0;\
  } else {\
    (o).fi = imag;\
  }\
  (o).value.f = real;\
} while(0)

#endif

#define mrb_ptr(o)      (o).value.p
#define mrb_float(o)    (o).value.f
#define mrb_real(o)     (o).value.f
#define mrb_imag(o)     (o).fi

static inline mrb_value
mrb_float_value(struct mrb_state *mrb, mrb_float f)
{
  mrb_value v;
  MRB_SET_VALUE(v, MRB_TT_FLOAT, value.f, f);
  return v;
}
#define mrb_float_pool(mrb,f) mrb_float_value(mrb,f)

static inline mrb_value
mrb_complex_value(struct mrb_state *mrb, mrb_float real, mrb_float imag)
{
  mrb_value v;
  MRB_SET_COMPLEX(v,real,imag);
  return v;
}

#endif /* MRUBY_COMPLEX_H */
