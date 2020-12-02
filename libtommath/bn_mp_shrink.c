#include "tommath_private.h"
#ifdef BN_MP_SHRINK_C
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

/* shrink a bignum */
mp_err mp_shrink(mp_int *a)
{
   mp_digit *tmp;
<<<<<<< HEAD
   int used = 1;

   if (a->used > 0) {
      used = a->used;
   }

   if (a->alloc != used) {
<<<<<<< HEAD
      if ((tmp = OPT_CAST(mp_digit) XREALLOC(a->dp, sizeof(mp_digit) * (size_t)used)) == NULL) {
=======
      if ((tmp = (mp_digit *) XREALLOC(a->dp,
                                       (size_t)a->alloc * sizeof (mp_digit),
                                       (size_t)used * sizeof(mp_digit))) == NULL) {
>>>>>>> upstream/master
=======
   int alloc = MP_MAX(MP_MIN_PREC, a->used);
   if (a->alloc != alloc) {
      if ((tmp = (mp_digit *) MP_REALLOC(a->dp,
                                         (size_t)a->alloc * sizeof(mp_digit),
                                         (size_t)alloc * sizeof(mp_digit))) == NULL) {
>>>>>>> upstream/master
         return MP_MEM;
      }
      a->dp    = tmp;
      a->alloc = alloc;
   }
   return MP_OKAY;
}
#endif
