#include "tommath_private.h"
#ifdef BN_MP_SUB_C
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

/* high level subtraction (handles signs) */
mp_err mp_sub(const mp_int *a, const mp_int *b, mp_int *c)
{
   mp_sign sa = a->sign, sb = b->sign;
   mp_err err;

   if (sa != sb) {
      /* subtract a negative from a positive, OR */
      /* subtract a positive from a negative. */
      /* In either case, ADD their magnitudes, */
      /* and use the sign of the first number. */
      c->sign = sa;
      err = s_mp_add(a, b, c);
   } else {
      /* subtract a positive from a positive, OR */
      /* subtract a negative from a negative. */
      /* First, take the difference between their */
      /* magnitudes, then... */
      if (mp_cmp_mag(a, b) != MP_LT) {
         /* Copy the sign from the first */
         c->sign = sa;
         /* The first has a larger or equal magnitude */
         err = s_mp_sub(a, b, c);
      } else {
         /* The result has the *opposite* sign from */
         /* the first number. */
         c->sign = (sa == MP_ZPOS) ? MP_NEG : MP_ZPOS;
         /* The second has a larger magnitude */
         err = s_mp_sub(b, a, c);
      }
   }
   return err;
}

#endif
