#include "tommath_private.h"
#ifdef BN_MP_PRIME_RABIN_MILLER_TRIALS_C
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

static const struct {
   int k, t;
} sizes[] = {
<<<<<<< HEAD
<<<<<<< HEAD
   {   128,    28 },
=======
   {    80,    -1 }, /* Use deterministic algorithm for size <= 80 bits */
   {    81,    39 },
   {    96,    37 },
   {   128,    32 },
   {   160,    27 },
   {   192,    21 },
>>>>>>> upstream/master
   {   256,    16 },
   {   384,    10 },
   {   512,     7 },
   {   640,     6 },
   {   768,     5 },
   {   896,     4 },
<<<<<<< HEAD
   {  1024,     4 }
=======
   {  1024,     4 },
   {  2048,     2 },
   {  4096,     1 },
>>>>>>> upstream/master
=======
   {    80, -1 }, /* Use deterministic algorithm for size <= 80 bits */
   {    81, 37 }, /* max. error = 2^(-96)*/
   {    96, 32 }, /* max. error = 2^(-96)*/
   {   128, 40 }, /* max. error = 2^(-112)*/
   {   160, 35 }, /* max. error = 2^(-112)*/
   {   256, 27 }, /* max. error = 2^(-128)*/
   {   384, 16 }, /* max. error = 2^(-128)*/
   {   512, 18 }, /* max. error = 2^(-160)*/
   {   768, 11 }, /* max. error = 2^(-160)*/
   {   896, 10 }, /* max. error = 2^(-160)*/
   {  1024, 12 }, /* max. error = 2^(-192)*/
   {  1536, 8  }, /* max. error = 2^(-192)*/
   {  2048, 6  }, /* max. error = 2^(-192)*/
   {  3072, 4  }, /* max. error = 2^(-192)*/
   {  4096, 5  }, /* max. error = 2^(-256)*/
   {  5120, 4  }, /* max. error = 2^(-256)*/
   {  6144, 4  }, /* max. error = 2^(-256)*/
   {  8192, 3  }, /* max. error = 2^(-256)*/
   {  9216, 3  }, /* max. error = 2^(-256)*/
   { 10240, 2  }  /* For bigger keysizes use always at least 2 Rounds */
>>>>>>> upstream/master
};

/* returns # of RM trials required for a given bit size */
int mp_prime_rabin_miller_trials(int size)
{
   int x;

   for (x = 0; x < (int)(sizeof(sizes)/(sizeof(sizes[0]))); x++) {
      if (sizes[x].k == size) {
         return sizes[x].t;
      } else if (sizes[x].k > size) {
         return (x == 0) ? sizes[0].t : sizes[x - 1].t;
      }
   }
   return sizes[x-1].t;
}


#endif
