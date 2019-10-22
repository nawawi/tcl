#include "tommath_private.h"
#ifdef BN_MP_ADDMOD_C
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

/* d = a + b (mod c) */
int mp_addmod(const mp_int *a, const mp_int *b, const mp_int *c, mp_int *d)
{
   int     res;
   mp_int  t;

   if ((res = mp_init(&t)) != MP_OKAY) {
      return res;
   }

   if ((res = mp_add(a, b, &t)) != MP_OKAY) {
      mp_clear(&t);
      return res;
   }
   res = mp_mod(&t, c, d);
   mp_clear(&t);
   return res;
}
#endif

/* ref:         $Format:%D$ */
/* git commit:  $Format:%H$ */
/* commit time: $Format:%ai$ */
