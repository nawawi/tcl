#include "tommath_private.h"
#ifdef BN_MP_COMPLEMENT_C
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

/* b = ~a */
mp_err mp_complement(const mp_int *a, mp_int *b)
{
   mp_err err = mp_neg(a, b);
   return (err == MP_OKAY) ? mp_sub_d(b, 1uL, b) : err;
}
#endif
