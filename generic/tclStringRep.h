/*
 * tclStringRep.h --
 *
 *	This file contains the definition of the Unicode string internal
 *	representation and macros to access it.
 *
 *	A Unicode string is an internationalized string. Conceptually, a
 *	Unicode string is an array of 16-bit quantities organized as a
 *	sequence of properly formed UTF-8 characters. There is a one-to-one
 *	map between Unicode and UTF characters. Because Unicode characters
 *	have a fixed width, operations such as indexing operate on Unicode
 *	data. The String object is optimized for the case where each UTF char
 *	in a string is only one byte. In this case, we store the value of
 *	numChars, but we don't store the Unicode data (unless Tcl_GetUnicode
 *	is explicitly called).
 *
 *	The String object type stores one or both formats. The default
 *	behavior is to store UTF. Once Unicode is calculated by a function, it
 *	is stored in the internal rep for future access (without an additional
 *	O(n) cost).
 *
 *	To allow many appends to be done to an object without constantly
 *	reallocating the space for the string or Unicode representation, we
 *	allocate double the space for the string or Unicode and use the
 *	internal representation to keep track of how much space is used vs.
 *	allocated.
 *
 * Copyright (c) 1995-1997 Sun Microsystems, Inc.
 * Copyright (c) 1999 by Scriptics Corporation.
 *
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */
<<<<<<< HEAD
=======

#ifndef _TCLSTRINGREP
#define _TCLSTRINGREP

>>>>>>> upstream/master

/*
 * The following structure is the internal rep for a String object. It keeps
 * track of how much memory has been used and how much has been allocated for
 * the Unicode and UTF string to enable growing and shrinking of the UTF and
 * Unicode reps of the String object with fewer mallocs. To optimize string
 * length and indexing operations, this structure also stores the number of
 * characters (same of UTF and Unicode!) once that value has been computed.
 *
 * Under normal configurations, what Tcl calls "Unicode" is actually UTF-16
 * restricted to the Basic Multilingual Plane (i.e. U+00000 to U+0FFFF). This
 * can be officially modified by altering the definition of Tcl_UniChar in
 * tcl.h, but do not do that unless you are sure what you're doing!
 */

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
typedef struct String {
=======
typedef struct {
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> upstream/master
=======
typedef struct {
>>>>>>> upstream/master
=======
typedef struct {
>>>>>>> upstream/master
    int numChars;		/* The number of chars in the string. -1 means
				 * this value has not been calculated. >= 0
=======
    size_t numChars;		/* The number of chars in the string. (size_t)-1 means
=======
    size_t numChars;		/* The number of chars in the string. -1 means
>>>>>>> upstream/master
				 * this value has not been calculated. Any other
>>>>>>> upstream/master
=======
typedef struct {
    size_t numChars;		/* The number of chars in the string. -1 means
				 * this value has not been calculated. Any other
>>>>>>> upstream/master
				 * means that there is a valid Unicode rep, or
				 * that the number of UTF bytes == the number
				 * of chars. */
    size_t allocated;		/* The amount of space actually allocated for
				 * the UTF string (minus 1 byte for the
				 * termination char). */
    size_t maxChars;		/* Max number of chars that can fit in the
				 * space allocated for the unicode array. */
    int hasUnicode;		/* Boolean determining whether the string has
				 * a Unicode representation. */
    Tcl_UniChar unicode[TCLFLEXARRAY];	/* The array of Unicode chars. The actual size
				 * of this field depends on the 'maxChars'
				 * field above. */
} String;

<<<<<<< HEAD
#define STRING_MAXCHARS \
    ((UINT_MAX - sizeof(String))/sizeof(Tcl_UniChar))
#define STRING_SIZE(numChars) \
    (sizeof(String) + ((numChars) * sizeof(Tcl_UniChar)))
#define stringCheckLimits(numChars) \
    do {								\
	if ((size_t)(numChars) > STRING_MAXCHARS) {		\
	    Tcl_Panic("max length for a Tcl unicode value (%" TCL_Z_MODIFIER "u chars) exceeded", \
		      STRING_MAXCHARS);					\
	}								\
    } while (0)
=======
#define STRING_SIZE(numChars) \
<<<<<<< HEAD
    (sizeof(String) + ((numChars) * sizeof(Tcl_UniChar)))
>>>>>>> upstream/master
=======
    (offsetof(String, unicode) + ((numChars + 1) * sizeof(Tcl_UniChar)))
>>>>>>> upstream/master
#define stringAttemptAlloc(numChars) \
    (String *) Tcl_AttemptAlloc(STRING_SIZE(numChars))
#define stringAlloc(numChars) \
    (String *) Tcl_Alloc(STRING_SIZE(numChars))
#define stringRealloc(ptr, numChars) \
    (String *) Tcl_Realloc((ptr), STRING_SIZE(numChars))
#define stringAttemptRealloc(ptr, numChars) \
    (String *) Tcl_AttemptRealloc((ptr), STRING_SIZE(numChars))
#define GET_STRING(objPtr) \
    ((String *) (objPtr)->internalRep.twoPtrValue.ptr1)
#define SET_STRING(objPtr, stringPtr) \
    ((objPtr)->internalRep.twoPtrValue.ptr2 = NULL),			\
    ((objPtr)->internalRep.twoPtrValue.ptr1 = (void *) (stringPtr))

<<<<<<< HEAD
=======
#endif /*  _TCLSTRINGREP */
>>>>>>> upstream/master
/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * fill-column: 78
 * End:
 */
