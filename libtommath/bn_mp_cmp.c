#include "tommath_private.h"
#ifdef BN_MP_CMP_C
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
 * The library is free for all purposes without any express
 * guarantee it works.
 */

/* compare two ints (signed)*/
int mp_cmp(const mp_int *a, const mp_int *b)
=======
/* LibTomMath, multiple-precision integer library -- Tom St Denis */
/* SPDX-License-Identifier: Unlicense */

/* compare two ints (signed)*/
mp_ord mp_cmp(const mp_int *a, const mp_int *b)
>>>>>>> upstream/master
{
   /* compare based on sign */
   if (a->sign != b->sign) {
      if (a->sign == MP_NEG) {
         return MP_LT;
      } else {
         return MP_GT;
      }
   }

   /* compare digits */
   if (a->sign == MP_NEG) {
      /* if negative compare opposite direction */
      return mp_cmp_mag(b, a);
   } else {
      return mp_cmp_mag(a, b);
   }
}
#endif
<<<<<<< HEAD

/* ref:         $Format:%D$ */
/* git commit:  $Format:%H$ */
/* commit time: $Format:%ai$ */
=======
>>>>>>> upstream/master
