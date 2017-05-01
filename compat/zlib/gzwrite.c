/* gzwrite.c -- zlib functions for writing gzip files
<<<<<<< HEAD
 * Copyright (C) 2004, 2005, 2010, 2011, 2012, 2013 Mark Adler
=======
 * Copyright (C) 2004-2017 Mark Adler
>>>>>>> upstream/master
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "gzguts.h"

/* Local functions */
local int gz_init OF((gz_statep));
local int gz_comp OF((gz_statep, int));
local int gz_zero OF((gz_statep, z_off64_t));
<<<<<<< HEAD

/* Initialize state for writing a gzip file.  Mark initialization by setting
   state->size to non-zero.  Return -1 on failure or 0 on success. */
=======
local z_size_t gz_write OF((gz_statep, voidpc, z_size_t));

/* Initialize state for writing a gzip file.  Mark initialization by setting
   state->size to non-zero.  Return -1 on a memory allocation failure, or 0 on
   success. */
>>>>>>> upstream/master
local int gz_init(state)
    gz_statep state;
{
    int ret;
    z_streamp strm = &(state->strm);

<<<<<<< HEAD
    /* allocate input buffer */
    state->in = (unsigned char *)malloc(state->want);
=======
    /* allocate input buffer (double size for gzprintf) */
    state->in = (unsigned char *)malloc(state->want << 1);
>>>>>>> upstream/master
    if (state->in == NULL) {
        gz_error(state, Z_MEM_ERROR, "out of memory");
        return -1;
    }

    /* only need output buffer and deflate state if compressing */
    if (!state->direct) {
        /* allocate output buffer */
        state->out = (unsigned char *)malloc(state->want);
        if (state->out == NULL) {
            free(state->in);
            gz_error(state, Z_MEM_ERROR, "out of memory");
            return -1;
        }

        /* allocate deflate memory, set up for gzip compression */
        strm->zalloc = Z_NULL;
        strm->zfree = Z_NULL;
        strm->opaque = Z_NULL;
        ret = deflateInit2(strm, state->level, Z_DEFLATED,
                           MAX_WBITS + 16, DEF_MEM_LEVEL, state->strategy);
        if (ret != Z_OK) {
            free(state->out);
            free(state->in);
            gz_error(state, Z_MEM_ERROR, "out of memory");
            return -1;
        }
<<<<<<< HEAD
=======
        strm->next_in = NULL;
>>>>>>> upstream/master
    }

    /* mark state as initialized */
    state->size = state->want;

    /* initialize write buffer if compressing */
    if (!state->direct) {
        strm->avail_out = state->size;
        strm->next_out = state->out;
        state->x.next = strm->next_out;
    }
    return 0;
}

/* Compress whatever is at avail_in and next_in and write to the output file.
<<<<<<< HEAD
   Return -1 if there is an error writing to the output file, otherwise 0.
   flush is assumed to be a valid deflate() flush value.  If flush is Z_FINISH,
   then the deflate() state is reset to start a new gzip stream.  If gz->direct
   is true, then simply write to the output file without compressing, and
   ignore flush. */
=======
   Return -1 if there is an error writing to the output file or if gz_init()
   fails to allocate memory, otherwise 0.  flush is assumed to be a valid
   deflate() flush value.  If flush is Z_FINISH, then the deflate() state is
   reset to start a new gzip stream.  If gz->direct is true, then simply write
   to the output file without compressing, and ignore flush. */
>>>>>>> upstream/master
local int gz_comp(state, flush)
    gz_statep state;
    int flush;
{
<<<<<<< HEAD
    int ret, got;
    unsigned have;
=======
    int ret, writ;
    unsigned have, put, max = ((unsigned)-1 >> 2) + 1;
>>>>>>> upstream/master
    z_streamp strm = &(state->strm);

    /* allocate memory if this is the first time through */
    if (state->size == 0 && gz_init(state) == -1)
        return -1;

    /* write directly if requested */
    if (state->direct) {
<<<<<<< HEAD
        got = write(state->fd, strm->next_in, strm->avail_in);
        if (got < 0 || (unsigned)got != strm->avail_in) {
            gz_error(state, Z_ERRNO, zstrerror());
            return -1;
        }
        strm->avail_in = 0;
=======
        while (strm->avail_in) {
            put = strm->avail_in > max ? max : strm->avail_in;
            writ = write(state->fd, strm->next_in, put);
            if (writ < 0) {
                gz_error(state, Z_ERRNO, zstrerror());
                return -1;
            }
            strm->avail_in -= (unsigned)writ;
            strm->next_in += writ;
        }
>>>>>>> upstream/master
        return 0;
    }

    /* run deflate() on provided input until it produces no more output */
    ret = Z_OK;
    do {
        /* write out current buffer contents if full, or if flushing, but if
           doing Z_FINISH then don't write until we get to Z_STREAM_END */
        if (strm->avail_out == 0 || (flush != Z_NO_FLUSH &&
            (flush != Z_FINISH || ret == Z_STREAM_END))) {
<<<<<<< HEAD
            have = (unsigned)(strm->next_out - state->x.next);
            if (have && ((got = write(state->fd, state->x.next, have)) < 0 ||
                         (unsigned)got != have)) {
                gz_error(state, Z_ERRNO, zstrerror());
                return -1;
=======
            while (strm->next_out > state->x.next) {
                put = strm->next_out - state->x.next > (int)max ? max :
                      (unsigned)(strm->next_out - state->x.next);
                writ = write(state->fd, state->x.next, put);
                if (writ < 0) {
                    gz_error(state, Z_ERRNO, zstrerror());
                    return -1;
                }
                state->x.next += writ;
>>>>>>> upstream/master
            }
            if (strm->avail_out == 0) {
                strm->avail_out = state->size;
                strm->next_out = state->out;
<<<<<<< HEAD
            }
            state->x.next = strm->next_out;
=======
                state->x.next = state->out;
            }
>>>>>>> upstream/master
        }

        /* compress */
        have = strm->avail_out;
        ret = deflate(strm, flush);
        if (ret == Z_STREAM_ERROR) {
            gz_error(state, Z_STREAM_ERROR,
                      "internal error: deflate stream corrupt");
            return -1;
        }
        have -= strm->avail_out;
    } while (have);

    /* if that completed a deflate stream, allow another to start */
    if (flush == Z_FINISH)
        deflateReset(strm);

    /* all done, no errors */
    return 0;
}

<<<<<<< HEAD
/* Compress len zeros to output.  Return -1 on error, 0 on success. */
=======
/* Compress len zeros to output.  Return -1 on a write error or memory
   allocation failure by gz_comp(), or 0 on success. */
>>>>>>> upstream/master
local int gz_zero(state, len)
    gz_statep state;
    z_off64_t len;
{
    int first;
    unsigned n;
    z_streamp strm = &(state->strm);

    /* consume whatever's left in the input buffer */
    if (strm->avail_in && gz_comp(state, Z_NO_FLUSH) == -1)
        return -1;

    /* compress len zeros (len guaranteed > 0) */
    first = 1;
    while (len) {
        n = GT_OFF(state->size) || (z_off64_t)state->size > len ?
            (unsigned)len : state->size;
        if (first) {
            memset(state->in, 0, n);
            first = 0;
        }
        strm->avail_in = n;
        strm->next_in = state->in;
        state->x.pos += n;
        if (gz_comp(state, Z_NO_FLUSH) == -1)
            return -1;
        len -= n;
    }
    return 0;
}

<<<<<<< HEAD
/* -- see zlib.h -- */
int ZEXPORT gzwrite(file, buf, len)
    gzFile file;
    voidpc buf;
    unsigned len;
{
    unsigned put = len;
    gz_statep state;
    z_streamp strm;

    /* get internal structure */
    if (file == NULL)
        return 0;
    state = (gz_statep)file;
    strm = &(state->strm);

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return 0;

    /* since an int is returned, make sure len fits in one, otherwise return
       with an error (this avoids the flaw in the interface) */
    if ((int)len < 0) {
        gz_error(state, Z_DATA_ERROR, "requested length does not fit in int");
        return 0;
    }
=======
/* Write len bytes from buf to file.  Return the number of bytes written.  If
   the returned value is less than len, then there was an error. */
local z_size_t gz_write(state, buf, len)
    gz_statep state;
    voidpc buf;
    z_size_t len;
{
    z_size_t put = len;
>>>>>>> upstream/master

    /* if len is zero, avoid unnecessary operations */
    if (len == 0)
        return 0;

    /* allocate memory if this is the first time through */
    if (state->size == 0 && gz_init(state) == -1)
        return 0;

    /* check for seek request */
    if (state->seek) {
        state->seek = 0;
        if (gz_zero(state, state->skip) == -1)
            return 0;
    }

    /* for small len, copy to input buffer, otherwise compress directly */
    if (len < state->size) {
        /* copy to input buffer, compress when full */
        do {
            unsigned have, copy;

<<<<<<< HEAD
            if (strm->avail_in == 0)
                strm->next_in = state->in;
            have = (unsigned)((strm->next_in + strm->avail_in) - state->in);
=======
            if (state->strm.avail_in == 0)
                state->strm.next_in = state->in;
            have = (unsigned)((state->strm.next_in + state->strm.avail_in) -
                              state->in);
>>>>>>> upstream/master
            copy = state->size - have;
            if (copy > len)
                copy = len;
            memcpy(state->in + have, buf, copy);
<<<<<<< HEAD
            strm->avail_in += copy;
=======
            state->strm.avail_in += copy;
>>>>>>> upstream/master
            state->x.pos += copy;
            buf = (const char *)buf + copy;
            len -= copy;
            if (len && gz_comp(state, Z_NO_FLUSH) == -1)
                return 0;
        } while (len);
    }
    else {
        /* consume whatever's left in the input buffer */
<<<<<<< HEAD
        if (strm->avail_in && gz_comp(state, Z_NO_FLUSH) == -1)
            return 0;

        /* directly compress user buffer to file */
        strm->avail_in = len;
        strm->next_in = (z_const Bytef *)buf;
        state->x.pos += len;
        if (gz_comp(state, Z_NO_FLUSH) == -1)
            return 0;
    }

    /* input was all buffered or compressed (put will fit in int) */
    return (int)put;
=======
        if (state->strm.avail_in && gz_comp(state, Z_NO_FLUSH) == -1)
            return 0;

        /* directly compress user buffer to file */
        state->strm.next_in = (z_const Bytef *)buf;
        do {
            unsigned n = (unsigned)-1;
            if (n > len)
                n = len;
            state->strm.avail_in = n;
            state->x.pos += n;
            if (gz_comp(state, Z_NO_FLUSH) == -1)
                return 0;
            len -= n;
        } while (len);
    }

    /* input was all buffered or compressed */
    return put;
}

/* -- see zlib.h -- */
int ZEXPORT gzwrite(file, buf, len)
    gzFile file;
    voidpc buf;
    unsigned len;
{
    gz_statep state;

    /* get internal structure */
    if (file == NULL)
        return 0;
    state = (gz_statep)file;

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return 0;

    /* since an int is returned, make sure len fits in one, otherwise return
       with an error (this avoids a flaw in the interface) */
    if ((int)len < 0) {
        gz_error(state, Z_DATA_ERROR, "requested length does not fit in int");
        return 0;
    }

    /* write len bytes from buf (the return value will fit in an int) */
    return (int)gz_write(state, buf, len);
}

/* -- see zlib.h -- */
z_size_t ZEXPORT gzfwrite(buf, size, nitems, file)
    voidpc buf;
    z_size_t size;
    z_size_t nitems;
    gzFile file;
{
    z_size_t len;
    gz_statep state;

    /* get internal structure */
    if (file == NULL)
        return 0;
    state = (gz_statep)file;

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return 0;

    /* compute bytes to read -- error on overflow */
    len = nitems * size;
    if (size && len / size != nitems) {
        gz_error(state, Z_STREAM_ERROR, "request does not fit in a size_t");
        return 0;
    }

    /* write len bytes to buf, return the number of full items written */
    return len ? gz_write(state, buf, len) / size : 0;
>>>>>>> upstream/master
}

/* -- see zlib.h -- */
int ZEXPORT gzputc(file, c)
    gzFile file;
    int c;
{
    unsigned have;
    unsigned char buf[1];
    gz_statep state;
    z_streamp strm;

    /* get internal structure */
    if (file == NULL)
        return -1;
    state = (gz_statep)file;
    strm = &(state->strm);

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return -1;

    /* check for seek request */
    if (state->seek) {
        state->seek = 0;
        if (gz_zero(state, state->skip) == -1)
            return -1;
    }

    /* try writing to input buffer for speed (state->size == 0 if buffer not
       initialized) */
    if (state->size) {
        if (strm->avail_in == 0)
            strm->next_in = state->in;
        have = (unsigned)((strm->next_in + strm->avail_in) - state->in);
        if (have < state->size) {
<<<<<<< HEAD
            state->in[have] = c;
=======
            state->in[have] = (unsigned char)c;
>>>>>>> upstream/master
            strm->avail_in++;
            state->x.pos++;
            return c & 0xff;
        }
    }

    /* no room in buffer or not initialized, use gz_write() */
<<<<<<< HEAD
    buf[0] = c;
    if (gzwrite(file, buf, 1) != 1)
=======
    buf[0] = (unsigned char)c;
    if (gz_write(state, buf, 1) != 1)
>>>>>>> upstream/master
        return -1;
    return c & 0xff;
}

/* -- see zlib.h -- */
int ZEXPORT gzputs(file, str)
    gzFile file;
    const char *str;
{
    int ret;
<<<<<<< HEAD
    unsigned len;

    /* write string */
    len = (unsigned)strlen(str);
    ret = gzwrite(file, str, len);
=======
    z_size_t len;
    gz_statep state;

    /* get internal structure */
    if (file == NULL)
        return -1;
    state = (gz_statep)file;

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return -1;

    /* write string */
    len = strlen(str);
    ret = gz_write(state, str, len);
>>>>>>> upstream/master
    return ret == 0 && len != 0 ? -1 : ret;
}

#if defined(STDC) || defined(Z_HAVE_STDARG_H)
#include <stdarg.h>

/* -- see zlib.h -- */
int ZEXPORTVA gzvprintf(gzFile file, const char *format, va_list va)
{
<<<<<<< HEAD
    int size, len;
=======
    int len;
    unsigned left;
    char *next;
>>>>>>> upstream/master
    gz_statep state;
    z_streamp strm;

    /* get internal structure */
    if (file == NULL)
<<<<<<< HEAD
        return -1;
=======
        return Z_STREAM_ERROR;
>>>>>>> upstream/master
    state = (gz_statep)file;
    strm = &(state->strm);

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
<<<<<<< HEAD
        return 0;

    /* make sure we have some buffer space */
    if (state->size == 0 && gz_init(state) == -1)
        return 0;
=======
        return Z_STREAM_ERROR;

    /* make sure we have some buffer space */
    if (state->size == 0 && gz_init(state) == -1)
        return state->err;
>>>>>>> upstream/master

    /* check for seek request */
    if (state->seek) {
        state->seek = 0;
        if (gz_zero(state, state->skip) == -1)
<<<<<<< HEAD
            return 0;
    }

    /* consume whatever's left in the input buffer */
    if (strm->avail_in && gz_comp(state, Z_NO_FLUSH) == -1)
        return 0;

    /* do the printf() into the input buffer, put length in len */
    size = (int)(state->size);
    state->in[size - 1] = 0;
#ifdef NO_vsnprintf
#  ifdef HAS_vsprintf_void
    (void)vsprintf((char *)(state->in), format, va);
    for (len = 0; len < size; len++)
        if (state->in[len] == 0) break;
#  else
    len = vsprintf((char *)(state->in), format, va);
#  endif
#else
#  ifdef HAS_vsnprintf_void
    (void)vsnprintf((char *)(state->in), size, format, va);
    len = strlen((char *)(state->in));
#  else
    len = vsnprintf((char *)(state->in), size, format, va);
=======
            return state->err;
    }

    /* do the printf() into the input buffer, put length in len -- the input
       buffer is double-sized just for this function, so there is guaranteed to
       be state->size bytes available after the current contents */
    if (strm->avail_in == 0)
        strm->next_in = state->in;
    next = (char *)(state->in + (strm->next_in - state->in) + strm->avail_in);
    next[state->size - 1] = 0;
#ifdef NO_vsnprintf
#  ifdef HAS_vsprintf_void
    (void)vsprintf(next, format, va);
    for (len = 0; len < state->size; len++)
        if (next[len] == 0) break;
#  else
    len = vsprintf(next, format, va);
#  endif
#else
#  ifdef HAS_vsnprintf_void
    (void)vsnprintf(next, state->size, format, va);
    len = strlen(next);
#  else
    len = vsnprintf(next, state->size, format, va);
>>>>>>> upstream/master
#  endif
#endif

    /* check that printf() results fit in buffer */
<<<<<<< HEAD
    if (len <= 0 || len >= (int)size || state->in[size - 1] != 0)
        return 0;

    /* update buffer and position, defer compression until needed */
    strm->avail_in = (unsigned)len;
    strm->next_in = state->in;
    state->x.pos += len;
=======
    if (len == 0 || (unsigned)len >= state->size || next[state->size - 1] != 0)
        return 0;

    /* update buffer and position, compress first half if past that */
    strm->avail_in += (unsigned)len;
    state->x.pos += len;
    if (strm->avail_in >= state->size) {
        left = strm->avail_in - state->size;
        strm->avail_in = state->size;
        if (gz_comp(state, Z_NO_FLUSH) == -1)
            return state->err;
        memcpy(state->in, state->in + state->size, left);
        strm->next_in = state->in;
        strm->avail_in = left;
    }
>>>>>>> upstream/master
    return len;
}

int ZEXPORTVA gzprintf(gzFile file, const char *format, ...)
{
    va_list va;
    int ret;

    va_start(va, format);
    ret = gzvprintf(file, format, va);
    va_end(va);
    return ret;
}

#else /* !STDC && !Z_HAVE_STDARG_H */

/* -- see zlib.h -- */
int ZEXPORTVA gzprintf (file, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
                       a11, a12, a13, a14, a15, a16, a17, a18, a19, a20)
    gzFile file;
    const char *format;
    int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
        a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
{
<<<<<<< HEAD
    int size, len;
=======
    unsigned len, left;
    char *next;
>>>>>>> upstream/master
    gz_statep state;
    z_streamp strm;

    /* get internal structure */
    if (file == NULL)
<<<<<<< HEAD
        return -1;
=======
        return Z_STREAM_ERROR;
>>>>>>> upstream/master
    state = (gz_statep)file;
    strm = &(state->strm);

    /* check that can really pass pointer in ints */
    if (sizeof(int) != sizeof(void *))
<<<<<<< HEAD
        return 0;

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return 0;

    /* make sure we have some buffer space */
    if (state->size == 0 && gz_init(state) == -1)
        return 0;
=======
        return Z_STREAM_ERROR;

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return Z_STREAM_ERROR;

    /* make sure we have some buffer space */
    if (state->size == 0 && gz_init(state) == -1)
        return state->error;
>>>>>>> upstream/master

    /* check for seek request */
    if (state->seek) {
        state->seek = 0;
        if (gz_zero(state, state->skip) == -1)
<<<<<<< HEAD
            return 0;
    }

    /* consume whatever's left in the input buffer */
    if (strm->avail_in && gz_comp(state, Z_NO_FLUSH) == -1)
        return 0;

    /* do the printf() into the input buffer, put length in len */
    size = (int)(state->size);
    state->in[size - 1] = 0;
#ifdef NO_snprintf
#  ifdef HAS_sprintf_void
    sprintf((char *)(state->in), format, a1, a2, a3, a4, a5, a6, a7, a8,
            a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
    for (len = 0; len < size; len++)
        if (state->in[len] == 0) break;
#  else
    len = sprintf((char *)(state->in), format, a1, a2, a3, a4, a5, a6, a7, a8,
                  a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
#  endif
#else
#  ifdef HAS_snprintf_void
    snprintf((char *)(state->in), size, format, a1, a2, a3, a4, a5, a6, a7, a8,
             a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
    len = strlen((char *)(state->in));
#  else
    len = snprintf((char *)(state->in), size, format, a1, a2, a3, a4, a5, a6,
                   a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18,
                   a19, a20);
=======
            return state->error;
    }

    /* do the printf() into the input buffer, put length in len -- the input
       buffer is double-sized just for this function, so there is guaranteed to
       be state->size bytes available after the current contents */
    if (strm->avail_in == 0)
        strm->next_in = state->in;
    next = (char *)(strm->next_in + strm->avail_in);
    next[state->size - 1] = 0;
#ifdef NO_snprintf
#  ifdef HAS_sprintf_void
    sprintf(next, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12,
            a13, a14, a15, a16, a17, a18, a19, a20);
    for (len = 0; len < size; len++)
        if (next[len] == 0)
            break;
#  else
    len = sprintf(next, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11,
                  a12, a13, a14, a15, a16, a17, a18, a19, a20);
#  endif
#else
#  ifdef HAS_snprintf_void
    snprintf(next, state->size, format, a1, a2, a3, a4, a5, a6, a7, a8, a9,
             a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
    len = strlen(next);
#  else
    len = snprintf(next, state->size, format, a1, a2, a3, a4, a5, a6, a7, a8,
                   a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
>>>>>>> upstream/master
#  endif
#endif

    /* check that printf() results fit in buffer */
<<<<<<< HEAD
    if (len <= 0 || len >= (int)size || state->in[size - 1] != 0)
        return 0;

    /* update buffer and position, defer compression until needed */
    strm->avail_in = (unsigned)len;
    strm->next_in = state->in;
    state->x.pos += len;
    return len;
=======
    if (len == 0 || len >= state->size || next[state->size - 1] != 0)
        return 0;

    /* update buffer and position, compress first half if past that */
    strm->avail_in += len;
    state->x.pos += len;
    if (strm->avail_in >= state->size) {
        left = strm->avail_in - state->size;
        strm->avail_in = state->size;
        if (gz_comp(state, Z_NO_FLUSH) == -1)
            return state->err;
        memcpy(state->in, state->in + state->size, left);
        strm->next_in = state->in;
        strm->avail_in = left;
    }
    return (int)len;
>>>>>>> upstream/master
}

#endif

/* -- see zlib.h -- */
int ZEXPORT gzflush(file, flush)
    gzFile file;
    int flush;
{
    gz_statep state;

    /* get internal structure */
    if (file == NULL)
<<<<<<< HEAD
        return -1;
=======
        return Z_STREAM_ERROR;
>>>>>>> upstream/master
    state = (gz_statep)file;

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return Z_STREAM_ERROR;

    /* check flush parameter */
    if (flush < 0 || flush > Z_FINISH)
        return Z_STREAM_ERROR;

    /* check for seek request */
    if (state->seek) {
        state->seek = 0;
        if (gz_zero(state, state->skip) == -1)
<<<<<<< HEAD
            return -1;
    }

    /* compress remaining data with requested flush */
    gz_comp(state, flush);
=======
            return state->err;
    }

    /* compress remaining data with requested flush */
    (void)gz_comp(state, flush);
>>>>>>> upstream/master
    return state->err;
}

/* -- see zlib.h -- */
int ZEXPORT gzsetparams(file, level, strategy)
    gzFile file;
    int level;
    int strategy;
{
    gz_statep state;
    z_streamp strm;

    /* get internal structure */
    if (file == NULL)
        return Z_STREAM_ERROR;
    state = (gz_statep)file;
    strm = &(state->strm);

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return Z_STREAM_ERROR;

    /* if no change is requested, then do nothing */
    if (level == state->level && strategy == state->strategy)
        return Z_OK;

    /* check for seek request */
    if (state->seek) {
        state->seek = 0;
        if (gz_zero(state, state->skip) == -1)
<<<<<<< HEAD
            return -1;
=======
            return state->err;
>>>>>>> upstream/master
    }

    /* change compression parameters for subsequent input */
    if (state->size) {
        /* flush previous input with previous parameters before changing */
<<<<<<< HEAD
        if (strm->avail_in && gz_comp(state, Z_PARTIAL_FLUSH) == -1)
=======
        if (strm->avail_in && gz_comp(state, Z_BLOCK) == -1)
>>>>>>> upstream/master
            return state->err;
        deflateParams(strm, level, strategy);
    }
    state->level = level;
    state->strategy = strategy;
    return Z_OK;
}

/* -- see zlib.h -- */
int ZEXPORT gzclose_w(file)
    gzFile file;
{
    int ret = Z_OK;
    gz_statep state;

    /* get internal structure */
    if (file == NULL)
        return Z_STREAM_ERROR;
    state = (gz_statep)file;

    /* check that we're writing */
    if (state->mode != GZ_WRITE)
        return Z_STREAM_ERROR;

    /* check for seek request */
    if (state->seek) {
        state->seek = 0;
        if (gz_zero(state, state->skip) == -1)
            ret = state->err;
    }

    /* flush, free memory, and close file */
    if (gz_comp(state, Z_FINISH) == -1)
        ret = state->err;
    if (state->size) {
        if (!state->direct) {
            (void)deflateEnd(&(state->strm));
            free(state->out);
        }
        free(state->in);
    }
    gz_error(state, Z_OK, NULL);
    free(state->path);
    if (close(state->fd) == -1)
        ret = Z_ERRNO;
    free(state);
    return ret;
}
