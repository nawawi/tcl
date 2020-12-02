#include "tommath_private.h"
#ifdef BN_MP_AND_C
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

/* AND two ints together */
int mp_and(const mp_int *a, const mp_int *b, mp_int *c)
{
   int     res, ix, px;
   mp_int  t;
   const mp_int *x;

   if (a->used > b->used) {
      if ((res = mp_init_copy(&t, a)) != MP_OKAY) {
         return res;
      }
      px = b->used;
      x = b;
   } else {
      if ((res = mp_init_copy(&t, b)) != MP_OKAY) {
         return res;
      }
      px = a->used;
      x = a;
   }

   for (ix = 0; ix < px; ix++) {
      t.dp[ix] &= x->dp[ix];
   }

   /* zero digits above the last from the smallest mp_int */
   for (; ix < t.used; ix++) {
      t.dp[ix] = 0;
   }

   mp_clamp(&t);
   mp_exch(c, &t);
   mp_clear(&t);
   return MP_OKAY;
}
#endif

/* ref:         $Format:%D$ */
/* git commit:  $Format:%H$ */
/* commit time: $Format:%ai$ */
=======
/* LibTomMath, multiple-precision integer library -- Tom St Denis */
/* SPDX-License-Identifier: Unlicense */

/* two complement and */
mp_err mp_and(const mp_int *a, const mp_int *b, mp_int *c)
{
   int used = MP_MAX(a->used, b->used) + 1, i;
   mp_err err;
   mp_digit ac = 1, bc = 1, cc = 1;
   mp_sign csign = ((a->sign == MP_NEG) && (b->sign == MP_NEG)) ? MP_NEG : MP_ZPOS;

   if (c->alloc < used) {
      if ((err = mp_grow(c, used)) != MP_OKAY) {
         return err;
      }
   }

   for (i = 0; i < used; i++) {
      mp_digit x, y;

      /* convert to two complement if negative */
      if (a->sign == MP_NEG) {
         ac += (i >= a->used) ? MP_MASK : (~a->dp[i] & MP_MASK);
         x = ac & MP_MASK;
         ac >>= MP_DIGIT_BIT;
      } else {
         x = (i >= a->used) ? 0uL : a->dp[i];
      }

      /* convert to two complement if negative */
      if (b->sign == MP_NEG) {
         bc += (i >= b->used) ? MP_MASK : (~b->dp[i] & MP_MASK);
         y = bc & MP_MASK;
         bc >>= MP_DIGIT_BIT;
      } else {
         y = (i >= b->used) ? 0uL : b->dp[i];
      }

      c->dp[i] = x & y;

      /* convert to to sign-magnitude if negative */
      if (csign == MP_NEG) {
         cc += ~c->dp[i] & MP_MASK;
         c->dp[i] = cc & MP_MASK;
         cc >>= MP_DIGIT_BIT;
      }
   }

   c->used = used;
   c->sign = csign;
   mp_clamp(c);
   return MP_OKAY;
}
#endif
>>>>>>> upstream/master
