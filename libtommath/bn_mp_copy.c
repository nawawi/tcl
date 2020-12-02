#include "tommath_private.h"
#ifdef BN_MP_COPY_C
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

/* copy, b = a */
mp_err mp_copy(const mp_int *a, mp_int *b)
{
   int n;
   mp_digit *tmpa, *tmpb;
   mp_err err;

   /* if dst == src do nothing */
   if (a == b) {
      return MP_OKAY;
   }

   /* grow dest */
   if (b->alloc < a->used) {
      if ((err = mp_grow(b, a->used)) != MP_OKAY) {
         return err;
      }
   }

   /* zero b and copy the parameters over */
   /* pointer aliases */

   /* source */
   tmpa = a->dp;

   /* destination */
   tmpb = b->dp;

   /* copy all the digits */
   for (n = 0; n < a->used; n++) {
      *tmpb++ = *tmpa++;
   }

   /* clear high digits */
   MP_ZERO_DIGITS(tmpb, b->used - n);

   /* copy used count and sign */
   b->used = a->used;
   b->sign = a->sign;
   return MP_OKAY;
}
#endif
