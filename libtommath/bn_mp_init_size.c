#include "tommath_private.h"
#ifdef BN_MP_INIT_SIZE_C
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

/* init an mp_init for a given size */
mp_err mp_init_size(mp_int *a, int size)
{
   size = MP_MAX(MP_MIN_PREC, size);

   /* alloc mem */
<<<<<<< HEAD
<<<<<<< HEAD
   a->dp = OPT_CAST(mp_digit) XMALLOC(sizeof(mp_digit) * (size_t)size);
=======
   a->dp = (mp_digit *) XMALLOC((size_t)size * sizeof(mp_digit));
>>>>>>> upstream/master
=======
   a->dp = (mp_digit *) MP_CALLOC((size_t)size, sizeof(mp_digit));
>>>>>>> upstream/master
   if (a->dp == NULL) {
      return MP_MEM;
   }

   /* set the members */
   a->used  = 0;
   a->alloc = size;
   a->sign  = MP_ZPOS;

   return MP_OKAY;
}
#endif
