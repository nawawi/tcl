#include "tommath_private.h"
#ifdef BN_MP_FWRITE_C
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

#ifndef LTM_NO_FILE
int mp_fwrite(const mp_int *a, int radix, FILE *stream)
=======
/* LibTomMath, multiple-precision integer library -- Tom St Denis */
/* SPDX-License-Identifier: Unlicense */

#ifndef MP_NO_FILE
mp_err mp_fwrite(const mp_int *a, int radix, FILE *stream)
>>>>>>> upstream/master
{
   char *buf;
   mp_err err;
   int len;
   size_t written;

   /* TODO: this function is not in this PR */
   if (MP_HAS(MP_RADIX_SIZE_OVERESTIMATE)) {
      /* if ((err = mp_radix_size_overestimate(&t, base, &len)) != MP_OKAY)      goto LBL_ERR; */
   } else {
      if ((err = mp_radix_size(a, radix, &len)) != MP_OKAY) {
         return err;
      }
   }

<<<<<<< HEAD
<<<<<<< HEAD
   buf = OPT_CAST(char) XMALLOC((size_t)len);
=======
   buf = (char *) XMALLOC((size_t)len);
>>>>>>> upstream/master
=======
   buf = (char *) MP_MALLOC((size_t)len);
>>>>>>> upstream/master
   if (buf == NULL) {
      return MP_MEM;
   }

<<<<<<< HEAD
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
=======
   if ((err = mp_to_radix(a, buf, (size_t)len, &written, radix)) != MP_OKAY) {
      goto LBL_ERR;
   }

   if (fwrite(buf, written, 1uL, stream) != 1uL) {
      err = MP_ERR;
      goto LBL_ERR;
>>>>>>> upstream/master
   }
   err = MP_OKAY;


<<<<<<< HEAD
<<<<<<< HEAD
   XFREE(buf);
=======
   XFREE(buf, len);
>>>>>>> upstream/master
   return MP_OKAY;
=======
LBL_ERR:
   MP_FREE_BUFFER(buf, (size_t)len);
   return err;
>>>>>>> upstream/master
}
#endif

#endif
