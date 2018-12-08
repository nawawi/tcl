/*
 * tclUnixThrd.c --
 *
 *	This file implements the UNIX-specific thread support.
 *
 * Copyright (c) 1991-1994 The Regents of the University of California.
 * Copyright (c) 1994-1997 Sun Microsystems, Inc.
 * Copyright (c) 2008 by George Peter Staplin
 *
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

#include "tclInt.h"

#if TCL_THREADS
<<<<<<< HEAD

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
typedef struct ThreadSpecificData {
=======
typedef struct {
>>>>>>> upstream/master
=======
typedef struct {
>>>>>>> upstream/master
=======
typedef struct {
>>>>>>> upstream/master
    char nabuf[16];
} ThreadSpecificData;

static Tcl_ThreadDataKey dataKey;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD

/*
 * This is the number of milliseconds to wait between internal retries in
 * the Tcl_MutexLock function.  This value must be greater than zero and
 * should be a suitable value for the given platform.
 *
 * TODO: This may need to be dynamically determined, based on the relative
 *       performance of the running process.
 */

#ifndef TCL_MUTEX_LOCK_SLEEP_TIME
#  define TCL_MUTEX_LOCK_SLEEP_TIME	(25)
#endif
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master

=======
>>>>>>> upstream/master
/*
=======

/*
>>>>>>> upstream/master
=======

/*
 * TIP #509. Ensures that Tcl's mutexes are reentrant.
 *
 *----------------------------------------------------------------------
 *
 * PMutexInit --
 *
 *	Sets up the memory pointed to by its argument so that it contains the
 *	implementation of a recursive lock. Caller supplies the space.
 *
 *----------------------------------------------------------------------
 *
 * PMutexDestroy --
 *
 *	Tears down the implementation of a recursive lock (but does not
 *	deallocate the space holding the lock).
 *
 *----------------------------------------------------------------------
 *
 * PMutexLock --
 *
 *	Locks a recursive lock. (Similar to pthread_mutex_lock)
 *
 *----------------------------------------------------------------------
 *
 * PMutexUnlock --
 *
 *	Unlocks a recursive lock. (Similar to pthread_mutex_unlock)
 *
 *----------------------------------------------------------------------
 *
 * PCondWait --
 *
 *	Waits on a condition variable linked a recursive lock. (Similar to
 *	pthread_cond_wait)
 *
 *----------------------------------------------------------------------
 *
 * PCondTimedWait --
 *
 *	Waits for a limited amount of time on a condition variable linked to a
 *	recursive lock. (Similar to pthread_cond_timedwait)
 *
 *----------------------------------------------------------------------
 */

#ifndef HAVE_DECL_PTHREAD_MUTEX_RECURSIVE
#define HAVE_DECL_PTHREAD_MUTEX_RECURSIVE 0
#endif

#if HAVE_DECL_PTHREAD_MUTEX_RECURSIVE
/*
 * Pthread has native reentrant (AKA recursive) mutexes. Use them for
 * Tcl_Mutex.
 */

typedef pthread_mutex_t PMutex;

static void
PMutexInit(
    PMutex *pmutexPtr)
{
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(pmutexPtr, &attr);
}

#define PMutexDestroy	pthread_mutex_destroy
#define PMutexLock	pthread_mutex_lock
#define PMutexUnlock	pthread_mutex_unlock
#define PCondWait	pthread_cond_wait
#define PCondTimedWait	pthread_cond_timedwait

#else /* !HAVE_PTHREAD_MUTEX_RECURSIVE */

/*
 * No native support for reentrant mutexes. Emulate them with regular mutexes
 * and thread-local counters.
 */

typedef struct PMutex {
    pthread_mutex_t mutex;
    pthread_t thread;
    int counter;
} PMutex;

static void
PMutexInit(
    PMutex *pmutexPtr)
{
    pthread_mutex_init(&pmutexPtr->mutex, NULL);
    pmutexPtr->thread = 0;
    pmutexPtr->counter = 0;
}

static void
PMutexDestroy(
    PMutex *pmutexPtr)
{
    pthread_mutex_destroy(&pmutexPtr->mutex);
}

static void
PMutexLock(
    PMutex *pmutexPtr)
{
    if (pmutexPtr->thread != pthread_self() || pmutexPtr->counter == 0) {
	pthread_mutex_lock(&pmutexPtr->mutex);
	pmutexPtr->thread = pthread_self();
	pmutexPtr->counter = 0;
    }
    pmutexPtr->counter++;
}

static void
PMutexUnlock(
    PMutex *pmutexPtr)
{
    pmutexPtr->counter--;
    if (pmutexPtr->counter == 0) {
	pmutexPtr->thread = 0;
	pthread_mutex_unlock(&pmutexPtr->mutex);
    }
}

static void
PCondWait(
    pthread_cond_t *pcondPtr,
    PMutex *pmutexPtr)
{
    pthread_cond_wait(pcondPtr, &pmutexPtr->mutex);
}

static void
PCondTimedWait(
    pthread_cond_t *pcondPtr,
    PMutex *pmutexPtr,
    struct timespec *ptime)
{
    pthread_cond_timedwait(pcondPtr, &pmutexPtr->mutex, ptime);
}
#endif /* HAVE_PTHREAD_MUTEX_RECURSIVE */

/*
>>>>>>> upstream/master
 * masterLock is used to serialize creation of mutexes, condition variables,
 * and thread local storage. This is the only place that can count on the
 * ability to statically initialize the mutex.
 */

static pthread_mutex_t masterLock = PTHREAD_MUTEX_INITIALIZER;

/*
 * initLock is used to serialize initialization and finalization of Tcl. It
 * cannot use any dyamically allocated storage.
 */

static pthread_mutex_t initLock = PTHREAD_MUTEX_INITIALIZER;

/*
 * allocLock is used by Tcl's version of malloc for synchronization. For
 * obvious reasons, cannot use any dyamically allocated storage.
 */

static PMutex allocLock;
static pthread_once_t allocLockInitOnce = PTHREAD_ONCE_INIT;

static void
allocLockInit(void)
{
    PMutexInit(&allocLock);
}
static PMutex *allocLockPtr = &allocLock;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
/*
 * The mutexLock serializes Tcl_MutexLock. This is necessary to prevent
 * races when finalizing a mutex that some other thread may want to lock.
 */

static pthread_mutex_t mutexLock = PTHREAD_MUTEX_INITIALIZER;

/*
 * These are for the critical sections inside this file.
 */

#define MASTER_LOCK	pthread_mutex_lock(&masterLock)
#define MASTER_UNLOCK	pthread_mutex_unlock(&masterLock)

=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
#endif /* TCL_THREADS */

/*
 *----------------------------------------------------------------------
 *
 * TclpThreadCreate --
 *
 *	This procedure creates a new thread.
 *
 * Results:
 *	TCL_OK if the thread could be created. The thread ID is returned in a
 *	parameter.
 *
 * Side effects:
 *	A new thread is created.
 *
 *----------------------------------------------------------------------
 */

int
TclpThreadCreate(
    Tcl_ThreadId *idPtr,	/* Return, the ID of the thread */
    Tcl_ThreadCreateProc *proc,	/* Main() function of the thread */
    ClientData clientData,	/* The one argument to Main() */
    size_t stackSize,		/* Size of stack for the new thread */
    int flags)			/* Flags controlling behaviour of the new
				 * thread. */
{
#if TCL_THREADS
    pthread_attr_t attr;
    pthread_t theThread;
    int result;

    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

#ifdef HAVE_PTHREAD_ATTR_SETSTACKSIZE
    if (stackSize != TCL_THREAD_STACK_DEFAULT) {
	pthread_attr_setstacksize(&attr, stackSize);
#ifdef TCL_THREAD_STACK_MIN
    } else {
	/*
	 * Certain systems define a thread stack size that by default is too
	 * small for many operations. The user has the option of defining
	 * TCL_THREAD_STACK_MIN to a value large enough to work for their
	 * needs. This would look like (for 128K min stack):
	 *    make MEM_DEBUG_FLAGS=-DTCL_THREAD_STACK_MIN=131072L
	 *
	 * This solution is not optimal, as we should allow the user to
	 * specify a size at runtime, but we don't want to slow this function
	 * down, and that would still leave the main thread at the default.
	 */

	size_t size;

	result = pthread_attr_getstacksize(&attr, &size);
	if (!result && (size < TCL_THREAD_STACK_MIN)) {
	    pthread_attr_setstacksize(&attr, (size_t) TCL_THREAD_STACK_MIN);
	}
#endif /* TCL_THREAD_STACK_MIN */
    }
#endif /* HAVE_PTHREAD_ATTR_SETSTACKSIZE */

    if (!(flags & TCL_THREAD_JOINABLE)) {
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    }

    if (pthread_create(&theThread, &attr,
	    (void * (*)(void *)) proc, (void *) clientData) &&
	    pthread_create(&theThread, NULL,
		    (void * (*)(void *)) proc, (void *) clientData)) {
	result = TCL_ERROR;
    } else {
	*idPtr = (Tcl_ThreadId) theThread;
	result = TCL_OK;
    }
    pthread_attr_destroy(&attr);
    return result;
#else
    return TCL_ERROR;
#endif /* TCL_THREADS */
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_JoinThread --
 *
 *	This procedure waits upon the exit of the specified thread.
 *
 * Results:
 *	TCL_OK if the wait was successful, TCL_ERROR else.
 *
 * Side effects:
 *	The result area is set to the exit code of the thread we waited upon.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_JoinThread(
    Tcl_ThreadId threadId,	/* Id of the thread to wait upon. */
    int *state)			/* Reference to the storage the result of the
				 * thread we wait upon will be written into.
				 * May be NULL. */
{
#if TCL_THREADS
    int result;
    unsigned long retcode, *retcodePtr = &retcode;

    result = pthread_join((pthread_t) threadId, (void**) retcodePtr);
    if (state) {
	*state = (int) retcode;
    }
    return (result == 0) ? TCL_OK : TCL_ERROR;
#else
    return TCL_ERROR;
#endif
}

/*
 *----------------------------------------------------------------------
 *
 * TclpThreadExit --
 *
 *	This procedure terminates the current thread.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	This procedure terminates the current thread.
 *
 *----------------------------------------------------------------------
 */

void
TclpThreadExit(
    int status)
{
#if TCL_THREADS
    pthread_exit(INT2PTR(status));
#else /* TCL_THREADS */
    exit(status);
#endif /* TCL_THREADS */
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GetCurrentThread --
 *
 *	This procedure returns the ID of the currently running thread.
 *
 * Results:
 *	A thread ID.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

Tcl_ThreadId
Tcl_GetCurrentThread(void)
{
#if TCL_THREADS
    return (Tcl_ThreadId) pthread_self();
#else
    return (Tcl_ThreadId) 0;
#endif
}

/*
 *----------------------------------------------------------------------
 *
 * TclpInitLock
 *
 *	This procedure is used to grab a lock that serializes initialization
 *	and finalization of Tcl. On some platforms this may also initialize
 *	the mutex used to serialize creation of more mutexes and thread local
 *	storage keys.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Acquire the initialization mutex.
 *
 *----------------------------------------------------------------------
 */

void
TclpInitLock(void)
{
#if TCL_THREADS
    pthread_mutex_lock(&initLock);
#endif
}

/*
 *----------------------------------------------------------------------
 *
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
 * TclpFinalizeLock
=======
 * TclFinalizeLock
>>>>>>> upstream/master
=======
 * TclFinalizeLock
>>>>>>> upstream/master
=======
 * TclFinalizeLock
>>>>>>> upstream/master
=======
 * TclFinalizeLock
>>>>>>> upstream/master
 *
 *	This procedure is used to destroy all private resources used in this
 *	file.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Destroys everything private. TclpInitLock must be held entering this
 *	function.
 *
 *----------------------------------------------------------------------
 */
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> upstream/master

void
TclFinalizeLock(void)
{
#if TCL_THREADS
    /*
     * You do not need to destroy mutexes that were created with the
     * PTHREAD_MUTEX_INITIALIZER macro. These mutexes do not need any
     * destruction: masterLock, allocLock, and initLock.
     */

<<<<<<< HEAD
=======
=======
>>>>>>> upstream/master

=======

>>>>>>> upstream/master
void
TclFinalizeLock(void)
{
#if TCL_THREADS
    /*
     * You do not need to destroy mutexes that were created with the
     * PTHREAD_MUTEX_INITIALIZER macro. These mutexes do not need any
     * destruction: masterLock, allocLock, and initLock.
     */

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    pthread_mutex_unlock(&initLock);
#endif
}

/*
 *----------------------------------------------------------------------
 *
 * TclpInitUnlock
 *
 *	This procedure is used to release a lock that serializes
 *	initialization and finalization of Tcl.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Release the initialization mutex.
 *
 *----------------------------------------------------------------------
 */

void
TclpInitUnlock(void)
{
#if TCL_THREADS
    pthread_mutex_unlock(&initLock);
#endif
}

/*
 *----------------------------------------------------------------------
 *
 * TclpMasterLock
 *
 *	This procedure is used to grab a lock that serializes creation and
 *	finalization of serialization objects. This interface is only needed
 *	in finalization; it is hidden during creation of the objects.
 *
 *	This lock must be different than the initLock because the initLock is
 *	held during creation of synchronization objects.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Acquire the master mutex.
 *
 *----------------------------------------------------------------------
 */

void
TclpMasterLock(void)
{
#if TCL_THREADS
    pthread_mutex_lock(&masterLock);
#endif
}

/*
 *----------------------------------------------------------------------
 *
 * TclpMasterUnlock
 *
 *	This procedure is used to release a lock that serializes creation and
 *	finalization of synchronization objects.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Release the master mutex.
 *
 *----------------------------------------------------------------------
 */

void
TclpMasterUnlock(void)
{
#if TCL_THREADS
    pthread_mutex_unlock(&masterLock);
#endif
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GetAllocMutex
 *
 *	This procedure returns a pointer to a statically initialized mutex for
 *	use by the memory allocator. The alloctor must use this lock, because
 *	all other locks are allocated...
 *
 * Results:
 *	A pointer to a mutex that is suitable for passing to Tcl_MutexLock and
 *	Tcl_MutexUnlock.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

Tcl_Mutex *
Tcl_GetAllocMutex(void)
{
#if TCL_THREADS
<<<<<<< HEAD
    pthread_mutex_t **allocLockPtrPtr = &allocLockPtr;
=======
    PMutex **allocLockPtrPtr = &allocLockPtr;

    pthread_once(&allocLockInitOnce, allocLockInit);
>>>>>>> upstream/master
    return (Tcl_Mutex *) allocLockPtrPtr;
#else
    return NULL;
#endif
}

#if TCL_THREADS

/*
 *----------------------------------------------------------------------
 *
 * Tcl_MutexLock --
 *
 *	This procedure is invoked to lock a mutex. This procedure handles
 *	initializing the mutex, if necessary. The caller can rely on the fact
 *	that Tcl_Mutex is an opaque pointer. This routine will change that
 *	pointer from NULL after first use.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	May block the current thread. The mutex is acquired when this returns.
 *	Will allocate memory for a pthread_mutex_t and initialize this the
 *	first time this Tcl_Mutex is used.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_MutexLock(
    Tcl_Mutex *mutexPtr)	/* Really (PMutex **) */
{
    PMutex *pmutexPtr;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
retry:

=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    if (*mutexPtr == NULL) {
	pthread_mutex_lock(&masterLock);
	if (*mutexPtr == NULL) {
	    /*
	     * Double inside master lock check to avoid a race condition.
	     */

	    pmutexPtr = Tcl_Alloc(sizeof(PMutex));
	    PMutexInit(pmutexPtr);
	    *mutexPtr = (Tcl_Mutex) pmutexPtr;
	    TclRememberMutex(mutexPtr);
	}
	pthread_mutex_unlock(&masterLock);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    }
    while (1) {
	pthread_mutex_lock(&mutexLock);
	pmutexPtr = *((pthread_mutex_t **)mutexPtr);
	if (pmutexPtr == NULL) {
	    pthread_mutex_unlock(&mutexLock);
	    goto retry;
	}
	if (pthread_mutex_trylock(pmutexPtr) == 0) {
	    pthread_mutex_unlock(&mutexLock);
	    return;
	}
	pthread_mutex_unlock(&mutexLock);
	/*
	 * BUGBUG: All core and Thread package tests pass when usleep()
	 *         is used; however, the Thread package tests hang at
	 *         various places when Tcl_Sleep() is used, typically
	 *         while running test "thread-17.8", "thread-17.9", or
	 *         "thread-17.11a".  Really, what we want here is just
	 *         to yield to other threads for a while.
	 */
#ifdef HAVE_USLEEP
	usleep(TCL_MUTEX_LOCK_SLEEP_TIME * 1000);
#else
	Tcl_Sleep(TCL_MUTEX_LOCK_SLEEP_TIME);
#endif
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    }
<<<<<<< HEAD
=======
    pmutexPtr = *((PMutex **) mutexPtr);
    PMutexLock(pmutexPtr);
>>>>>>> upstream/master
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_MutexUnlock --
 *
 *	This procedure is invoked to unlock a mutex. The mutex must have been
 *	locked by Tcl_MutexLock.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The mutex is released when this returns.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_MutexUnlock(
    Tcl_Mutex *mutexPtr)	/* Really (PMutex **) */
{
    PMutex *pmutexPtr = *(PMutex **) mutexPtr;

    PMutexUnlock(pmutexPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * TclpFinalizeMutex --
 *
 *	This procedure is invoked to clean up one mutex. This is only safe to
 *	call at the end of time.
 *
 *	This assumes the Master Lock is held.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The mutex list is deallocated.
 *
 *----------------------------------------------------------------------
 */

void
TclpFinalizeMutex(
    Tcl_Mutex *mutexPtr)
{
    PMutex *pmutexPtr = *(PMutex **) mutexPtr;

    if (pmutexPtr != NULL) {
	PMutexDestroy(pmutexPtr);
	Tcl_Free(pmutexPtr);
	*mutexPtr = NULL;
    }
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> upstream/master

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ConditionWait --
<<<<<<< HEAD
 *
 *	This procedure is invoked to wait on a condition variable. The mutex
 *	is automically released as part of the wait, and automatically grabbed
 *	when the condition is signaled.
 *
=======
 *
 *	This procedure is invoked to wait on a condition variable. The mutex
 *	is automically released as part of the wait, and automatically grabbed
 *	when the condition is signaled.
 *
>>>>>>> upstream/master
 *	The mutex must be held when this procedure is called.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	May block the current thread. The mutex is acquired when this returns.
 *	Will allocate memory for a pthread_mutex_t and initialize this the
 *	first time this Tcl_Mutex is used.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_ConditionWait(
    Tcl_Condition *condPtr,	/* Really (pthread_cond_t **) */
    Tcl_Mutex *mutexPtr,	/* Really (PMutex **) */
    const Tcl_Time *timePtr) /* Timeout on waiting period */
{
    pthread_cond_t *pcondPtr;
    PMutex *pmutexPtr;
    struct timespec ptime;

    if (*condPtr == NULL) {
	pthread_mutex_lock(&masterLock);

	/*
	 * Double check inside mutex to avoid race, then initialize condition
	 * variable if necessary.
	 */

	if (*condPtr == NULL) {
	    pcondPtr = Tcl_Alloc(sizeof(pthread_cond_t));
	    pthread_cond_init(pcondPtr, NULL);
	    *condPtr = (Tcl_Condition) pcondPtr;
	    TclRememberCondition(condPtr);
	}
	pthread_mutex_unlock(&masterLock);
    }
    pmutexPtr = *((PMutex **) mutexPtr);
    pcondPtr = *((pthread_cond_t **) condPtr);
    if (timePtr == NULL) {
	PCondWait(pcondPtr, pmutexPtr);
    } else {
	Tcl_Time now;

	/*
	 * Make sure to take into account the microsecond component of the
	 * current time, including possible overflow situations. [Bug #411603]
	 */

	Tcl_GetTime(&now);
	ptime.tv_sec = timePtr->sec + now.sec +
	    (timePtr->usec + now.usec) / 1000000;
	ptime.tv_nsec = 1000 * ((timePtr->usec + now.usec) % 1000000);
	PCondTimedWait(pcondPtr, pmutexPtr, &ptime);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ConditionNotify --
<<<<<<< HEAD
 *
 *	This procedure is invoked to signal a condition variable.
 *
=======
 *
 *	This procedure is invoked to signal a condition variable.
 *
>>>>>>> upstream/master
 *	The mutex must be held during this call to avoid races, but this
 *	interface does not enforce that.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	May unblock another thread.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_ConditionNotify(
    Tcl_Condition *condPtr)
{
    pthread_cond_t *pcondPtr = *((pthread_cond_t **) condPtr);

    if (pcondPtr != NULL) {
	pthread_cond_broadcast(pcondPtr);
    } else {
	/*
	 * No-one has used the condition variable, so there are no waiters.
	 */
    }
}

/*
 *----------------------------------------------------------------------
 *
 * TclpFinalizeCondition --
 *
 *	This procedure is invoked to clean up a condition variable. This is
 *	only safe to call at the end of time.
 *
 *	This assumes the Master Lock is held.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The condition variable is deallocated.
 *
 *----------------------------------------------------------------------
 */

void
TclpFinalizeCondition(
    Tcl_Condition *condPtr)
{
    pthread_cond_t *pcondPtr = *(pthread_cond_t **) condPtr;

    if (pcondPtr != NULL) {
	pthread_cond_destroy(pcondPtr);
	Tcl_Free(pcondPtr);
	*condPtr = NULL;
    }
}
<<<<<<< HEAD

<<<<<<< HEAD
<<<<<<< HEAD
/*
 *----------------------------------------------------------------------
 *
 * TclpReaddir, TclpInetNtoa --
 *
 *	These procedures replace core C versions to be used in a threaded
 *	environment.
 *
 * Results:
 *	See documentation of C functions.
 *
 * Side effects:
 *	See documentation of C functions.
 *
 * Notes:
 *	TclpReaddir is no longer used by the core (see 1095909), but it
 *	appears in the internal stubs table (see #589526).
 *
 *----------------------------------------------------------------------
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#ifndef TCL_NO_DEPRECATED
>>>>>>> upstream/master
=======
#ifndef TCL_NO_DEPRECATED
>>>>>>> upstream/master
Tcl_DirEntry *
TclpReaddir(
    DIR * dir)
{
    return TclOSreaddir(dir);
<<<<<<< HEAD
}
<<<<<<< HEAD

#undef TclpInetNtoa
char *
TclpInetNtoa(
    struct in_addr addr)
{
#ifdef TCL_THREADS
    ThreadSpecificData *tsdPtr = TCL_TSD_INIT(&dataKey);
    unsigned char *b = (unsigned char*) &addr.s_addr;

    sprintf(tsdPtr->nabuf, "%u.%u.%u.%u", b[0], b[1], b[2], b[3]);
    return tsdPtr->nabuf;
#else
    return inet_ntoa(addr);
#endif
=======
>>>>>>> upstream/master
}
<<<<<<< HEAD

#undef TclpInetNtoa
char *
TclpInetNtoa(
    struct in_addr addr)
{
#ifdef TCL_THREADS
    ThreadSpecificData *tsdPtr = TCL_TSD_INIT(&dataKey);
    unsigned char *b = (unsigned char*) &addr.s_addr;

    sprintf(tsdPtr->nabuf, "%u.%u.%u.%u", b[0], b[1], b[2], b[3]);
    return tsdPtr->nabuf;
#else
    return inet_ntoa(addr);
#endif
}
=======
>>>>>>> upstream/master
#endif /* TCL_NO_DEPRECATED */

=======
>>>>>>> upstream/master
#ifdef TCL_THREADS
=======
>>>>>>> upstream/master
=======

>>>>>>> upstream/master
/*
 * Additions by AOL for specialized thread memory allocator.
 */

#ifdef USE_THREAD_ALLOC
<<<<<<< HEAD
<<<<<<< HEAD
static volatile int initialized = 0;
static pthread_key_t key;

typedef struct allocMutex {
=======
static pthread_key_t key;

typedef struct {
>>>>>>> upstream/master
=======
static pthread_key_t key;

typedef struct {
>>>>>>> upstream/master
    Tcl_Mutex tlock;
    PMutex plock;
} AllocMutex;

Tcl_Mutex *
TclpNewAllocMutex(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    struct allocMutex *lockPtr;
=======
    allocMutex *lockPtr;
>>>>>>> upstream/master
    register pthread_mutex_t *plockPtr;

    lockPtr = malloc(sizeof(allocMutex));
    if (lockPtr == NULL) {
	Tcl_Panic("could not allocate lock");
    }
    plockPtr = &lockPtr->plock;
    lockPtr->tlock = (Tcl_Mutex) plockPtr;
    pthread_mutex_init(&lockPtr->plock, NULL);
    return &lockPtr->tlock;
}

void
<<<<<<< HEAD
TclpFreeAllocMutex(
    Tcl_Mutex *mutex)		/* The alloc mutex to free. */
{
    allocMutex* lockPtr = (allocMutex*) mutex;
    if (!lockPtr) {
	return;
    }
    pthread_mutex_destroy(&lockPtr->plock);
    free(lockPtr);
=======

#undef TclpInetNtoa
char *
TclpInetNtoa(
    struct in_addr addr)
{
#ifdef TCL_THREADS
    ThreadSpecificData *tsdPtr = TCL_TSD_INIT(&dataKey);
    unsigned char *b = (unsigned char*) &addr.s_addr;

    sprintf(tsdPtr->nabuf, "%u.%u.%u.%u", b[0], b[1], b[2], b[3]);
    return tsdPtr->nabuf;
#else
    return inet_ntoa(addr);
#endif
}
=======
>>>>>>> upstream/master

#ifdef TCL_THREADS
/*
<<<<<<< HEAD
 * Additions by AOL for specialized thread memory allocator.
=======
 *----------------------------------------------------------------------
 *
 * Tcl_ConditionWait --
 *
 *	This procedure is invoked to wait on a condition variable. The mutex
 *	is automically released as part of the wait, and automatically grabbed
 *	when the condition is signaled.
 *
 *	The mutex must be held when this procedure is called.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	May block the current thread. The mutex is aquired when this returns.
 *	Will allocate memory for a pthread_mutex_t and initialize this the
 *	first time this Tcl_Mutex is used.
 *
 *----------------------------------------------------------------------
>>>>>>> upstream/master
 */
=======
    allocMutex *lockPtr;
    register pthread_mutex_t *plockPtr;
=======
    AllocMutex *lockPtr;
    register PMutex *plockPtr;
>>>>>>> upstream/master

    lockPtr = malloc(sizeof(AllocMutex));
    if (lockPtr == NULL) {
	Tcl_Panic("could not allocate lock");
    }
    plockPtr = &lockPtr->plock;
    lockPtr->tlock = (Tcl_Mutex) plockPtr;
    PMutexInit(&lockPtr->plock);
    return &lockPtr->tlock;
}

void
TclpFreeAllocMutex(
    Tcl_Mutex *mutex)		/* The alloc mutex to free. */
{
    AllocMutex *lockPtr = (AllocMutex *) mutex;

    if (!lockPtr) {
	return;
    }
    PMutexDestroy(&lockPtr->plock);
    free(lockPtr);
}

void
TclpInitAllocCache(void)
{
    pthread_key_create(&key, NULL);
}
>>>>>>> upstream/master

#ifdef USE_THREAD_ALLOC
static pthread_key_t key;

typedef struct allocMutex {
    Tcl_Mutex tlock;
    pthread_mutex_t plock;
} allocMutex;

Tcl_Mutex *
TclpNewAllocMutex(void)
{
    struct allocMutex *lockPtr;
    register pthread_mutex_t *plockPtr;

    lockPtr = malloc(sizeof(struct allocMutex));
    if (lockPtr == NULL) {
	Tcl_Panic("could not allocate lock");
    }
    plockPtr = &lockPtr->plock;
    lockPtr->tlock = (Tcl_Mutex) plockPtr;
    pthread_mutex_init(&lockPtr->plock, NULL);
    return &lockPtr->tlock;
}

void
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> upstream/master
TclpFreeAllocMutex(
    Tcl_Mutex *mutex)		/* The alloc mutex to free. */
=======
Tcl_ConditionWait(
    Tcl_Condition *condPtr,	/* Really (pthread_cond_t **) */
    Tcl_Mutex *mutexPtr,	/* Really (pthread_mutex_t **) */
    const Tcl_Time *timePtr) /* Timeout on waiting period */
>>>>>>> upstream/master
{
    allocMutex* lockPtr = (allocMutex*) mutex;
    if (!lockPtr) {
	return;
    }
    pthread_mutex_destroy(&lockPtr->plock);
    free(lockPtr);
}

<<<<<<< HEAD
void
TclpInitAllocCache(void)
{
    pthread_mutex_lock(allocLockPtr);
    pthread_key_create(&key, TclpFreeAllocCache);
    pthread_mutex_unlock(allocLockPtr);
>>>>>>> upstream/master
}

void
TclpInitAllocCache(void)
{
    pthread_key_create(&key, NULL);
}

void
TclpFreeAllocCache(
    void *ptr)
{
    if (ptr != NULL) {
	/*
<<<<<<< HEAD
	 * Called by the pthread lib when a thread exits
=======
TclpFreeAllocCache(
    void *ptr)
{
    if (ptr != NULL) {
	/*
	 * Called by TclFinalizeThreadAllocThread() during the thread
	 * finalization initiated from Tcl_FinalizeThread()
>>>>>>> upstream/master
=======
	 * Called by TclFinalizeThreadAllocThread() during the thread
	 * finalization initiated from Tcl_FinalizeThread()
>>>>>>> upstream/master
	 */

	TclFreeAllocCache(ptr);
	pthread_setspecific(key, NULL);
<<<<<<< HEAD
<<<<<<< HEAD

<<<<<<< HEAD
    } else if (initialized) {
=======

    } else {
>>>>>>> upstream/master
	/*
	 * Called by TclFinalizeThreadAlloc() during the process
	 * finalization initiated from Tcl_Finalize()
	 */

	pthread_key_delete(key);
<<<<<<< HEAD
	initialized = 0;
=======
    } else {
	pthread_key_delete(key);
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    }
}

void *
TclpGetAllocCache(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
    if (!initialized) {
	pthread_mutex_lock(allocLockPtr);
	if (!initialized) {
	    pthread_key_create(&key, TclpFreeAllocCache);
	    initialized = 1;
	}
	pthread_mutex_unlock(allocLockPtr);
    }
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    return pthread_getspecific(key);
}

void
TclpSetAllocCache(
    void *arg)
<<<<<<< HEAD
=======
    if (*condPtr == NULL) {
	pthread_mutex_lock(&masterLock);

	/*
	 * Double check inside mutex to avoid race, then initialize condition
	 * variable if necessary.
	 */

	if (*condPtr == NULL) {
	    pcondPtr = ckalloc(sizeof(pthread_cond_t));
	    pthread_cond_init(pcondPtr, NULL);
	    *condPtr = (Tcl_Condition) pcondPtr;
	    TclRememberCondition(condPtr);
	}
	pthread_mutex_unlock(&masterLock);
=======
{
    pthread_setspecific(key, arg);
}
#endif /* USE_THREAD_ALLOC */

void *
TclpThreadCreateKey(void)
{
    pthread_key_t *ptkeyPtr;

    ptkeyPtr = TclpSysAlloc(sizeof(pthread_key_t));
    if (NULL == ptkeyPtr) {
	Tcl_Panic("unable to allocate thread key!");
>>>>>>> upstream/master
    }
    pmutexPtr = *((pthread_mutex_t **)mutexPtr);
    pcondPtr = *((pthread_cond_t **)condPtr);
    if (timePtr == NULL) {
	pthread_cond_wait(pcondPtr, pmutexPtr);
    } else {
	Tcl_Time now;

<<<<<<< HEAD
	/*
	 * Make sure to take into account the microsecond component of the
	 * current time, including possible overflow situations. [Bug #411603]
	 */

	Tcl_GetTime(&now);
	ptime.tv_sec = timePtr->sec + now.sec +
	    (timePtr->usec + now.usec) / 1000000;
	ptime.tv_nsec = 1000 * ((timePtr->usec + now.usec) % 1000000);
	pthread_cond_timedwait(pcondPtr, pmutexPtr, &ptime);
    }
=======
    if (pthread_key_create(ptkeyPtr, NULL)) {
	Tcl_Panic("unable to create pthread key!");
    }

    return ptkeyPtr;
>>>>>>> upstream/master
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ConditionNotify --
 *
 *	This procedure is invoked to signal a condition variable.
 *
 *	The mutex must be held during this call to avoid races, but this
 *	interface does not enforce that.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	May unblock another thread.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_ConditionNotify(
    Tcl_Condition *condPtr)
>>>>>>> upstream/master
=======

<<<<<<< HEAD
    } else {
	/*
	 * Called by TclFinalizeThreadAlloc() during the process
	 * finalization initiated from Tcl_Finalize()
	 */

	pthread_key_delete(key);
    }
}

void *
TclpGetAllocCache(void)
{
    return pthread_getspecific(key);
}

void
TclpSetAllocCache(
    void *arg)
>>>>>>> upstream/master
{
    pthread_setspecific(key, arg);
}
#endif /* USE_THREAD_ALLOC */

void *
TclpThreadCreateKey(void)
{
    pthread_key_t *ptkeyPtr;

    ptkeyPtr = TclpSysAlloc(sizeof *ptkeyPtr, 0);
    if (NULL == ptkeyPtr) {
	Tcl_Panic("unable to allocate thread key!");
<<<<<<< HEAD
    }

    if (pthread_key_create(ptkeyPtr, NULL)) {
	Tcl_Panic("unable to create pthread key!");
    }

    return ptkeyPtr;
}
<<<<<<< HEAD

void
TclpThreadDeleteKey(
    void *keyPtr)
{
    pthread_key_t *ptkeyPtr = keyPtr;

=======
void
TclpThreadDeleteKey(
    void *keyPtr)
{
    pthread_key_t *ptkeyPtr = keyPtr;

>>>>>>> upstream/master
    if (pthread_key_delete(*ptkeyPtr)) {
	Tcl_Panic("unable to delete key!");
    }

    TclpSysFree(keyPtr);
}

void
TclpThreadSetMasterTSD(
    void *tsdKeyPtr,
    void *ptr)
{
    pthread_key_t *ptkeyPtr = tsdKeyPtr;

    if (pthread_setspecific(*ptkeyPtr, ptr)) {
	Tcl_Panic("unable to set master TSD value");
=======

/*
 *----------------------------------------------------------------------
 *
 * TclpFinalizeCondition --
 *
 *	This procedure is invoked to clean up a condition variable. This is
 *	only safe to call at the end of time.
 *
 *	This assumes the Master Lock is held.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The condition variable is deallocated.
 *
 *----------------------------------------------------------------------
 */

void
TclpFinalizeCondition(
    Tcl_Condition *condPtr)
{
    pthread_cond_t *pcondPtr = *(pthread_cond_t **)condPtr;

    if (pcondPtr != NULL) {
	pthread_cond_destroy(pcondPtr);
	ckfree(pcondPtr);
	*condPtr = NULL;
>>>>>>> upstream/master
    }
}
#endif /* TCL_THREADS */

/*
 *----------------------------------------------------------------------
 *
 * TclpReaddir, TclpInetNtoa --
 *
 *	These procedures replace core C versions to be used in a threaded
 *	environment.
 *
 * Results:
 *	See documentation of C functions.
 *
 * Side effects:
 *	See documentation of C functions.
 *
 * Notes:
 *	TclpReaddir is no longer used by the core (see 1095909), but it
 *	appears in the internal stubs table (see #589526).
 *
 *----------------------------------------------------------------------
 */

Tcl_DirEntry *
TclpReaddir(
    DIR * dir)
{
    return TclOSreaddir(dir);
}

<<<<<<< HEAD
void *
TclpThreadGetMasterTSD(
    void *tsdKeyPtr)
{
    pthread_key_t *ptkeyPtr = tsdKeyPtr;

    return pthread_getspecific(*ptkeyPtr);
}

=======
#undef TclpInetNtoa
char *
TclpInetNtoa(
    struct in_addr addr)
{
#ifdef TCL_THREADS
    ThreadSpecificData *tsdPtr = TCL_TSD_INIT(&dataKey);
    unsigned char *b = (unsigned char*) &addr.s_addr;

<<<<<<< HEAD
    sprintf(tsdPtr->nabuf, "%u.%u.%u.%u", b[0], b[1], b[2], b[3]);
    return tsdPtr->nabuf;
#else
    return inet_ntoa(addr);
#endif
}

#ifdef TCL_THREADS
/*
 * Additions by AOL for specialized thread memory allocator.
 */

#ifdef USE_THREAD_ALLOC
static pthread_key_t key;

typedef struct {
    Tcl_Mutex tlock;
    pthread_mutex_t plock;
} allocMutex;

Tcl_Mutex *
TclpNewAllocMutex(void)
{
    allocMutex *lockPtr;
    register pthread_mutex_t *plockPtr;

    lockPtr = malloc(sizeof(allocMutex));
    if (lockPtr == NULL) {
	Tcl_Panic("could not allocate lock");
    }
    plockPtr = &lockPtr->plock;
    lockPtr->tlock = (Tcl_Mutex) plockPtr;
    pthread_mutex_init(&lockPtr->plock, NULL);
    return &lockPtr->tlock;
}

void
TclpFreeAllocMutex(
    Tcl_Mutex *mutex)		/* The alloc mutex to free. */
{
    allocMutex* lockPtr = (allocMutex*) mutex;
    if (!lockPtr) {
	return;
=======
    }

    if (pthread_key_create(ptkeyPtr, NULL)) {
	Tcl_Panic("unable to create pthread key!");
    }

    return ptkeyPtr;
}

void
TclpThreadDeleteKey(
    void *keyPtr)
{
    pthread_key_t *ptkeyPtr = keyPtr;

    if (pthread_key_delete(*ptkeyPtr)) {
	Tcl_Panic("unable to delete key!");
    }

    TclpSysFree(keyPtr);
}

void
TclpThreadSetMasterTSD(
    void *tsdKeyPtr,
    void *ptr)
{
    pthread_key_t *ptkeyPtr = tsdKeyPtr;

    if (pthread_setspecific(*ptkeyPtr, ptr)) {
	Tcl_Panic("unable to set master TSD value");
>>>>>>> upstream/master
    }
    pthread_mutex_destroy(&lockPtr->plock);
    free(lockPtr);
}

void
TclpInitAllocCache(void)
{
    pthread_mutex_lock(allocLockPtr);
    pthread_key_create(&key, NULL);
    pthread_mutex_unlock(allocLockPtr);
}

<<<<<<< HEAD
void
TclpFreeAllocCache(
    void *ptr)
{
    if (ptr != NULL) {
	/*
	 * Called by TclFinalizeThreadAllocThread() during the thread
	 * finalization initiated from Tcl_FinalizeThread()
	 */

	TclFreeAllocCache(ptr);
	pthread_setspecific(key, NULL);

    } else {
	/*
	 * Called by TclFinalizeThreadAlloc() during the process
	 * finalization initiated from Tcl_Finalize()
	 */

	pthread_key_delete(key);
    }
}

void *
TclpGetAllocCache(void)
{
    return pthread_getspecific(key);
}

void
TclpSetAllocCache(
    void *arg)
{
    pthread_setspecific(key, arg);
}
#endif /* USE_THREAD_ALLOC */

void *
TclpThreadCreateKey(void)
{
    pthread_key_t *ptkeyPtr;

    ptkeyPtr = TclpSysAlloc(sizeof *ptkeyPtr, 0);
    if (NULL == ptkeyPtr) {
	Tcl_Panic("unable to allocate thread key!");
    }

    if (pthread_key_create(ptkeyPtr, NULL)) {
	Tcl_Panic("unable to create pthread key!");
    }

    return ptkeyPtr;
}

void
TclpThreadDeleteKey(
    void *keyPtr)
{
    pthread_key_t *ptkeyPtr = keyPtr;

    if (pthread_key_delete(*ptkeyPtr)) {
	Tcl_Panic("unable to delete key!");
    }

    TclpSysFree(keyPtr);
}

void
TclpThreadSetMasterTSD(
    void *tsdKeyPtr,
    void *ptr)
{
    pthread_key_t *ptkeyPtr = tsdKeyPtr;

    if (pthread_setspecific(*ptkeyPtr, ptr)) {
	Tcl_Panic("unable to set master TSD value");
    }
}

void *
TclpThreadGetMasterTSD(
    void *tsdKeyPtr)
{
    pthread_key_t *ptkeyPtr = tsdKeyPtr;

    return pthread_getspecific(*ptkeyPtr);
}

>>>>>>> upstream/master
=======
=======
>>>>>>> upstream/master
void *
TclpThreadGetMasterTSD(
    void *tsdKeyPtr)
{
    pthread_key_t *ptkeyPtr = tsdKeyPtr;

    return pthread_getspecific(*ptkeyPtr);
}

>>>>>>> upstream/master
#endif /* TCL_THREADS */

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * fill-column: 78
 * End:
 */
