#include "tommath_private.h"
#ifdef BN_MP_SQR_C
<<<<<<< HEAD
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
=======
/* LibTomMath, multiple-precision integer library -- Tom St Denis */
/* SPDX-License-Identifier: Unlicense */
>>>>>>> upstream/master

/* computes b = a*a */
mp_err mp_sqr(const mp_int *a, mp_int *b)
{
   mp_err err;
   if (MP_HAS(S_MP_TOOM_SQR) && /* use Toom-Cook? */
       (a->used >= MP_TOOM_SQR_CUTOFF)) {
      err = s_mp_toom_sqr(a, b);
   } else if (MP_HAS(S_MP_KARATSUBA_SQR) &&  /* Karatsuba? */
              (a->used >= MP_KARATSUBA_SQR_CUTOFF)) {
      err = s_mp_karatsuba_sqr(a, b);
   } else if (MP_HAS(S_MP_SQR_FAST) && /* can we use the fast comba multiplier? */
              (((a->used * 2) + 1) < MP_WARRAY) &&
              (a->used < (MP_MAXFAST / 2))) {
      err = s_mp_sqr_fast(a, b);
   } else if (MP_HAS(S_MP_SQR)) {
      err = s_mp_sqr(a, b);
   } else {
      err = MP_VAL;
   }
   b->sign = MP_ZPOS;
   return err;
}
#endif
