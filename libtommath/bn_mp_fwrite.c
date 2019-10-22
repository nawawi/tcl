#include "tommath_private.h"
#ifdef BN_MP_FWRITE_C
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

#ifndef LTM_NO_FILE
int mp_fwrite(const mp_int *a, int radix, FILE *stream)
{
   char *buf;
   int err, len, x;

   if ((err = mp_radix_size(a, radix, &len)) != MP_OKAY) {
      return err;
   }

<<<<<<< HEAD
   buf = OPT_CAST(char) XMALLOC((size_t)len);
=======
   buf = (char *) XMALLOC((size_t)len);
>>>>>>> upstream/master
   if (buf == NULL) {
      return MP_MEM;
   }

   if ((err = mp_toradix(a, buf, radix)) != MP_OKAY) {
<<<<<<< HEAD
      XFREE(buf);
=======
      XFREE(buf, len);
>>>>>>> upstream/master
      return err;
   }

   for (x = 0; x < len; x++) {
      if (fputc((int)buf[x], stream) == EOF) {
<<<<<<< HEAD
         XFREE(buf);
=======
         XFREE(buf, len);
>>>>>>> upstream/master
         return MP_VAL;
      }
   }

<<<<<<< HEAD
   XFREE(buf);
=======
   XFREE(buf, len);
>>>>>>> upstream/master
   return MP_OKAY;
}
#endif

#endif

/* ref:         $Format:%D$ */
/* git commit:  $Format:%H$ */
/* commit time: $Format:%ai$ */
