#include "tommath_private.h"
#ifdef BN_MP_DR_SETUP_C
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

/* determines the setup value */
void mp_dr_setup(const mp_int *a, mp_digit *d)
{
   /* the casts are required if MP_DIGIT_BIT is one less than
    * the number of bits in a mp_digit [e.g. MP_DIGIT_BIT==31]
    */
   *d = (mp_digit)(((mp_word)1 << (mp_word)MP_DIGIT_BIT) - (mp_word)a->dp[0]);
}

#endif
