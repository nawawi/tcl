/* LibTomMath, multiple-precision integer library -- Tom St Denis
 *
 * LibTomMath is a library that provides multiple-precision
 * integer arithmetic as well as number theoretic functionality.
 *
 * The library was designed directly after the MPI library by
 * Michael Fromberger but has been written from scratch with
 * additional optimizations in place.
 *
<<<<<<< HEAD
 * The library is free for all purposes without any express
 * guarantee it works.
=======
 * SPDX-License-Identifier: Unlicense
>>>>>>> upstream/master
 */
#ifndef TOMMATH_PRIV_H_
#define TOMMATH_PRIV_H_

#include <tommath.h>
<<<<<<< HEAD
#include <ctype.h>
=======
>>>>>>> upstream/master

#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif

#ifdef __cplusplus
extern "C" {
<<<<<<< HEAD

/* C++ compilers don't like assigning void * to mp_digit * */
#define OPT_CAST(x) (x *)

#else

/* C on the other hand doesn't care */
#define OPT_CAST(x)

=======
>>>>>>> upstream/master
#endif

/* define heap macros */
#ifndef XMALLOC
/* default to libc stuff */
<<<<<<< HEAD
#   define XMALLOC   malloc
#   define XFREE     free
#   define XREALLOC  realloc
#   define XCALLOC   calloc
#elif 0
/* prototypes for our heap functions */
extern void *XMALLOC(size_t n);
extern void *XREALLOC(void *p, size_t n);
extern void *XCALLOC(size_t n, size_t s);
extern void XFREE(void *p);
#endif

=======
#   define XMALLOC(size)                   malloc(size)
#   define XFREE(mem, size)                free(mem)
#   define XREALLOC(mem, oldsize, newsize) realloc(mem, newsize)
#elif 0
/* prototypes for our heap functions */
extern void *XMALLOC(size_t size);
extern void *XREALLOC(void *mem, size_t oldsize, size_t newsize);
extern void XFREE(void *mem, size_t size);
#endif

/* you'll have to tune these... */
#define KARATSUBA_MUL_CUTOFF 80      /* Min. number of digits before Karatsuba multiplication is used. */
#define KARATSUBA_SQR_CUTOFF 120     /* Min. number of digits before Karatsuba squaring is used. */
#define TOOM_MUL_CUTOFF      350     /* no optimal values of these are known yet so set em high */
#define TOOM_SQR_CUTOFF      400

/* size of comba arrays, should be at least 2 * 2**(BITS_PER_WORD - BITS_PER_DIGIT*2) */
#define MP_WARRAY               (1u << (((sizeof(mp_word) * CHAR_BIT) - (2 * DIGIT_BIT)) + 1))

/* ---> Basic Manipulations <--- */
#define IS_ZERO(a) ((a)->used == 0)
#define IS_EVEN(a) (((a)->used == 0) || (((a)->dp[0] & 1u) == 0u))
#define IS_ODD(a)  (((a)->used > 0) && (((a)->dp[0] & 1u) == 1u))

>>>>>>> upstream/master
/* lowlevel functions, do not call! */
int s_mp_add(const mp_int *a, const mp_int *b, mp_int *c);
int s_mp_sub(const mp_int *a, const mp_int *b, mp_int *c);
#define s_mp_mul(a, b, c) s_mp_mul_digs(a, b, c, (a)->used + (b)->used + 1)
int fast_s_mp_mul_digs(const mp_int *a, const mp_int *b, mp_int *c, int digs);
int s_mp_mul_digs(const mp_int *a, const mp_int *b, mp_int *c, int digs);
int fast_s_mp_mul_high_digs(const mp_int *a, const mp_int *b, mp_int *c, int digs);
int s_mp_mul_high_digs(const mp_int *a, const mp_int *b, mp_int *c, int digs);
int fast_s_mp_sqr(const mp_int *a, mp_int *b);
int s_mp_sqr(const mp_int *a, mp_int *b);
int mp_karatsuba_mul(const mp_int *a, const mp_int *b, mp_int *c);
int mp_toom_mul(const mp_int *a, const mp_int *b, mp_int *c);
int mp_karatsuba_sqr(const mp_int *a, mp_int *b);
int mp_toom_sqr(const mp_int *a, mp_int *b);
int fast_mp_invmod(const mp_int *a, const mp_int *b, mp_int *c);
int mp_invmod_slow(const mp_int *a, const mp_int *b, mp_int *c);
int fast_mp_montgomery_reduce(mp_int *x, const mp_int *n, mp_digit rho);
int mp_exptmod_fast(const mp_int *G, const mp_int *X, const mp_int *P, mp_int *Y, int redmode);
int s_mp_exptmod(const mp_int *G, const mp_int *X, const mp_int *P, mp_int *Y, int redmode);
void bn_reverse(unsigned char *s, int len);

<<<<<<< HEAD
<<<<<<< HEAD
extern const char *mp_s_rmap;
=======
extern const char *const mp_s_rmap;
>>>>>>> upstream/master
=======
extern const char *const mp_s_rmap;
>>>>>>> upstream/master
extern const unsigned char mp_s_rmap_reverse[];
extern const size_t mp_s_rmap_reverse_sz;

/* Fancy macro to set an MPI from another type.
 * There are several things assumed:
<<<<<<< HEAD
 *  x is the counter and unsigned
=======
 *  x is the counter
>>>>>>> upstream/master
 *  a is the pointer to the MPI
 *  b is the original value that should be set in the MPI.
 */
#define MP_SET_XLONG(func_name, type)                    \
int func_name (mp_int * a, type b)                       \
{                                                        \
<<<<<<< HEAD
  unsigned int  x;                                       \
  int           res;                                     \
                                                         \
  mp_zero (a);                                           \
                                                         \
  /* set four bits at a time */                          \
  for (x = 0; x < (sizeof(type) * 2u); x++) {            \
    /* shift the number up four bits */                  \
    if ((res = mp_mul_2d (a, 4, a)) != MP_OKAY) {        \
      return res;                                        \
    }                                                    \
                                                         \
    /* OR in the top four bits of the source */          \
    a->dp[0] |= (mp_digit)(b >> ((sizeof(type) * 8u) - 4u)) & 15uL;\
                                                         \
    /* shift the source up to the next four bits */      \
    b <<= 4;                                             \
                                                         \
    /* ensure that digits are not clamped off */         \
    a->used += 1;                                        \
  }                                                      \
  mp_clamp (a);                                          \
  return MP_OKAY;                                        \
=======
   int x = 0;                                            \
   int new_size = (((CHAR_BIT * sizeof(type)) + DIGIT_BIT) - 1) / DIGIT_BIT; \
   int res = mp_grow(a, new_size);                       \
   if (res == MP_OKAY) {                                 \
     mp_zero(a);                                         \
     while (b != 0u) {                                   \
        a->dp[x++] = ((mp_digit)b & MP_MASK);            \
        if ((CHAR_BIT * sizeof (b)) <= DIGIT_BIT) { break; } \
        b >>= ((CHAR_BIT * sizeof (b)) <= DIGIT_BIT ? 0 : DIGIT_BIT); \
     }                                                   \
     a->used = x;                                        \
   }                                                     \
   return res;                                           \
>>>>>>> upstream/master
}

#ifdef __cplusplus
}
#endif

#endif


/* ref:         $Format:%D$ */
/* git commit:  $Format:%H$ */
/* commit time: $Format:%ai$ */
