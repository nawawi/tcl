/*
 * tclStubLib.c --
 *
 *	Stub object that will be statically linked into extensions that want
 *	to access Tcl.
 *
 * Copyright (c) 1998-1999 by Scriptics Corporation.
 * Copyright (c) 1998 Paul Duffin.
 *
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

#include "tclInt.h"

MODULE_SCOPE const TclStubs *tclStubsPtr;
MODULE_SCOPE const TclPlatStubs *tclPlatStubsPtr;
MODULE_SCOPE const TclIntStubs *tclIntStubsPtr;
MODULE_SCOPE const TclIntPlatStubs *tclIntPlatStubsPtr;

const TclStubs *tclStubsPtr = NULL;
const TclPlatStubs *tclPlatStubsPtr = NULL;
const TclIntStubs *tclIntStubsPtr = NULL;
const TclIntPlatStubs *tclIntPlatStubsPtr = NULL;

/*
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
 * Use our own isDigit to avoid linking to libc on windows
 */

static int isDigit(const int c)
{
    return (c >= '0' && c <= '9');
}
=======
 * Use our own ISDIGIT to avoid linking to libc on windows
 */

#define ISDIGIT(c) (((unsigned)((c)-'0')) <= 9)
>>>>>>> upstream/master
=======
 * Use our own ISDIGIT to avoid linking to libc on windows
 */

#define ISDIGIT(c) (((unsigned)((c)-'0')) <= 9)
>>>>>>> upstream/master
=======
 * Use our own ISDIGIT to avoid linking to libc on windows
 */

#define ISDIGIT(c) (((unsigned)((c)-'0')) <= 9)
>>>>>>> upstream/master
=======
 * Use our own ISDIGIT to avoid linking to libc on windows
 */

#define ISDIGIT(c) (((unsigned)((c)-'0')) <= 9)
>>>>>>> upstream/master

/*
 *----------------------------------------------------------------------
 *
 * Tcl_InitStubs --
 *
 *	Tries to initialise the stub table pointers and ensures that the
 *	correct version of Tcl is loaded.
 *
 * Results:
 *	The actual version of Tcl that satisfies the request, or NULL to
 *	indicate that an error occurred.
 *
 * Side effects:
 *	Sets the stub table pointers.
 *
 *----------------------------------------------------------------------
 */
#undef Tcl_InitStubs
MODULE_SCOPE const char *
Tcl_InitStubs(
    Tcl_Interp *interp,
    const char *version,
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    int exact)
=======
    int exact,
    int magic)
>>>>>>> upstream/master
=======
    int exact,
    int magic)
>>>>>>> upstream/master
=======
    int exact,
    int magic)
>>>>>>> upstream/master
=======
    int exact,
    int magic)
>>>>>>> upstream/master
{
    Interp *iPtr = (Interp *) interp;
    const char *actualVersion = NULL;
    ClientData pkgData = NULL;
    const TclStubs *stubsPtr = iPtr->stubTable;

    /*
     * We can't optimize this check by caching tclStubsPtr because that
     * prevents apps from being able to load/unload Tcl dynamically multiple
     * times. [Bug 615304]
     */

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    if (!stubsPtr || (stubsPtr->magic != TCL_STUB_MAGIC)) {
	iPtr->result = "interpreter uses an incompatible stubs mechanism";
=======
    if (!stubsPtr || (stubsPtr->magic != (((exact&0xff00) >= 0x900) ? magic : TCL_STUB_MAGIC))) {
	iPtr->result = (char *)"interpreter uses an incompatible stubs mechanism";
>>>>>>> upstream/master
	iPtr->freeProc = TCL_STATIC;
=======
    if (!stubsPtr || (stubsPtr->magic != (((exact&0xff00) >= 0x900) ? magic : TCL_STUB_MAGIC))) {
	iPtr->result = (char *)"interpreter uses an incompatible stubs mechanism";
	iPtr->freeProc = 0;
>>>>>>> upstream/master
=======
    if (!stubsPtr || (stubsPtr->magic != (((exact&0xff00) >= 0x900) ? magic : TCL_STUB_MAGIC))) {
<<<<<<< HEAD
	iPtr->result = (char *)"interpreter uses an incompatible stubs mechanism";
	iPtr->freeProc = 0;
>>>>>>> upstream/master
=======
	iPtr->legacyResult = "interpreter uses an incompatible stubs mechanism";
	iPtr->legacyFreeProc = 0; /* TCL_STATIC */
>>>>>>> upstream/master
=======
    if (!stubsPtr || (stubsPtr->magic != (((exact&0xff00) >= 0x900) ? magic : TCL_STUB_MAGIC))) {
=======
    if (!stubsPtr || (stubsPtr->magic != (((exact&0xFF00) >= 0x900) ? magic : TCL_STUB_MAGIC))) {
>>>>>>> upstream/master
	iPtr->legacyResult = "interpreter uses an incompatible stubs mechanism";
	iPtr->legacyFreeProc = 0; /* TCL_STATIC */
>>>>>>> upstream/master
	return NULL;
    }

    actualVersion = stubsPtr->tcl_PkgRequireEx(interp, "Tcl", version, 0, &pkgData);
    if (actualVersion == NULL) {
	return NULL;
    }
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    if (exact) {
=======
    if (exact&1) {
>>>>>>> upstream/master
=======
    if (exact&1) {
>>>>>>> upstream/master
=======
    if (exact&1) {
>>>>>>> upstream/master
=======
    if (exact&1) {
>>>>>>> upstream/master
	const char *p = version;
	int count = 0;

	while (*p) {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	    count += !isDigit(*p++);
=======
	    count += !ISDIGIT(*p++);
>>>>>>> upstream/master
=======
	    count += !ISDIGIT(*p++);
>>>>>>> upstream/master
=======
	    count += !ISDIGIT(*p++);
>>>>>>> upstream/master
=======
	    count += !ISDIGIT(*p++);
>>>>>>> upstream/master
	}
	if (count == 1) {
	    const char *q = actualVersion;

	    p = version;
	    while (*p && (*p == *q)) {
		p++; q++;
	    }
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	    if (*p || isDigit(*q)) {
=======
	    if (*p || ISDIGIT(*q)) {
>>>>>>> upstream/master
=======
	    if (*p || ISDIGIT(*q)) {
>>>>>>> upstream/master
=======
	    if (*p || ISDIGIT(*q)) {
>>>>>>> upstream/master
=======
	    if (*p || ISDIGIT(*q)) {
>>>>>>> upstream/master
		/* Construct error message */
		stubsPtr->tcl_PkgRequireEx(interp, "Tcl", version, 1, NULL);
		return NULL;
	    }
	} else {
	    actualVersion = stubsPtr->tcl_PkgRequireEx(interp, "Tcl", version, 1, NULL);
	    if (actualVersion == NULL) {
		return NULL;
	    }
	}
    }
    if (((exact&0xFF00) < 0x900)) {
	/* We are running Tcl 8.x */
	stubsPtr = (TclStubs *)pkgData;
    }
    tclStubsPtr = stubsPtr;

    if (stubsPtr->hooks) {
	tclPlatStubsPtr = stubsPtr->hooks->tclPlatStubs;
	tclIntStubsPtr = stubsPtr->hooks->tclIntStubs;
	tclIntPlatStubsPtr = stubsPtr->hooks->tclIntPlatStubs;
    } else {
	tclPlatStubsPtr = NULL;
	tclIntStubsPtr = NULL;
	tclIntPlatStubsPtr = NULL;
    }

    return actualVersion;
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * fill-column: 78
 * End:
 */
