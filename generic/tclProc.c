/*
 * tclProc.c --
 *
 *	This file contains routines that implement Tcl procedures, including
 *	the "proc" and "uplevel" commands.
 *
 * Copyright (c) 1987-1993 The Regents of the University of California.
 * Copyright (c) 1994-1998 Sun Microsystems, Inc.
 * Copyright (c) 2004-2006 Miguel Sofer
 * Copyright (c) 2007 Daniel A. Steffen <das@users.sourceforge.net>
 *
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

#include "tclInt.h"
#include "tclCompile.h"
#include <assert.h>

/*
 * Variables that are part of the [apply] command implementation and which
 * have to be passed to the other side of the NRE call.
 */

typedef struct {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    int isRootEnsemble;
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    Command cmd;
    ExtraFrameInfo efi;
} ApplyExtraData;

/*
 * Prototypes for static functions in this file
 */

static void		DupLambdaInternalRep(Tcl_Obj *objPtr,
			    Tcl_Obj *copyPtr);
static void		FreeLambdaInternalRep(Tcl_Obj *objPtr);
static int		InitArgsAndLocals(Tcl_Interp *interp, int skip);
static void		InitResolvedLocals(Tcl_Interp *interp,
			    ByteCode *codePtr, Var *defPtr,
			    Namespace *nsPtr);
static void		InitLocalCache(Proc *procPtr);
static void		ProcBodyDup(Tcl_Obj *srcPtr, Tcl_Obj *dupPtr);
static void		ProcBodyFree(Tcl_Obj *objPtr);
static int		ProcWrongNumArgs(Tcl_Interp *interp, int skip);
static void		MakeProcError(Tcl_Interp *interp,
			    Tcl_Obj *procNameObj);
static void		MakeLambdaError(Tcl_Interp *interp,
			    Tcl_Obj *procNameObj);
static int		SetLambdaFromAny(Tcl_Interp *interp, Tcl_Obj *objPtr);

static Tcl_NRPostProc ApplyNR2;
static Tcl_NRPostProc InterpProcNR2;
static Tcl_NRPostProc Uplevel_Callback;

/*
 * The ProcBodyObjType type
 */

const Tcl_ObjType tclProcBodyType = {
    "procbody",			/* name for this type */
    ProcBodyFree,		/* FreeInternalRep function */
    ProcBodyDup,		/* DupInternalRep function */
    NULL,			/* UpdateString function; Tcl_GetString and
				 * Tcl_GetStringFromObj should panic
				 * instead. */
    NULL			/* SetFromAny function; Tcl_ConvertToType
				 * should panic instead. */
};

#define ProcSetIntRep(objPtr, procPtr)					\
    do {								\
	Tcl_ObjIntRep ir;						\
	(procPtr)->refCount++;						\
	ir.twoPtrValue.ptr1 = (procPtr);				\
	ir.twoPtrValue.ptr2 = NULL;					\
	Tcl_StoreIntRep((objPtr), &tclProcBodyType, &ir);		\
    } while (0)

#define ProcGetIntRep(objPtr, procPtr)					\
    do {								\
	const Tcl_ObjIntRep *irPtr;					\
<<<<<<< HEAD
	irPtr = Tcl_FetchIntRep((objPtr), &tclProcBodyType);		\
=======
	irPtr = TclFetchIntRep((objPtr), &tclProcBodyType);		\
<<<<<<< HEAD
>>>>>>> upstream/master
	(procPtr) = irPtr ? irPtr->twoPtrValue.ptr1 : NULL;		\
=======
	(procPtr) = irPtr ? (Proc *)irPtr->twoPtrValue.ptr1 : NULL;		\
>>>>>>> upstream/master
    } while (0)

/*
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
 * The [upvar]/[uplevel] level reference type. Uses the twoPtrValue field,
 * encoding the type of level reference in ptr and the actual parsed out
 * offset in ptr2.
=======
 * The [upvar]/[uplevel] level reference type. Uses the longValue field
 * to remember the integer value of a parsed #<integer> format.
>>>>>>> upstream/master
=======
 * The [upvar]/[uplevel] level reference type. Uses the longValue field
 * to remember the integer value of a parsed #<integer> format.
>>>>>>> upstream/master
=======
 * The [upvar]/[uplevel] level reference type. Uses the longValue field
 * to remember the integer value of a parsed #<integer> format.
>>>>>>> upstream/master
=======
 * The [upvar]/[uplevel] level reference type. Uses the longValue field
 * to remember the integer value of a parsed #<integer> format.
>>>>>>> upstream/master
=======
 * The [upvar]/[uplevel] level reference type. Uses the longValue field
 * to remember the integer value of a parsed #<integer> format.
>>>>>>> upstream/master
 *
 * Uses the default behaviour throughout, and never disposes of the string
 * rep; it's just a cache type.
 */

static const Tcl_ObjType levelReferenceType = {
    "levelReference",
    NULL, NULL, NULL, NULL
};

/*
 * The type of lambdas. Note that every lambda will *always* have a string
 * representation.
 *
 * Internally, ptr1 is a pointer to a Proc instance that is not bound to a
 * command name, and ptr2 is a pointer to the namespace that the Proc instance
 * will execute within. IF YOU CHANGE THIS, CHECK IN tclDisassemble.c TOO.
 */

static const Tcl_ObjType lambdaType = {
    "lambdaExpr",		/* name */
    FreeLambdaInternalRep,	/* freeIntRepProc */
    DupLambdaInternalRep,	/* dupIntRepProc */
    NULL,			/* updateStringProc */
    SetLambdaFromAny		/* setFromAnyProc */
};

#define LambdaSetIntRep(objPtr, procPtr, nsObjPtr)			\
    do {								\
	Tcl_ObjIntRep ir;						\
	ir.twoPtrValue.ptr1 = (procPtr);				\
	ir.twoPtrValue.ptr2 = (nsObjPtr);				\
	Tcl_IncrRefCount((nsObjPtr));					\
	Tcl_StoreIntRep((objPtr), &lambdaType, &ir);			\
    } while (0)

#define LambdaGetIntRep(objPtr, procPtr, nsObjPtr)			\
    do {								\
	const Tcl_ObjIntRep *irPtr;					\
<<<<<<< HEAD
	irPtr = Tcl_FetchIntRep((objPtr), &lambdaType);			\
=======
	irPtr = TclFetchIntRep((objPtr), &lambdaType);			\
<<<<<<< HEAD
>>>>>>> upstream/master
	(procPtr) = irPtr ? irPtr->twoPtrValue.ptr1 : NULL;		\
	(nsObjPtr) = irPtr ? irPtr->twoPtrValue.ptr2 : NULL;		\
=======
	(procPtr) = irPtr ? (Proc *)irPtr->twoPtrValue.ptr1 : NULL;		\
	(nsObjPtr) = irPtr ? (Tcl_Obj *)irPtr->twoPtrValue.ptr2 : NULL;		\
>>>>>>> upstream/master
    } while (0)


/*
 *----------------------------------------------------------------------
 *
 * Tcl_ProcObjCmd --
 *
 *	This object-based function is invoked to process the "proc" Tcl
 *	command. See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl object result value.
 *
 * Side effects:
 *	A new procedure gets created.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_ProcObjCmd(
    TCL_UNUSED(ClientData),
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const objv[])	/* Argument objects. */
{
    Interp *iPtr = (Interp *) interp;
    Proc *procPtr;
    const char *procName;
    const char *simpleName, *procArgs, *procBody;
    Namespace *nsPtr, *altNsPtr, *cxtNsPtr;
    Tcl_Command cmd;

    if (objc != 4) {
	Tcl_WrongNumArgs(interp, 1, objv, "name args body");
	return TCL_ERROR;
    }

    /*
     * Determine the namespace where the procedure should reside. Unless the
     * command name includes namespace qualifiers, this will be the current
     * namespace.
     */

    procName = TclGetString(objv[1]);
    TclGetNamespaceForQualName(interp, procName, NULL, 0,
	    &nsPtr, &altNsPtr, &cxtNsPtr, &simpleName);

    if (nsPtr == NULL) {
	Tcl_SetObjResult(interp, Tcl_ObjPrintf(
		"can't create procedure \"%s\": unknown namespace",
		procName));
	Tcl_SetErrorCode(interp, "TCL", "VALUE", "COMMAND", NULL);
	return TCL_ERROR;
    }
    if (simpleName == NULL) {
	Tcl_SetObjResult(interp, Tcl_ObjPrintf(
		"can't create procedure \"%s\": bad procedure name",
		procName));
	Tcl_SetErrorCode(interp, "TCL", "VALUE", "COMMAND", NULL);
	return TCL_ERROR;
    }

    /*
     * Create the data structure to represent the procedure.
     */

    if (TclCreateProc(interp, /*ignored nsPtr*/ NULL, simpleName, objv[2],
	    objv[3], &procPtr) != TCL_OK) {
	Tcl_AddErrorInfo(interp, "\n    (creating proc \"");
	Tcl_AddErrorInfo(interp, simpleName);
	Tcl_AddErrorInfo(interp, "\")");
	return TCL_ERROR;
    }

    cmd = TclNRCreateCommandInNs(interp, simpleName, (Tcl_Namespace *) nsPtr,
	TclObjInterpProc, TclNRInterpProc, procPtr, TclProcDeleteProc);

    /*
     * Now initialize the new procedure's cmdPtr field. This will be used
     * later when the procedure is called to determine what namespace the
     * procedure will run in. This will be different than the current
     * namespace if the proc was renamed into a different namespace.
     */

    procPtr->cmdPtr = (Command *) cmd;

    /*
     * TIP #280: Remember the line the procedure body is starting on. In a
     * bytecode context we ask the engine to provide us with the necessary
     * information. This is for the initialization of the byte code compiler
     * when the body is used for the first time.
     *
     * This code is nearly identical to the #280 code in SetLambdaFromAny, see
     * this file. The differences are the different index of the body in the
     * line array of the context, and the lambda code requires some special
     * processing. Find a way to factor the common elements into a single
     * function.
     */

    if (iPtr->cmdFramePtr) {
	CmdFrame *contextPtr = (CmdFrame *)TclStackAlloc(interp, sizeof(CmdFrame));

	*contextPtr = *iPtr->cmdFramePtr;
	if (contextPtr->type == TCL_LOCATION_BC) {
	    /*
	     * Retrieve source information from the bytecode, if possible. If
	     * the information is retrieved successfully, context.type will be
	     * TCL_LOCATION_SOURCE and the reference held by
	     * context.data.eval.path will be counted.
	     */

	    TclGetSrcInfoForPc(contextPtr);
	} else if (contextPtr->type == TCL_LOCATION_SOURCE) {
	    /*
	     * The copy into 'context' up above has created another reference
	     * to 'context.data.eval.path'; account for it.
	     */

	    Tcl_IncrRefCount(contextPtr->data.eval.path);
	}

	if (contextPtr->type == TCL_LOCATION_SOURCE) {
	    /*
	     * We can account for source location within a proc only if the
	     * proc body was not created by substitution.
	     */

	    if (contextPtr->line
		    && (contextPtr->nline >= 4) && (contextPtr->line[3] >= 0)) {
		int isNew;
		Tcl_HashEntry *hePtr;
		CmdFrame *cfPtr = (CmdFrame *)Tcl_Alloc(sizeof(CmdFrame));

		cfPtr->level = -1;
		cfPtr->type = contextPtr->type;
		cfPtr->line = (int *)Tcl_Alloc(sizeof(int));
		cfPtr->line[0] = contextPtr->line[3];
		cfPtr->nline = 1;
		cfPtr->framePtr = NULL;
		cfPtr->nextPtr = NULL;

		cfPtr->data.eval.path = contextPtr->data.eval.path;
		Tcl_IncrRefCount(cfPtr->data.eval.path);

		cfPtr->cmd = NULL;
		cfPtr->len = 0;

		hePtr = Tcl_CreateHashEntry(iPtr->linePBodyPtr,
			procPtr, &isNew);
		if (!isNew) {
		    /*
		     * Get the old command frame and release it. See also
		     * TclProcCleanupProc in this file. Currently it seems as
		     * if only the procbodytest::proc command of the testsuite
		     * is able to trigger this situation.
		     */

		    CmdFrame *cfOldPtr = (CmdFrame *)Tcl_GetHashValue(hePtr);

		    if (cfOldPtr->type == TCL_LOCATION_SOURCE) {
			Tcl_DecrRefCount(cfOldPtr->data.eval.path);
			cfOldPtr->data.eval.path = NULL;
		    }
		    Tcl_Free(cfOldPtr->line);
		    cfOldPtr->line = NULL;
		    Tcl_Free(cfOldPtr);
		}
		Tcl_SetHashValue(hePtr, cfPtr);
	    }

	    /*
	     * 'contextPtr' is going out of scope; account for the reference
	     * that it's holding to the path name.
	     */

	    Tcl_DecrRefCount(contextPtr->data.eval.path);
	    contextPtr->data.eval.path = NULL;
	}
	TclStackFree(interp, contextPtr);
    }

    /*
     * Optimize for no-op procs: if the body is not precompiled (like a TclPro
     * procbody), and the argument list is just "args" and the body is empty,
     * define a compileProc to compile a no-op.
     *
     * Notes:
     *	 - cannot be done for any argument list without having different
     *	   compiled/not-compiled behaviour in the "wrong argument #" case, or
     *	   making this code much more complicated. In any case, it doesn't
     *	   seem to make a lot of sense to verify the number of arguments we
     *	   are about to ignore ...
     *	 - could be enhanced to handle also non-empty bodies that contain only
     *	   comments; however, parsing the body will slow down the compilation
     *	   of all procs whose argument list is just _args_
     */

    if (Tcl_FetchIntRep(objv[3], &tclProcBodyType)) {
	goto done;
    }

    procArgs = TclGetString(objv[2]);

    while (*procArgs == ' ') {
	procArgs++;
    }

    if ((procArgs[0] == 'a') && (strncmp(procArgs, "args", 4) == 0)) {
	size_t numBytes;

	procArgs +=4;
	while (*procArgs != '\0') {
	    if (*procArgs != ' ') {
		goto done;
	    }
	    procArgs++;
	}

	/*
	 * The argument list is just "args"; check the body
	 */

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	procBody = Tcl_GetStringFromObj(objv[3], &numBytes);
=======
	procBody = TclGetStringFromObj(objv[3], &numBytes);
>>>>>>> upstream/master
=======
	procBody = TclGetStringFromObj(objv[3], &numBytes);
>>>>>>> upstream/master
=======
	procBody = TclGetStringFromObj(objv[3], &numBytes);
>>>>>>> upstream/master
=======
	procBody = TclGetString(objv[3]);
	numBytes = objv[3]->length;
>>>>>>> upstream/master
=======
	procBody = TclGetStringFromObj(objv[3], &numBytes);
>>>>>>> upstream/master
	if (TclParseAllWhiteSpace(procBody, numBytes) < numBytes) {
	    goto done;
	}

	/*
	 * The body is just spaces: link the compileProc
	 */

	((Command *) cmd)->compileProc = TclCompileNoOp;
    }

  done:
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * TclCreateProc --
 *
 *	Creates the data associated with a Tcl procedure definition. This
 *	function knows how to handle two types of body objects: strings and
 *	procbody. Strings are the traditional (and common) value for bodies,
 *	procbody are values created by extensions that have loaded a
 *	previously compiled script.
 *
 * Results:
 *	Returns TCL_OK on success, along with a pointer to a Tcl procedure
 *	definition in procPtrPtr where the cmdPtr field is not initialised.
 *	This definition should be freed by calling TclProcCleanupProc() when
 *	it is no longer needed. Returns TCL_ERROR if anything goes wrong.
 *
 * Side effects:
 *	If anything goes wrong, this function returns an error message in the
 *	interpreter.
 *
 *----------------------------------------------------------------------
 */

int
TclCreateProc(
    Tcl_Interp *interp,		/* Interpreter containing proc. */
    TCL_UNUSED(Namespace *) /*nsPtr*/,
    const char *procName,	/* Unqualified name of this proc. */
    Tcl_Obj *argsPtr,		/* Description of arguments. */
    Tcl_Obj *bodyPtr,		/* Command body. */
    Proc **procPtrPtr)		/* Returns: pointer to proc data. */
{
    Interp *iPtr = (Interp *) interp;

<<<<<<< HEAD
    register Proc *procPtr = NULL;
    int i, result, numArgs;
    size_t plen;
    const char *bytes, *argname, *argnamei;
    char argnamelast;
    register CompiledLocal *localPtr = NULL;
    Tcl_Obj *defPtr, *errorObj, **argArray;
=======
    Proc *procPtr = NULL;
    int i, result, numArgs;
    CompiledLocal *localPtr = NULL;
    Tcl_Obj **argArray;
>>>>>>> upstream/master
    int precompiled = 0;

    ProcGetIntRep(bodyPtr, procPtr);
    if (procPtr != NULL) {
	/*
	 * Because the body is a TclProProcBody, the actual body is already
	 * compiled, and it is not shared with anyone else, so it's OK not to
	 * unshare it (as a matter of fact, it is bad to unshare it, because
	 * there may be no source code).
	 *
	 * We don't create and initialize a Proc structure for the procedure;
	 * rather, we use what is in the body object. We increment the ref
	 * count of the Proc struct since the command (soon to be created)
	 * will be holding a reference to it.
	 */

	procPtr->iPtr = iPtr;
	procPtr->refCount++;
	precompiled = 1;
    } else {
	/*
	 * If the procedure's body object is shared because its string value
	 * is identical to, e.g., the body of another procedure, we must
	 * create a private copy for this procedure to use. Such sharing of
	 * procedure bodies is rare but can cause problems. A procedure body
	 * is compiled in a context that includes the number of "slots"
	 * allocated by the compiler for local variables. There is a local
	 * variable slot for each formal parameter (the
	 * "procPtr->numCompiledLocals = numArgs" assignment below). This
	 * means that the same code can not be shared by two procedures that
	 * have a different number of arguments, even if their bodies are
	 * identical. Note that we don't use Tcl_DuplicateObj since we would
	 * not want any bytecode internal representation.
	 */

	if (Tcl_IsShared(bodyPtr)) {
<<<<<<< HEAD
=======
	    const char *bytes;
>>>>>>> upstream/master
	    size_t length;
	    Tcl_Obj *sharedBodyPtr = bodyPtr;

	    bytes = TclGetStringFromObj(bodyPtr, &length);
	    bodyPtr = Tcl_NewStringObj(bytes, length);

	    /*
	     * TIP #280.
	     * Ensure that the continuation line data for the original body is
	     * not lost and applies to the new body as well.
	     */

	    TclContinuationsCopy(bodyPtr, sharedBodyPtr);
	}

	/*
	 * Create and initialize a Proc structure for the procedure. We
	 * increment the ref count of the procedure's body object since there
	 * will be a reference to it in the Proc structure.
	 */

	Tcl_IncrRefCount(bodyPtr);

	procPtr = (Proc *)Tcl_Alloc(sizeof(Proc));
	procPtr->iPtr = iPtr;
	procPtr->refCount = 1;
	procPtr->bodyPtr = bodyPtr;
	procPtr->numArgs = 0;	/* Actual argument count is set below. */
	procPtr->numCompiledLocals = 0;
	procPtr->firstLocalPtr = NULL;
	procPtr->lastLocalPtr = NULL;
    }

    /*
     * Break up the argument list into argument specifiers, then process each
     * argument specifier. If the body is precompiled, processing is limited
     * to checking that the parsed argument is consistent with the one stored
     * in the Proc.
     */

    result = Tcl_ListObjGetElements(interp , argsPtr ,&numArgs ,&argArray);
    if (result != TCL_OK) {
	goto procError;
    }

    if (precompiled) {
	if (numArgs > procPtr->numArgs) {
	    Tcl_SetObjResult(interp, Tcl_ObjPrintf(
		    "procedure \"%s\": arg list contains %d entries, "
		    "precompiled header expects %d", procName, numArgs,
		    procPtr->numArgs));
	    Tcl_SetErrorCode(interp, "TCL", "OPERATION", "PROC",
		    "BYTECODELIES", NULL);
	    goto procError;
	}
	localPtr = procPtr->firstLocalPtr;
    } else {
	procPtr->numArgs = numArgs;
	procPtr->numCompiledLocals = numArgs;
    }

    for (i = 0; i < numArgs; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	int fieldCount, nameLength, valueLength;
=======
	int fieldCount, nameLength;
	size_t valueLength;
<<<<<<< HEAD
>>>>>>> upstream/master
=======
	int fieldCount, nameLength;
	size_t valueLength;
>>>>>>> upstream/master
	const char **fieldValues;
=======
	Tcl_Obj **fieldValues;
>>>>>>> upstream/master
=======
	const char *argname, *argnamei, *argnamelast;
	int fieldCount;
	size_t nameLength;
	Tcl_Obj **fieldValues;
>>>>>>> upstream/master

	/*
	 * Now divide the specifier up into name and default.
	 */

	result = Tcl_ListObjGetElements(interp, argArray[i], &fieldCount,
		&fieldValues);
	if (result != TCL_OK) {
	    goto procError;
	}
	if (fieldCount > 2) {
<<<<<<< HEAD
	    errorObj = Tcl_NewStringObj(
=======
	    Tcl_Obj *errorObj = Tcl_NewStringObj(
>>>>>>> upstream/master
		"too many fields in argument specifier \"", -1);
	    Tcl_AppendObjToObj(errorObj, argArray[i]);
	    Tcl_AppendToObj(errorObj, "\"", -1);
	    Tcl_SetObjResult(interp, errorObj);
	    Tcl_SetErrorCode(interp, "TCL", "OPERATION", "PROC",
		    "FORMALARGUMENTFORMAT", NULL);
	    goto procError;
	}
	if ((fieldCount == 0) || (fieldValues[0]->length == 0)) {
	    Tcl_SetObjResult(interp, Tcl_NewStringObj(
		    "argument with no name", -1));
	    Tcl_SetErrorCode(interp, "TCL", "OPERATION", "PROC",
		    "FORMALARGUMENTFORMAT", NULL);
	    goto procError;
	}

<<<<<<< HEAD
	argname = TclGetStringFromObj(fieldValues[0], &plen);
	nameLength = Tcl_NumUtfChars(argname, plen);
	if (fieldCount == 2) {
	    const char * value = TclGetString(fieldValues[1]);
	    valueLength = Tcl_NumUtfChars(value, fieldValues[1]->length);
	} else {
	    valueLength = 0;
	}
=======
	argname = TclGetStringFromObj(fieldValues[0], &nameLength);
>>>>>>> upstream/master

	/*
	 * Check that the formal parameter name is a scalar.
	 */

	argnamei = argname;
<<<<<<< HEAD
<<<<<<< HEAD
	argnamelast = argname[plen-1];
	while (plen--) {
	    if (argnamei[0] == '(') {
		if (argnamelast == ')') {	/* We have an array element. */
		    Tcl_SetObjResult(interp, Tcl_ObjPrintf(
			    "formal parameter \"%s\" is an array element",
			    Tcl_GetString(fieldValues[0])));
=======
	argnamelast = Tcl_UtfPrev(argname + nameLength, argname);
=======
	argnamelast = (nameLength > 0) ? (argname + nameLength - 1) : argname;
>>>>>>> upstream/master
	while (argnamei < argnamelast) {
	    if (*argnamei == '(') {
		if (*argnamelast == ')') { /* We have an array element. */
		    Tcl_SetObjResult(interp, Tcl_ObjPrintf(
			    "formal parameter \"%s\" is an array element",
			    TclGetString(fieldValues[0])));
>>>>>>> upstream/master
		    Tcl_SetErrorCode(interp, "TCL", "OPERATION", "PROC",
			    "FORMALARGUMENTFORMAT", NULL);
		    goto procError;
		}
<<<<<<< HEAD
	    } else if ((argnamei[0] == ':') && (argnamei[1] == ':')) {
		errorObj = Tcl_NewStringObj("formal parameter \"", -1);
=======
	    } else if (*argnamei == ':' && *(argnamei+1) == ':') {
		Tcl_Obj *errorObj = Tcl_NewStringObj(
		    "formal parameter \"", -1);
>>>>>>> upstream/master
		Tcl_AppendObjToObj(errorObj, fieldValues[0]);
		Tcl_AppendToObj(errorObj, "\" is not a simple name", -1);
		Tcl_SetObjResult(interp, errorObj);
		Tcl_SetErrorCode(interp, "TCL", "OPERATION", "PROC",
			"FORMALARGUMENTFORMAT", NULL);
		goto procError;
	    }
	    argnamei++;
	}

	if (precompiled) {
	    /*
	     * Compare the parsed argument with the stored one. Note that the
	     * only flag value that makes sense at this point is VAR_ARGUMENT
	     * (its value was kept the same as pre VarReform to simplify
	     * tbcload's processing of older byetcodes).
	     *
	     * The only other flag vlaue that is important to retrieve from
	     * precompiled procs is VAR_TEMPORARY (also unchanged). It is
	     * needed later when retrieving the variable names.
	     */

	    if ((localPtr->nameLength != nameLength)
<<<<<<< HEAD
		    || (Tcl_UtfNcmp(localPtr->name, argname, nameLength))
=======
		    || (memcmp(localPtr->name, argname, nameLength) != 0)
>>>>>>> upstream/master
		    || (localPtr->frameIndex != i)
		    || !(localPtr->flags & VAR_ARGUMENT)
		    || (localPtr->defValuePtr == NULL && fieldCount == 2)
		    || (localPtr->defValuePtr != NULL && fieldCount != 2)) {
		Tcl_SetObjResult(interp, Tcl_ObjPrintf(
			"procedure \"%s\": formal parameter %d is "
			"inconsistent with precompiled body", procName, i));
		Tcl_SetErrorCode(interp, "TCL", "OPERATION", "PROC",
			"BYTECODELIES", NULL);
		goto procError;
	    }

	    /*
	     * Compare the default value if any.
	     */

	    if (localPtr->defValuePtr != NULL) {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		int tmpLength;
		const char *tmpPtr = TclGetStringFromObj(localPtr->defValuePtr,
			&tmpLength);

		if ((valueLength != tmpLength) ||
			strncmp(fieldValues[1], tmpPtr, (size_t) tmpLength)) {
=======
		const char *tmpPtr = TclGetString(localPtr->defValuePtr);
		size_t tmpLength = localPtr->defValuePtr->length;

		if ((valueLength != tmpLength) ||
			strncmp(fieldValues[1], tmpPtr, tmpLength)) {
>>>>>>> upstream/master
=======
		const char *tmpPtr = TclGetString(localPtr->defValuePtr);
		size_t tmpLength = localPtr->defValuePtr->length;

		if ((valueLength != tmpLength) ||
<<<<<<< HEAD
			strncmp(fieldValues[1], tmpPtr, tmpLength)) {
>>>>>>> upstream/master
		    Tcl_SetObjResult(interp, Tcl_ObjPrintf(
			    "procedure \"%s\": formal parameter \"%s\" has "
			    "default value inconsistent with precompiled body",
			    procName, fieldValues[0]));
		    ckfree(fieldValues);
=======
			Tcl_UtfNcmp(Tcl_GetString(fieldValues[1]), tmpPtr, tmpLength)) {
		    errorObj = Tcl_ObjPrintf(
			    "procedure \"%s\": formal parameter \"" ,procName);
=======
		size_t tmpLength, valueLength;
		const char *tmpPtr = TclGetStringFromObj(localPtr->defValuePtr, &tmpLength);
		const char *value = TclGetStringFromObj(fieldValues[1], &valueLength);

		if ((valueLength != tmpLength)
		     || memcmp(value, tmpPtr, tmpLength) != 0
		) {
		    Tcl_Obj *errorObj = Tcl_ObjPrintf(
			    "procedure \"%s\": formal parameter \"", procName);
>>>>>>> upstream/master
		    Tcl_AppendObjToObj(errorObj, fieldValues[0]);
		    Tcl_AppendToObj(errorObj, "\" has "
			"default value inconsistent with precompiled body", -1);
		    Tcl_SetObjResult(interp, errorObj);
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
		    Tcl_SetErrorCode(interp, "TCL", "OPERATION", "PROC",
			    "BYTECODELIES", NULL);
		    goto procError;
		}
	    }
	    if ((i == numArgs - 1)
		    && (localPtr->nameLength == 4)
		    && (localPtr->name[0] == 'a')
		    && (strcmp(localPtr->name, "args") == 0)) {
		localPtr->flags |= VAR_IS_ARGS;
	    }

	    localPtr = localPtr->nextPtr;
	} else {
	    /*
	     * Allocate an entry in the runtime procedure frame's array of
	     * local variables for the argument.
	     */

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	    localPtr = Tcl_Alloc(TclOffset(CompiledLocal, name) + fieldValues[0]->length +1);
=======
	    localPtr = Tcl_Alloc(offsetof(CompiledLocal, name) + fieldValues[0]->length +1);
>>>>>>> upstream/master
=======
	    localPtr = (CompiledLocal *)Tcl_Alloc(offsetof(CompiledLocal, name) + fieldValues[0]->length +1);
>>>>>>> upstream/master
=======
	    localPtr = (CompiledLocal *)Tcl_Alloc(
		    offsetof(CompiledLocal, name) + fieldValues[0]->length + 1);
>>>>>>> upstream/master
	    if (procPtr->firstLocalPtr == NULL) {
		procPtr->firstLocalPtr = procPtr->lastLocalPtr = localPtr;
	    } else {
		procPtr->lastLocalPtr->nextPtr = localPtr;
		procPtr->lastLocalPtr = localPtr;
	    }
	    localPtr->nextPtr = NULL;
	    localPtr->nameLength = nameLength;
	    localPtr->frameIndex = i;
	    localPtr->flags = VAR_ARGUMENT;
	    localPtr->resolveInfo = NULL;

	    if (fieldCount == 2) {
		localPtr->defValuePtr = fieldValues[1];
		Tcl_IncrRefCount(localPtr->defValuePtr);
	    } else {
		localPtr->defValuePtr = NULL;
	    }
	    memcpy(localPtr->name, argname, fieldValues[0]->length + 1);
	    if ((i == numArgs - 1)
		    && (localPtr->nameLength == 4)
		    && (localPtr->name[0] == 'a')
		    && (memcmp(localPtr->name, "args", 4) == 0)) {
		localPtr->flags |= VAR_IS_ARGS;
	    }
	}
    }

    *procPtrPtr = procPtr;
    return TCL_OK;

  procError:
    if (precompiled) {
	procPtr->refCount--;
    } else {
	Tcl_DecrRefCount(bodyPtr);
	while (procPtr->firstLocalPtr != NULL) {
	    localPtr = procPtr->firstLocalPtr;
	    procPtr->firstLocalPtr = localPtr->nextPtr;

	    if (localPtr->defValuePtr != NULL) {
		Tcl_DecrRefCount(localPtr->defValuePtr);
	    }

	    Tcl_Free(localPtr);
	}
	Tcl_Free(procPtr);
    }
    return TCL_ERROR;
}

/*
 *----------------------------------------------------------------------
 *
 * TclGetFrame --
 *
 *	Given a description of a procedure frame, such as the first argument
 *	to an "uplevel" or "upvar" command, locate the call frame for the
 *	appropriate level of procedure.
 *
 * Results:
 *	The return value is -1 if an error occurred in finding the frame (in
 *	this case an error message is left in the interp's result). 1 is
 *	returned if string was either a number or a number preceded by "#" and
 *	it specified a valid frame. 0 is returned if string isn't one of the
 *	two things above (in this case, the lookup acts as if string were
 *	"1"). The variable pointed to by framePtrPtr is filled in with the
 *	address of the desired frame (unless an error occurs, in which case it
 *	isn't modified).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
TclGetFrame(
    Tcl_Interp *interp,		/* Interpreter in which to find frame. */
    const char *name,		/* String describing frame. */
    CallFrame **framePtrPtr)	/* Store pointer to frame here (or NULL if
				 * global frame indicated). */
{
<<<<<<< HEAD
<<<<<<< HEAD
    register Interp *iPtr = (Interp *) interp;
    int curLevel, level, result;
    CallFrame *framePtr;

    /*
     * Parse string to figure out which level number to go to.
     */

    result = 1;
    curLevel = iPtr->varFramePtr->level;
    if (*name== '#') {
	if (Tcl_GetInt(interp, name+1, &level) != TCL_OK || level < 0) {
	    goto levelError;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
	}
    } else if (isdigit(UCHAR(*name))) { /* INTL: digit */
	if (Tcl_GetInt(interp, name, &level) != TCL_OK) {
	    goto levelError;
	}
	level = curLevel - level;
    } else {
	level = curLevel - 1;
	result = 0;
    }

    /*
     * Figure out which frame to use, and return it to the caller.
<<<<<<< HEAD
<<<<<<< HEAD
     */

    for (framePtr = iPtr->varFramePtr; framePtr != NULL;
	    framePtr = framePtr->callerVarPtr) {
	if (framePtr->level == level) {
	    break;
	}
    }
    if (framePtr == NULL) {
	goto levelError;
    }

    *framePtrPtr = framePtr;
    return result;

  levelError:
    Tcl_SetObjResult(interp, Tcl_ObjPrintf("bad level \"%s\"", name));
    Tcl_SetErrorCode(interp, "TCL", "VALUE", "STACKLEVEL", NULL);
<<<<<<< HEAD
=======
     */

    for (framePtr = iPtr->varFramePtr; framePtr != NULL;
	    framePtr = framePtr->callerVarPtr) {
	if (framePtr->level == level) {
	    break;
	}
    }
    if (framePtr == NULL) {
	goto levelError;
    }

    *framePtrPtr = framePtr;
    return result;

  levelError:
    Tcl_SetObjResult(interp, Tcl_ObjPrintf("bad level \"%s\"", name));
    Tcl_SetErrorCode(interp, "TCL", "VALUE", "STACKLEVEL", NULL);
=======
     */

    for (framePtr = iPtr->varFramePtr; framePtr != NULL;
	    framePtr = framePtr->callerVarPtr) {
	if (framePtr->level == level) {
	    break;
	}
    }
    if (framePtr == NULL) {
	goto levelError;
    }

    *framePtrPtr = framePtr;
    return result;

  levelError:
    Tcl_SetObjResult(interp, Tcl_ObjPrintf("bad level \"%s\"", name));
    Tcl_SetErrorCode(interp, "TCL", "VALUE", "STACKLEVEL", NULL);
=======
>>>>>>> upstream/master
    return -1;
=======
=======
>>>>>>> upstream/master
	int result;
	Tcl_Obj obj;

	obj.bytes = (char *) name;
	obj.length = strlen(name);
	obj.typePtr = NULL;
	result = TclObjGetFrame(interp, &obj, framePtrPtr);
	TclFreeIntRep(&obj);
	return result;
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
}

/*
 *----------------------------------------------------------------------
 *
 * TclObjGetFrame --
 *
 *	Given a description of a procedure frame, such as the first argument
 *	to an "uplevel" or "upvar" command, locate the call frame for the
 *	appropriate level of procedure.
 *
 * Results:
 *	The return value is -1 if an error occurred in finding the frame (in
 *	this case an error message is left in the interp's result). 1 is
 *	returned if objPtr was either an int or an int preceded by "#" and
 *	it specified a valid frame. 0 is returned if objPtr isn't one of the
 *	two things above (in this case, the lookup acts as if objPtr were
 *	"1"). The variable pointed to by framePtrPtr is filled in with the
 *	address of the desired frame (unless an error occurs, in which case it
 *	isn't modified).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
TclObjGetFrame(
    Tcl_Interp *interp,		/* Interpreter in which to find frame. */
    Tcl_Obj *objPtr,		/* Object describing frame. */
    CallFrame **framePtrPtr)	/* Store pointer to frame here (or NULL if
				 * global frame indicated). */
{
    Interp *iPtr = (Interp *) interp;
    int curLevel, level, result;
    const Tcl_ObjIntRep *irPtr;
    const char *name = NULL;
    Tcl_WideInt w;

    /*
     * Parse object to figure out which level number to go to.
     */

    result = 0;
    curLevel = iPtr->varFramePtr->level;

    /*
     * Check for integer first, since that has potential to spare us
     * a generation of a stringrep.
     */

    if (objPtr == NULL) {
	/* Do nothing */
    } else if (TCL_OK == Tcl_GetIntFromObj(NULL, objPtr, &level)) {
	Tcl_GetWideIntFromObj(NULL, objPtr, &w);
	if (w < 0 || w > INT_MAX || curLevel > w + INT_MAX) {
	    result = -1;
	} else {
	    level = curLevel - level;
	    result = 1;
	}
<<<<<<< HEAD
    } else if ((irPtr = Tcl_FetchIntRep(objPtr, &levelReferenceType))) {
=======
    } else if ((irPtr = TclFetchIntRep(objPtr, &levelReferenceType))) {
>>>>>>> upstream/master
	level = irPtr->wideValue;
	result = 1;
    } else {
	name = TclGetString(objPtr);
	if (name[0] == '#') {
	    if (TCL_OK == Tcl_GetInt(NULL, name+1, &level)) {
		if (level < 0 || (level > 0 && name[1] == '-')) {
		    result = -1;
		} else {
		    Tcl_ObjIntRep ir;

		    ir.wideValue = level;
		    Tcl_StoreIntRep(objPtr, &levelReferenceType, &ir);
		    result = 1;
		}
	    } else {
		result = -1;
	    }
<<<<<<< HEAD
	} else if (TclGetWideBitsFromObj(interp, objPtr, &w) == TCL_OK) {
=======
	} else if (TclGetWideBitsFromObj(NULL, objPtr, &w) == TCL_OK) {
>>>>>>> upstream/master
	    /*
	     * If this were an integer, we'd have succeeded already.
	     * Docs say we have to treat this as a 'bad level'  error.
	     */
	    result = -1;
	}
<<<<<<< HEAD
    }

    if (result == 0) {
	level = curLevel - 1;
    }
    if (result != -1) {
=======
    }

    if (result != -1) {
	/* if relative current level */
	if (result == 0) {
	    if (!curLevel) {
		/* we are in top-level, so simply generate bad level */
		name = "1";
		goto badLevel;
	    }
	    level = curLevel - 1;
	}
>>>>>>> upstream/master
	if (level >= 0) {
	    CallFrame *framePtr;
	    for (framePtr = iPtr->varFramePtr; framePtr != NULL;
		    framePtr = framePtr->callerVarPtr) {
		if (framePtr->level == level) {
		    *framePtrPtr = framePtr;
		    return result;
		}
	    }
	}
    }
<<<<<<< HEAD

    if (name == NULL) {
	name = TclGetString(objPtr);
    }
    Tcl_SetObjResult(interp, Tcl_ObjPrintf("bad level \"%s\"", name));
    Tcl_SetErrorCode(interp, "TCL", "LOOKUP", "LEVEL", name, NULL);
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
badLevel:
    if (name == NULL) {
	name = objPtr ? TclGetString(objPtr) : "1" ;
    }
    Tcl_SetObjResult(interp, Tcl_ObjPrintf("bad level \"%s\"", name));
    Tcl_SetErrorCode(interp, "TCL", "LOOKUP", "LEVEL", name, NULL);
>>>>>>> upstream/master
    return -1;
}

/*
 *----------------------------------------------------------------------
 *
 * TclObjGetFrame --
 *
<<<<<<< HEAD
 *	Given a description of a procedure frame, such as the first argument
 *	to an "uplevel" or "upvar" command, locate the call frame for the
 *	appropriate level of procedure.
=======
 *	This object function is invoked to process the "uplevel" Tcl command.
 *	See the user documentation for details on what it does.
>>>>>>> upstream/master
 *
 * Results:
 *	The return value is -1 if an error occurred in finding the frame (in
 *	this case an error message is left in the interp's result). 1 is
 *	returned if objPtr was either an int or an int preceded by "#" and
 *	it specified a valid frame. 0 is returned if objPtr isn't one of the
 *	two things above (in this case, the lookup acts as if objPtr were
 *	"1"). The variable pointed to by framePtrPtr is filled in with the
 *	address of the desired frame (unless an error occurs, in which case it
 *	isn't modified).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

<<<<<<< HEAD
int
TclObjGetFrame(
    Tcl_Interp *interp,		/* Interpreter in which to find frame. */
    Tcl_Obj *objPtr,		/* Object describing frame. */
    CallFrame **framePtrPtr)	/* Store pointer to frame here (or NULL if
				 * global frame indicated). */
=======
static int
Uplevel_Callback(
    ClientData data[],
    Tcl_Interp *interp,
    int result)
{
    CallFrame *savedVarFramePtr = (CallFrame *)data[0];

    if (result == TCL_ERROR) {
	Tcl_AppendObjToErrorInfo(interp, Tcl_ObjPrintf(
		"\n    (\"uplevel\" body line %d)", Tcl_GetErrorLine(interp)));
    }

    /*
     * Restore the variable frame, and return.
     */

    ((Interp *)interp)->varFramePtr = savedVarFramePtr;
    return result;
}

int
Tcl_UplevelObjCmd(
    ClientData clientData,
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const objv[])	/* Argument objects. */
{
    return Tcl_NRCallObjProc(interp, TclNRUplevelObjCmd, clientData, objc, objv);
}

int
TclNRUplevelObjCmd(
    TCL_UNUSED(ClientData),
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const objv[])	/* Argument objects. */
>>>>>>> upstream/master
{

<<<<<<< HEAD
    register Interp *iPtr = (Interp *) interp;
<<<<<<< HEAD
    int curLevel, level, result;
    const char *name = NULL;
=======
=======
    Interp *iPtr = (Interp *) interp;
>>>>>>> upstream/master
    CmdFrame *invoker = NULL;
    int word = 0;
    int result;
    CallFrame *savedVarFramePtr, *framePtr;
    Tcl_Obj *objPtr;

    if (objc < 2) {
    /* to do
    *    simplify things by interpreting the argument as a command when there
    *    is only one argument.  This requires a TIP since currently a single
    *    argument is interpreted as a level indicator if possible.
    */
    uplevelSyntax:
	Tcl_WrongNumArgs(interp, 1, objv, "?level? command ?arg ...?");
	return TCL_ERROR;
    } else if (!TclHasStringRep(objv[1]) && objc == 2) {
	int status ,llength;
	status = Tcl_ListObjLength(interp, objv[1], &llength);
	if (status == TCL_OK && llength > 1) {
	    /* the first argument can't interpreted as a level. Avoid
	     * generating a string representation of the script. */
	    result = TclGetFrame(interp, "1", &framePtr);
	    if (result == -1) {
		return TCL_ERROR;
	    }
	    objc -= 1;
	    objv += 1;
	    goto havelevel;
	}
    }

    /*
     * Find the level to use for executing the command.
     */

    result = TclObjGetFrame(interp, objv[1], &framePtr);
    if (result == -1) {
	return TCL_ERROR;
    }
    objc -= result + 1;
    if (objc == 0) {
	goto uplevelSyntax;
    }
    objv += result + 1;
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master

    havelevel:

    /*
     * Parse object to figure out which level number to go to.
     */

    result = 0;
    curLevel = iPtr->varFramePtr->level;

    /*
     * Check for integer first, since that has potential to spare us
     * a generation of a stringrep.
     */

<<<<<<< HEAD
<<<<<<< HEAD
    if (objPtr == NULL) {
	/* Do nothing */
    } else if (TCL_OK == Tcl_GetIntFromObj(NULL, objPtr, &level)
	    && (level >= 0)) {
	level = curLevel - level;
	result = 1;
    } else if (objPtr->typePtr == &levelReferenceType) {
	level = (int) objPtr->internalRep.longValue;
	result = 1;
    } else {
	name = TclGetString(objPtr);
	if (name[0] == '#') {
	    if (TCL_OK == Tcl_GetInt(NULL, name+1, &level) && level >= 0) {
		TclFreeIntRep(objPtr);
		objPtr->typePtr = &levelReferenceType;
		objPtr->internalRep.longValue = level;
		result = 1;
	    } else {
		result = -1;
	    }
	} else if (isdigit(UCHAR(name[0]))) { /* INTL: digit */
	    /*
	     * If this were an integer, we'd have succeeded already.
	     * Docs say we have to treat this as a 'bad level'  error.
	     */
	    result = -1;
	}
    }

    if (result == 0) {
	level = curLevel - 1;
	name = "1";
    }
    if (result != -1) {
	if (level >= 0) {
	    CallFrame *framePtr;
	    for (framePtr = iPtr->varFramePtr; framePtr != NULL;
		    framePtr = framePtr->callerVarPtr) {
		if (framePtr->level == level) {
		    *framePtrPtr = framePtr;
		    return result;
		}
	    }
	}
	if (name == NULL) {
	    name = TclGetString(objPtr);
	}
    }

    Tcl_SetObjResult(interp, Tcl_ObjPrintf("bad level \"%s\"", name));
    Tcl_SetErrorCode(interp, "TCL", "LOOKUP", "LEVEL", name, NULL);
>>>>>>> upstream/master
    return -1;
=======
=======
>>>>>>> upstream/master
    if (objc == 1) {
	/*
	 * TIP #280. Make actual argument location available to eval'd script
	 */

	TclArgumentGet(interp, objv[0], &invoker, &word);
	objPtr = objv[0];

    } else {
	/*
	 * More than one argument: concatenate them together with spaces
	 * between, then evaluate the result. Tcl_EvalObjEx will delete the
	 * object when it decrements its refcount after eval'ing it.
	 */

	objPtr = Tcl_ConcatObj(objc, objv);
    }

    TclNRAddCallback(interp, Uplevel_Callback, savedVarFramePtr, NULL, NULL,
	    NULL);
    return TclNREvalObjEx(interp, objPtr, 0, invoker, word);
>>>>>>> upstream/master
}

/*
 *----------------------------------------------------------------------
 *
 * TclObjGetFrame --
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *	Given a description of a procedure frame, such as the first argument
 *	to an "uplevel" or "upvar" command, locate the call frame for the
 *	appropriate level of procedure.
=======
 *	This object function is invoked to process the "uplevel" Tcl command.
 *	See the user documentation for details on what it does.
>>>>>>> upstream/master
 *
 * Results:
 *	The return value is -1 if an error occurred in finding the frame (in
 *	this case an error message is left in the interp's result). 1 is
 *	returned if objPtr was either a number or a number preceded by "#" and
 *	it specified a valid frame. 0 is returned if objPtr isn't one of the
 *	two things above (in this case, the lookup acts as if objPtr were
 *	"1"). The variable pointed to by framePtrPtr is filled in with the
 *	address of the desired frame (unless an error occurs, in which case it
 *	isn't modified).
=======
 *	Given the name of a procedure, return a pointer to the record
 *	describing the procedure. The procedure will be looked up using the
 *	usual rules: first in the current namespace and then in the global
 *	namespace.
 *
 * Results:
 *	NULL is returned if the name doesn't correspond to any procedure.
 *	Otherwise, the return value is a pointer to the procedure's record. If
 *	the name is found but refers to an imported command that points to a
 *	"real" procedure defined in another namespace, a pointer to that
 *	"real" procedure's structure is returned.
>>>>>>> upstream/master
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

<<<<<<< HEAD
<<<<<<< HEAD
int
TclObjGetFrame(
    Tcl_Interp *interp,		/* Interpreter in which to find frame. */
    Tcl_Obj *objPtr,		/* Object describing frame. */
    CallFrame **framePtrPtr)	/* Store pointer to frame here (or NULL if
				 * global frame indicated). */
=======
static int
Uplevel_Callback(
    ClientData data[],
    Tcl_Interp *interp,
    int result)
{
    CallFrame *savedVarFramePtr = data[0];

    if (result == TCL_ERROR) {
	Tcl_AppendObjToErrorInfo(interp, Tcl_ObjPrintf(
		"\n    (\"uplevel\" body line %d)", Tcl_GetErrorLine(interp)));
    }

    /*
     * Restore the variable frame, and return.
     */

    ((Interp *)interp)->varFramePtr = savedVarFramePtr;
    return result;
}

	/* ARGSUSED */
int
Tcl_UplevelObjCmd(
    ClientData dummy,		/* Not used. */
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const objv[])	/* Argument objects. */
{
<<<<<<< HEAD
    return Tcl_NRCallObjProc(interp, TclNRUplevelObjCmd, dummy, objc, objv);
=======
    Tcl_Command origCmd = TclGetOriginalCommand((Tcl_Command) cmdPtr);

    if (origCmd != NULL) {
	cmdPtr = (Command *) origCmd;
    }
    if (cmdPtr->deleteProc == TclProcDeleteProc) {
	return (Proc *)cmdPtr->objClientData;
    }
    return NULL;
>>>>>>> upstream/master
}
<<<<<<< HEAD
=======

static int
ProcWrongNumArgs(
    Tcl_Interp *interp,
    int skip)
{
    CallFrame *framePtr = ((Interp *)interp)->varFramePtr;
    register Proc *procPtr = framePtr->procPtr;
    int localCt = procPtr->numCompiledLocals, numArgs, i;
    Tcl_Obj **desiredObjs;
    const char *final = NULL;
>>>>>>> upstream/master

int
TclNRUplevelObjCmd(
    ClientData dummy,		/* Not used. */
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const objv[])	/* Argument objects. */
>>>>>>> upstream/master
{

<<<<<<< HEAD
    register Interp *iPtr = (Interp *) interp;
<<<<<<< HEAD
    int curLevel, level, result;
    CallFrame *framePtr;
    const char *name;
=======
    CmdFrame *invoker = NULL;
    int word = 0;
    int result;
    CallFrame *savedVarFramePtr, *framePtr;
    Tcl_Obj *objPtr;
=======
    numArgs = framePtr->procPtr->numArgs;
    desiredObjs = (Tcl_Obj **)TclStackAlloc(interp,
	    sizeof(Tcl_Obj *) * (numArgs+1));
>>>>>>> upstream/master

    if (objc < 2) {
    uplevelSyntax:
	Tcl_WrongNumArgs(interp, 1, objv, "?level? command ?arg ...?");
	return TCL_ERROR;
    }
>>>>>>> upstream/master

    /*
     * Parse object to figure out which level number to go to.
     */

<<<<<<< HEAD
    result = 1;
    curLevel = iPtr->varFramePtr->level;
    if (objPtr == NULL) {
	name = "1";
	goto haveLevel1;
    }
=======
    result = TclObjGetFrame(interp, objv[1], &framePtr);
    if (result == -1) {
	return TCL_ERROR;
    }
    objc -= result + 1;
    if (objc == 0) {
	goto uplevelSyntax;
    }
    objv += result + 1;
>>>>>>> upstream/master

    name = TclGetString(objPtr);
    if (objPtr->typePtr == &levelReferenceType) {
	if (objPtr->internalRep.twoPtrValue.ptr1) {
	    level = curLevel - PTR2INT(objPtr->internalRep.twoPtrValue.ptr2);
	} else {
	    level = PTR2INT(objPtr->internalRep.twoPtrValue.ptr2);
	}
	if (level < 0) {
	    goto levelError;
	}
	/* TODO: Consider skipping the typePtr checks */
    } else if (objPtr->typePtr == &tclIntType
#ifndef TCL_WIDE_INT_IS_LONG
	    || objPtr->typePtr == &tclWideIntType
#endif
	    ) {
	if (TclGetIntFromObj(NULL, objPtr, &level) != TCL_OK || level < 0) {
	    goto levelError;
	}
	level = curLevel - level;
    } else if (*name == '#') {
	if (Tcl_GetInt(interp, name+1, &level) != TCL_OK || level < 0) {
	    goto levelError;
	}

	/*
	 * Cache for future reference.
	 */

	TclFreeIntRep(objPtr);
	objPtr->typePtr = &levelReferenceType;
	objPtr->internalRep.twoPtrValue.ptr1 = (void *) 0;
	objPtr->internalRep.twoPtrValue.ptr2 = INT2PTR(level);
    } else if (isdigit(UCHAR(*name))) { /* INTL: digit */
	if (Tcl_GetInt(interp, name, &level) != TCL_OK) {
	    return -1;
=======
	}
    } else if (isdigit(UCHAR(*name))) { /* INTL: digit */
	if (Tcl_GetInt(interp, name, &level) != TCL_OK) {
	    goto levelError;
>>>>>>> upstream/master
	}

<<<<<<< HEAD
	/*
	 * Cache for future reference.
	 */

	TclFreeIntRep(objPtr);
	objPtr->typePtr = &levelReferenceType;
	objPtr->internalRep.twoPtrValue.ptr1 = (void *) 1;
	objPtr->internalRep.twoPtrValue.ptr2 = INT2PTR(level);
	level = curLevel - level;
    } else {
	/*
	 * Don't cache as the object *isn't* a level reference (might even be
	 * NULL...)
	 */

    haveLevel1:
	level = curLevel - 1;
	result = 0;
    }

    /*
     * Figure out which frame to use, and return it to the caller.
<<<<<<< HEAD
     */

    for (framePtr = iPtr->varFramePtr; framePtr != NULL;
	    framePtr = framePtr->callerVarPtr) {
	if (framePtr->level == level) {
	    break;
	}
    }
    if (framePtr == NULL) {
	goto levelError;
    }
    *framePtrPtr = framePtr;
    return result;

  levelError:
=======
     */

    for (framePtr = iPtr->varFramePtr; framePtr != NULL;
	    framePtr = framePtr->callerVarPtr) {
	if (framePtr->level == level) {
	    break;
	}
    }
    if (framePtr == NULL) {
	goto levelError;
    }

    *framePtrPtr = framePtr;
    return result;

  levelError:
    Tcl_SetObjResult(interp, Tcl_ObjPrintf("bad level \"%s\"", name));
    Tcl_SetErrorCode(interp, "TCL", "VALUE", "STACKLEVEL", NULL);
    return -1;
=======
    if (objc == 1) {
	/*
	 * TIP #280. Make actual argument location available to eval'd script
	 */

	TclArgumentGet(interp, objv[0], &invoker, &word);
	objPtr = objv[0];

    } else {
	/*
	 * More than one argument: concatenate them together with spaces
	 * between, then evaluate the result. Tcl_EvalObjEx will delete the
	 * object when it decrements its refcount after eval'ing it.
	 */

	objPtr = Tcl_ConcatObj(objc, objv);
    }

    TclNRAddCallback(interp, Uplevel_Callback, savedVarFramePtr, NULL, NULL,
	    NULL);
    return TclNREvalObjEx(interp, objPtr, 0, invoker, word);
>>>>>>> upstream/master
=======
Proc *
TclFindProc(
    Interp *iPtr,		/* Interpreter in which to look. */
    const char *procName)	/* Name of desired procedure. */
{
    Tcl_Command cmd;
    Command *cmdPtr;

    cmd = Tcl_FindCommand((Tcl_Interp *) iPtr, procName, NULL, /*flags*/ 0);
    if (cmd == (Tcl_Command) NULL) {
	return NULL;
    }
    cmdPtr = (Command *) cmd;

    return TclIsProc(cmdPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * TclIsProc --
 *
 *	Tells whether a command is a Tcl procedure or not.
 *
 * Results:
 *	If the given command is actually a Tcl procedure, the return value is
 *	the address of the record describing the procedure. Otherwise the
 *	return value is 0.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

Proc *
TclIsProc(
    Command *cmdPtr)		/* Command to test. */
{
    Tcl_Command origCmd = TclGetOriginalCommand((Tcl_Command) cmdPtr);

    if (origCmd != NULL) {
	cmdPtr = (Command *) origCmd;
    }
    if (cmdPtr->deleteProc == TclProcDeleteProc) {
	return cmdPtr->objClientData;
    }
    return NULL;
}

static int
ProcWrongNumArgs(
    Tcl_Interp *interp,
    int skip)
{
    CallFrame *framePtr = ((Interp *)interp)->varFramePtr;
    Proc *procPtr = framePtr->procPtr;
    int localCt = procPtr->numCompiledLocals, numArgs, i;
    Tcl_Obj **desiredObjs;
    const char *final = NULL;

    /*
     * Build up desired argument list for Tcl_WrongNumArgs
     */

    numArgs = framePtr->procPtr->numArgs;
    desiredObjs = TclStackAlloc(interp,
	    (int) sizeof(Tcl_Obj *) * (numArgs+1));

    if (framePtr->isProcCallFrame & FRAME_IS_LAMBDA) {
	desiredObjs[0] = Tcl_NewStringObj("lambdaExpr", -1);
    } else {
	desiredObjs[0] = framePtr->objv[skip-1];
    }
    Tcl_IncrRefCount(desiredObjs[0]);

    if (localCt > 0) {
<<<<<<< HEAD
	register Var *defPtr = (Var *) (&framePtr->localCachePtr->varName0 + localCt);
=======
	Var *defPtr = (Var *) (&framePtr->localCachePtr->varName0 + localCt);
>>>>>>> upstream/master

	for (i=1 ; i<=numArgs ; i++, defPtr++) {
	    Tcl_Obj *argObj;
	    Tcl_Obj *namePtr = localName(framePtr, i-1);

	    if (defPtr->value.objPtr != NULL) {
		TclNewObj(argObj);
		Tcl_AppendStringsToObj(argObj, "?", TclGetString(namePtr), "?", NULL);
	    } else if (defPtr->flags & VAR_IS_ARGS) {
		numArgs--;
		final = "?arg ...?";
		break;
	    } else {
		argObj = namePtr;
		Tcl_IncrRefCount(namePtr);
	    }
	    desiredObjs[i] = argObj;
	}
    }

    Tcl_ResetResult(interp);
    Tcl_WrongNumArgs(interp, numArgs+1, desiredObjs, final);

    for (i=0 ; i<=numArgs ; i++) {
	Tcl_DecrRefCount(desiredObjs[i]);
    }
    TclStackFree(interp, desiredObjs);
    return TCL_ERROR;
>>>>>>> upstream/master
}

/*
 *----------------------------------------------------------------------
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * TclObjGetFrame --
 *
<<<<<<< HEAD
 *	Given a description of a procedure frame, such as the first argument
 *	to an "uplevel" or "upvar" command, locate the call frame for the
 *	appropriate level of procedure.
 *
 * Results:
 *	The return value is -1 if an error occurred in finding the frame (in
 *	this case an error message is left in the interp's result). 1 is
 *	returned if objPtr was either an int or an int preceded by "#" and
 *	it specified a valid frame. 0 is returned if objPtr isn't one of the
 *	two things above (in this case, the lookup acts as if objPtr were
 *	"1"). The variable pointed to by framePtrPtr is filled in with the
 *	address of the desired frame (unless an error occurs, in which case it
 *	isn't modified).
=======
 *	Given the name of a procedure, return a pointer to the record
 *	describing the procedure. The procedure will be looked up using the
 *	usual rules: first in the current namespace and then in the global
 *	namespace.
 *
 * Results:
 *	NULL is returned if the name doesn't correspond to any procedure.
 *	Otherwise, the return value is a pointer to the procedure's record. If
 *	the name is found but refers to an imported command that points to a
 *	"real" procedure defined in another namespace, a pointer to that
 *	"real" procedure's structure is returned.
>>>>>>> upstream/master
 *
 * Side effects:
 *	None.
=======
 * TclInitCompiledLocals --
=======
 * InitResolvedLocals --
>>>>>>> upstream/master
 *
 *	This routine is invoked in order to initialize the compiled locals
 *	table for a new call frame.
 *
<<<<<<< HEAD
 *	DEPRECATED: functionality has been inlined elsewhere; this function
 *	remains to insure binary compatibility with Itcl.
 *
=======
>>>>>>> upstream/master
 * Results:
 *	None.
 *
 * Side effects:
 *	May invoke various name resolvers in order to determine which
 *	variables are being referenced at runtime.
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
 *
 *----------------------------------------------------------------------
 */

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
int
TclObjGetFrame(
    Tcl_Interp *interp,		/* Interpreter in which to find frame. */
    Tcl_Obj *objPtr,		/* Object describing frame. */
    CallFrame **framePtrPtr)	/* Store pointer to frame here (or NULL if
				 * global frame indicated). */
{
    register Interp *iPtr = (Interp *) interp;
    int curLevel, level, result;
    const char *name = NULL;

<<<<<<< HEAD
    /*
     * Parse object to figure out which level number to go to.
     */
=======
    bodyPtr = framePtr->procPtr->bodyPtr;
    ByteCodeGetIntRep(bodyPtr, &tclByteCodeType, codePtr);
    if (codePtr == NULL) {
	Tcl_Panic("body object for proc attached to frame is not a byte code type");
    }
>>>>>>> upstream/master

    result = 0;
    curLevel = iPtr->varFramePtr->level;
=======
static void
InitResolvedLocals(
    Tcl_Interp *interp,		/* Current interpreter. */
    ByteCode *codePtr,
    Var *varPtr,
    Namespace *nsPtr)		/* Pointer to current namespace. */
{
    Interp *iPtr = (Interp *) interp;
    int haveResolvers = (nsPtr->compiledVarResProc || iPtr->resolverPtr);
    CompiledLocal *firstLocalPtr, *localPtr;
    int varNum;
    Tcl_ResolvedVarInfo *resVarInfo;
>>>>>>> upstream/master

    /*
     * Check for integer first, since that has potential to spare us
     * a generation of a stringrep.
     */

    if (objPtr == NULL) {
	/* Do nothing */
    } else if (TCL_OK == Tcl_GetIntFromObj(NULL, objPtr, &level)
	    && (level >= 0)) {
	level = curLevel - level;
	result = 1;
    } else if (objPtr->typePtr == &levelReferenceType) {
	level = (int) objPtr->internalRep.longValue;
	result = 1;
    } else {
	name = TclGetString(objPtr);
	if (name[0] == '#') {
	    if (TCL_OK == Tcl_GetInt(NULL, name+1, &level) && level >= 0) {
		TclFreeIntRep(objPtr);
		objPtr->typePtr = &levelReferenceType;
		objPtr->internalRep.longValue = level;
		result = 1;
	    } else {
		result = -1;
	    }
	} else if (isdigit(UCHAR(name[0]))) { /* INTL: digit */
	    /*
	     * If this were an integer, we'd have succeeded already.
	     * Docs say we have to treat this as a 'bad level'  error.
	     */
	    result = -1;
	}
=======
Proc *
TclFindProc(
    Interp *iPtr,		/* Interpreter in which to look. */
    const char *procName)	/* Name of desired procedure. */
{
    Tcl_Command cmd;
    Command *cmdPtr;

    cmd = Tcl_FindCommand((Tcl_Interp *) iPtr, procName, NULL, /*flags*/ 0);
    if (cmd == (Tcl_Command) NULL) {
	return NULL;
>>>>>>> upstream/master
    }

<<<<<<< HEAD
    if (result == 0) {
	level = curLevel - 1;
	name = "1";
    }
    if (result != -1) {
	if (level >= 0) {
	    CallFrame *framePtr;
	    for (framePtr = iPtr->varFramePtr; framePtr != NULL;
		    framePtr = framePtr->callerVarPtr) {
		if (framePtr->level == level) {
		    *framePtrPtr = framePtr;
		    return result;
		}
	    }
	}
	if (name == NULL) {
	    name = TclGetString(objPtr);
	}
    }

>>>>>>> upstream/master
    Tcl_SetObjResult(interp, Tcl_ObjPrintf("bad level \"%s\"", name));
    Tcl_SetErrorCode(interp, "TCL", "VALUE", "STACKLEVEL", NULL);
    return -1;
=======
    return TclIsProc(cmdPtr);
>>>>>>> upstream/master
=======
=======
>>>>>>> upstream/master
void
TclInitCompiledLocals(
    Tcl_Interp *interp,		/* Current interpreter. */
    CallFrame *framePtr,	/* Call frame to initialize. */
    Namespace *nsPtr)		/* Pointer to current namespace. */
{
    Var *varPtr = framePtr->compiledLocals;
    Tcl_Obj *bodyPtr;
    ByteCode *codePtr;

    bodyPtr = framePtr->procPtr->bodyPtr;
    ByteCodeGetIntRep(bodyPtr, &tclByteCodeType, codePtr);
    if (codePtr == NULL) {
	Tcl_Panic("body object for proc attached to frame is not a byte code type");
    }
<<<<<<< HEAD
    codePtr = bodyPtr->internalRep.twoPtrValue.ptr1;
=======
>>>>>>> upstream/master

    if (framePtr->numCompiledLocals) {
	if (!codePtr->localCachePtr) {
	    InitLocalCache(framePtr->procPtr) ;
	}
	framePtr->localCachePtr = codePtr->localCachePtr;
	framePtr->localCachePtr->refCount++;
    }

    InitResolvedLocals(interp, codePtr, varPtr, nsPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * InitResolvedLocals --
 *
 *	This routine is invoked in order to initialize the compiled locals
 *	table for a new call frame.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	May invoke various name resolvers in order to determine which
 *	variables are being referenced at runtime.
 *
 *----------------------------------------------------------------------
 */

static void
InitResolvedLocals(
    Tcl_Interp *interp,		/* Current interpreter. */
    ByteCode *codePtr,
    Var *varPtr,
    Namespace *nsPtr)		/* Pointer to current namespace. */
{
    Interp *iPtr = (Interp *) interp;
    int haveResolvers = (nsPtr->compiledVarResProc || iPtr->resolverPtr);
    CompiledLocal *firstLocalPtr, *localPtr;
    int varNum;
    Tcl_ResolvedVarInfo *resVarInfo;

    /*
     * Find the localPtr corresponding to varPtr
     */

    varNum = varPtr - iPtr->framePtr->compiledLocals;
    localPtr = iPtr->framePtr->procPtr->firstLocalPtr;
    while (varNum--) {
	localPtr = localPtr->nextPtr;
    }

    if (!(haveResolvers && (codePtr->flags & TCL_BYTECODE_RESOLVE_VARS))) {
	goto doInitResolvedLocals;
    }

    /*
     * This is the first run after a recompile, or else the resolver epoch
     * has changed: update the resolver cache.
     */

    firstLocalPtr = localPtr;
    for (; localPtr != NULL; localPtr = localPtr->nextPtr) {
	if (localPtr->resolveInfo) {
	    if (localPtr->resolveInfo->deleteProc) {
		localPtr->resolveInfo->deleteProc(localPtr->resolveInfo);
	    } else {
		Tcl_Free(localPtr->resolveInfo);
	    }
	    localPtr->resolveInfo = NULL;
	}
	localPtr->flags &= ~VAR_RESOLVED;

	if (haveResolvers &&
		!(localPtr->flags & (VAR_ARGUMENT|VAR_TEMPORARY))) {
	    ResolverScheme *resPtr = iPtr->resolverPtr;
	    Tcl_ResolvedVarInfo *vinfo;
	    int result;

	    if (nsPtr->compiledVarResProc) {
		result = nsPtr->compiledVarResProc(nsPtr->interp,
			localPtr->name, localPtr->nameLength,
			(Tcl_Namespace *) nsPtr, &vinfo);
	    } else {
		result = TCL_CONTINUE;
	    }

	    while ((result == TCL_CONTINUE) && resPtr) {
		if (resPtr->compiledVarResProc) {
		    result = resPtr->compiledVarResProc(nsPtr->interp,
			    localPtr->name, localPtr->nameLength,
			    (Tcl_Namespace *) nsPtr, &vinfo);
		}
		resPtr = resPtr->nextPtr;
	    }
	    if (result == TCL_OK) {
		localPtr->resolveInfo = vinfo;
		localPtr->flags |= VAR_RESOLVED;
	    }
	}
    }
    localPtr = firstLocalPtr;
    codePtr->flags &= ~TCL_BYTECODE_RESOLVE_VARS;

    /*
     * Initialize the array of local variables stored in the call frame. Some
     * variables may have special resolution rules. In that case, we call
     * their "resolver" procs to get our hands on the variable, and we make
     * the compiled local a link to the real variable.
     */

  doInitResolvedLocals:
    for (; localPtr != NULL; varPtr++, localPtr = localPtr->nextPtr) {
	varPtr->flags = 0;
	varPtr->value.objPtr = NULL;

	/*
	 * Now invoke the resolvers to determine the exact variables that
	 * should be used.
	 */

	resVarInfo = localPtr->resolveInfo;
	if (resVarInfo && resVarInfo->fetchProc) {
	    Var *resolvedVarPtr = (Var *)
		    resVarInfo->fetchProc(interp, resVarInfo);

	    if (resolvedVarPtr) {
		if (TclIsVarInHash(resolvedVarPtr)) {
		    VarHashRefCount(resolvedVarPtr)++;
		}
		varPtr->flags = VAR_LINK;
		varPtr->value.linkPtr = resolvedVarPtr;
	    }
	}
    }
}

void
TclFreeLocalCache(
    Tcl_Interp *interp,
    LocalCache *localCachePtr)
{
    int i;
    Tcl_Obj **namePtrPtr = &localCachePtr->varName0;

    for (i = 0; i < localCachePtr->numVars; i++, namePtrPtr++) {
<<<<<<< HEAD
	register Tcl_Obj *objPtr = *namePtrPtr;
=======
	Tcl_Obj *objPtr = *namePtrPtr;
>>>>>>> upstream/master

	if (objPtr) {
	    /* TclReleaseLiteral calls Tcl_DecrRefCount for us */
	    TclReleaseLiteral(interp, objPtr);
	}
    }
    Tcl_Free(localCachePtr);
}

static void
InitLocalCache(
    Proc *procPtr)
{
    Interp *iPtr = procPtr->iPtr;
    ByteCode *codePtr;
    int localCt = procPtr->numCompiledLocals;
    int numArgs = procPtr->numArgs, i = 0;

    Tcl_Obj **namePtr;
    Var *varPtr;
    LocalCache *localCachePtr;
    CompiledLocal *localPtr;
    int isNew;

    ByteCodeGetIntRep(procPtr->bodyPtr, &tclByteCodeType, codePtr);

    ByteCodeGetIntRep(procPtr->bodyPtr, &tclByteCodeType, codePtr);

    /*
     * Cache the names and initial values of local variables; store the
     * cache in both the framePtr for this execution and in the codePtr
     * for future calls.
     */

    localCachePtr = (LocalCache *)Tcl_Alloc(offsetof(LocalCache, varName0)
	    + localCt * sizeof(Tcl_Obj *)
	    + numArgs * sizeof(Var));

    namePtr = &localCachePtr->varName0;
    varPtr = (Var *) (namePtr + localCt);
    localPtr = procPtr->firstLocalPtr;
    while (localPtr) {
	if (TclIsVarTemporary(localPtr)) {
	    *namePtr = NULL;
	} else {
	    *namePtr = TclCreateLiteral(iPtr, localPtr->name,
		    localPtr->nameLength, /* hash */ -1,
<<<<<<< HEAD
		    &new, /* nsPtr */ NULL, 0, NULL);
=======
		    &isNew, /* nsPtr */ NULL, 0, NULL);
>>>>>>> upstream/master
	    Tcl_IncrRefCount(*namePtr);
	}

	if (i < numArgs) {
	    varPtr->flags = (localPtr->flags & VAR_IS_ARGS);
	    varPtr->value.objPtr = localPtr->defValuePtr;
	    varPtr++;
	    i++;
	}
	namePtr++;
	localPtr = localPtr->nextPtr;
    }
    codePtr->localCachePtr = localCachePtr;
    localCachePtr->refCount = 1;
    localCachePtr->numVars = localCt;
>>>>>>> upstream/master
}

/*
 *----------------------------------------------------------------------
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Tcl_UplevelObjCmd --
 *
 *	This object function is invoked to process the "uplevel" Tcl command.
 *	See the user documentation for details on what it does.
=======
=======
>>>>>>> upstream/master
 * InitArgsAndLocals --
 *
 *	This routine is invoked in order to initialize the arguments and other
 *	compiled locals table for a new call frame.
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
<<<<<<< HEAD
<<<<<<< HEAD
 *	See the user documentation.
=======
=======
>>>>>>> upstream/master
 *	Allocates memory on the stack for the compiled local variables, the
 *	caller is responsible for freeing them. Initialises all variables. May
 *	invoke various name resolvers in order to determine which variables
 *	are being referenced at runtime.
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
 *
 *----------------------------------------------------------------------
 */

static int
<<<<<<< HEAD
Uplevel_Callback(
    ClientData data[],
    Tcl_Interp *interp,
    int result)
{
    CallFrame *savedVarFramePtr = data[0];

    if (result == TCL_ERROR) {
	Tcl_AppendObjToErrorInfo(interp, Tcl_ObjPrintf(
		"\n    (\"uplevel\" body line %d)", Tcl_GetErrorLine(interp)));
    }

    /*
     * Restore the variable frame, and return.
     */

    ((Interp *)interp)->varFramePtr = savedVarFramePtr;
    return result;
}

	/* ARGSUSED */
int
Tcl_UplevelObjCmd(
    ClientData dummy,		/* Not used. */
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const objv[])	/* Argument objects. */
{
    return Tcl_NRCallObjProc(interp, TclNRUplevelObjCmd, dummy, objc, objv);
}

int
TclNRUplevelObjCmd(
    ClientData dummy,		/* Not used. */
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const objv[])	/* Argument objects. */
{

    register Interp *iPtr = (Interp *) interp;
    CmdFrame *invoker = NULL;
    int word = 0;
    int result;
    CallFrame *savedVarFramePtr, *framePtr;
    Tcl_Obj *objPtr;

    if (objc < 2) {
    uplevelSyntax:
	Tcl_WrongNumArgs(interp, 1, objv, "?level? command ?arg ...?");
	return TCL_ERROR;
    }

    /*
     * Find the level to use for executing the command.
     */

    result = TclObjGetFrame(interp, objv[1], &framePtr);
    if (result == -1) {
	return TCL_ERROR;
    }
    objc -= result + 1;
    if (objc == 0) {
	goto uplevelSyntax;
    }
    objv += result + 1;

    /*
     * Modify the interpreter state to execute in the given frame.
     */

    savedVarFramePtr = iPtr->varFramePtr;
    iPtr->varFramePtr = framePtr;

    /*
     * Execute the residual arguments as a command.
     */

    if (objc == 1) {
	/*
	 * TIP #280. Make actual argument location available to eval'd script
	 */

	TclArgumentGet(interp, objv[0], &invoker, &word);
	objPtr = objv[0];

    } else {
	/*
	 * More than one argument: concatenate them together with spaces
	 * between, then evaluate the result. Tcl_EvalObjEx will delete the
	 * object when it decrements its refcount after eval'ing it.
	 */

	objPtr = Tcl_ConcatObj(objc, objv);
    }

    TclNRAddCallback(interp, Uplevel_Callback, savedVarFramePtr, NULL, NULL,
	    NULL);
    return TclNREvalObjEx(interp, objPtr, 0, invoker, word);
=======
InitArgsAndLocals(
    Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    int skip)			/* Number of initial arguments to be skipped,
				 * i.e., words in the "command name". */
{
    CallFrame *framePtr = ((Interp *)interp)->varFramePtr;
<<<<<<< HEAD
    register Proc *procPtr = framePtr->procPtr;
    ByteCode *codePtr;
    register Var *varPtr, *defPtr;
=======
    Proc *procPtr = framePtr->procPtr;
    ByteCode *codePtr;
    Var *varPtr, *defPtr;
>>>>>>> upstream/master
    int localCt = procPtr->numCompiledLocals, numArgs, argCt, i, imax;
    Tcl_Obj *const *argObjs;

    ByteCodeGetIntRep(procPtr->bodyPtr, &tclByteCodeType, codePtr);

    /*
     * Make sure that the local cache of variable names and initial values has
     * been initialised properly .
     */

    if (localCt) {
	if (!codePtr->localCachePtr) {
	    InitLocalCache(procPtr) ;
	}
	framePtr->localCachePtr = codePtr->localCachePtr;
	framePtr->localCachePtr->refCount++;
	defPtr = (Var *) (&framePtr->localCachePtr->varName0 + localCt);
    } else {
	defPtr = NULL;
    }

    /*
     * Create the "compiledLocals" array. Make sure it is large enough to hold
     * all the procedure's compiled local variables, including its formal
     * parameters.
     */

    varPtr = (Var *)TclStackAlloc(interp, localCt * sizeof(Var));
    framePtr->compiledLocals = varPtr;
    framePtr->numCompiledLocals = localCt;

    /*
     * Match and assign the call's actual parameters to the procedure's formal
     * arguments. The formal arguments are described by the first numArgs
     * entries in both the Proc structure's local variable list and the call
     * frame's local variable array.
     */

    numArgs = procPtr->numArgs;
    argCt = framePtr->objc - skip;	/* Set it to the number of args to the
					 * procedure. */
    argObjs = framePtr->objv + skip;
    if (numArgs == 0) {
	if (argCt) {
	    goto incorrectArgs;
	} else {
	    goto correctArgs;
	}
    }
    imax = ((argCt < numArgs-1) ? argCt : numArgs-1);
    for (i = 0; i < imax; i++, varPtr++, defPtr ? defPtr++ : defPtr) {
	/*
	 * "Normal" arguments; last formal is special, depends on it being
	 * 'args'.
	 */

	Tcl_Obj *objPtr = argObjs[i];

	varPtr->flags = 0;
	varPtr->value.objPtr = objPtr;
	Tcl_IncrRefCount(objPtr);	/* Local var is a reference. */
    }
    for (; i < numArgs-1; i++, varPtr++, defPtr ? defPtr++ : defPtr) {
	/*
	 * This loop is entered if argCt < (numArgs-1). Set default values;
	 * last formal is special.
	 */

	Tcl_Obj *objPtr = defPtr ? defPtr->value.objPtr : NULL;

	if (!objPtr) {
	    goto incorrectArgs;
	}
	varPtr->flags = 0;
	varPtr->value.objPtr = objPtr;
	Tcl_IncrRefCount(objPtr);	/* Local var reference. */
    }

    /*
     * When we get here, the last formal argument remains to be defined:
     * defPtr and varPtr point to the last argument to be initialized.
     */

    varPtr->flags = 0;
    if (defPtr && defPtr->flags & VAR_IS_ARGS) {
	Tcl_Obj *listPtr = Tcl_NewListObj(argCt-i, argObjs+i);

	varPtr->value.objPtr = listPtr;
	Tcl_IncrRefCount(listPtr);	/* Local var is a reference. */
    } else if (argCt == numArgs) {
	Tcl_Obj *objPtr = argObjs[i];

	varPtr->value.objPtr = objPtr;
	Tcl_IncrRefCount(objPtr);	/* Local var is a reference. */
    } else if ((argCt < numArgs) && defPtr && defPtr->value.objPtr) {
	Tcl_Obj *objPtr = defPtr->value.objPtr;

	varPtr->value.objPtr = objPtr;
	Tcl_IncrRefCount(objPtr);	/* Local var is a reference. */
    } else {
	goto incorrectArgs;
    }
    varPtr++;

    /*
     * Initialise and resolve the remaining compiledLocals. In the absence of
     * resolvers, they are undefined local vars: (flags=0, value=NULL).
     */

  correctArgs:
    if (numArgs < localCt) {
	if (!framePtr->nsPtr->compiledVarResProc
		&& !((Interp *)interp)->resolverPtr) {
	    memset(varPtr, 0, (localCt - numArgs)*sizeof(Var));
	} else {
	    InitResolvedLocals(interp, codePtr, varPtr, framePtr->nsPtr);
	}
    }

    return TCL_OK;

    /*
     * Initialise all compiled locals to avoid problems at DeleteLocalVars.
     */

  incorrectArgs:
    if ((skip != 1) &&
	    TclInitRewriteEnsemble(interp, skip-1, 0, framePtr->objv)) {
	TclNRAddCallback(interp, TclClearRootEnsemble, NULL, NULL, NULL, NULL);
    }
    memset(varPtr, 0,
	    ((framePtr->compiledLocals + localCt)-varPtr) * sizeof(Var));
    return ProcWrongNumArgs(interp, skip);
}

/*
 *----------------------------------------------------------------------
 *
 * TclPushProcCallFrame --
 *
 *	Compiles a proc body if necessary, then pushes a CallFrame suitable
 *	for executing it.
 *
 * Results:
 *	A standard Tcl object result value.
 *
 * Side effects:
 *	The proc's body may be recompiled. A CallFrame is pushed, it will have
 *	to be popped by the caller.
 *
 *----------------------------------------------------------------------
 */

int
TclPushProcCallFrame(
    ClientData clientData,	/* Record describing procedure to be
				 * interpreted. */
    Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    int objc,			/* Count of number of arguments to this
				 * procedure. */
    Tcl_Obj *const objv[],	/* Argument value objects. */
    int isLambda)		/* 1 if this is a call by ApplyObjCmd: it
				 * needs special rules for error msg */
{
    Proc *procPtr = (Proc *)clientData;
    Namespace *nsPtr = procPtr->cmdPtr->nsPtr;
    CallFrame *framePtr, **framePtrPtr;
    int result;
    ByteCode *codePtr;

    /*
     * If necessary (i.e. if we haven't got a suitable compilation already
     * cached) compile the procedure's body. The compiler will allocate frame
     * slots for the procedure's non-argument local variables. Note that
     * compiling the body might increase procPtr->numCompiledLocals if new
     * local variables are found while compiling.
     */

    ByteCodeGetIntRep(procPtr->bodyPtr, &tclByteCodeType, codePtr);
    if (codePtr != NULL) {
	Interp *iPtr = (Interp *) interp;

	/*
	 * When we've got bytecode, this is the check for validity. That is,
	 * the bytecode must be for the right interpreter (no cross-leaks!),
	 * the code must be from the current epoch (so subcommand compilation
	 * is up-to-date), the namespace must match (so variable handling
	 * is right) and the resolverEpoch must match (so that new shadowed
	 * commands and/or resolver changes are considered).
	 */

	if (((Interp *) *codePtr->interpHandle != iPtr)
		|| (codePtr->compileEpoch != iPtr->compileEpoch)
		|| (codePtr->nsPtr != nsPtr)
		|| (codePtr->nsEpoch != nsPtr->resolverEpoch)) {
	    goto doCompilation;
	}
    } else {
    doCompilation:
	result = TclProcCompileProc(interp, procPtr, procPtr->bodyPtr, nsPtr,
		(isLambda ? "body of lambda term" : "body of proc"),
		TclGetString(objv[isLambda]));
	if (result != TCL_OK) {
	    return result;
	}
    }

    /*
     * Set up and push a new call frame for the new procedure invocation.
     * This call frame will execute in the proc's namespace, which might be
     * different than the current namespace. The proc's namespace is that of
     * its command, which can change if the command is renamed from one
     * namespace to another.
     */

    framePtrPtr = &framePtr;
    (void) TclPushStackFrame(interp, (Tcl_CallFrame **) framePtrPtr,
	    (Tcl_Namespace *) nsPtr,
	    (isLambda? (FRAME_IS_PROC|FRAME_IS_LAMBDA) : FRAME_IS_PROC));

    framePtr->objc = objc;
    framePtr->objv = objv;
    framePtr->procPtr = procPtr;

    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * TclObjInterpProc --
 *
 *	When a Tcl procedure gets invoked during bytecode evaluation, this
 *	object-based routine gets invoked to interpret the procedure.
 *
 * Results:
 *	A standard Tcl object result value.
 *
 * Side effects:
 *	Depends on the commands in the procedure.
 *
 *----------------------------------------------------------------------
 */

int
TclObjInterpProc(
    ClientData clientData,	/* Record describing procedure to be
				 * interpreted. */
    Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    int objc,			/* Count of number of arguments to this
				 * procedure. */
    Tcl_Obj *const objv[])	/* Argument value objects. */
{
    /*
     * Not used much in the core; external interface for iTcl
     */

    return Tcl_NRCallObjProc(interp, TclNRInterpProc, clientData, objc, objv);
}

int
TclNRInterpProc(
    ClientData clientData,	/* Record describing procedure to be
				 * interpreted. */
    Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    int objc,			/* Count of number of arguments to this
				 * procedure. */
    Tcl_Obj *const objv[])	/* Argument value objects. */
{
    int result = TclPushProcCallFrame(clientData, interp, objc, objv,
	    /*isLambda*/ 0);

    if (result != TCL_OK) {
	return TCL_ERROR;
    }
    return TclNRInterpProcCore(interp, objv[0], 1, &MakeProcError);
}

/*
 *----------------------------------------------------------------------
 *
 * TclNRInterpProcCore --
 *
 *	When a Tcl procedure, lambda term or anything else that works like a
 *	procedure gets invoked during bytecode evaluation, this object-based
 *	routine gets invoked to interpret the body.
 *
 * Results:
 *	A standard Tcl object result value.
 *
 * Side effects:
 *	Nearly anything; depends on the commands in the procedure body.
 *
 *----------------------------------------------------------------------
 */

int
TclNRInterpProcCore(
    Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    Tcl_Obj *procNameObj,	/* Procedure name for error reporting. */
    int skip,			/* Number of initial arguments to be skipped,
				 * i.e., words in the "command name". */
    ProcErrorProc *errorProc)	/* How to convert results from the script into
				 * results of the overall procedure. */
{
    Interp *iPtr = (Interp *) interp;
    Proc *procPtr = iPtr->varFramePtr->procPtr;
    int result;
    CallFrame *freePtr;
    ByteCode *codePtr;

    result = InitArgsAndLocals(interp, skip);
    if (result != TCL_OK) {
	freePtr = iPtr->framePtr;
	Tcl_PopCallFrame(interp);	/* Pop but do not free. */
	TclStackFree(interp, freePtr->compiledLocals);
					/* Free compiledLocals. */
	TclStackFree(interp, freePtr);	/* Free CallFrame. */
	return TCL_ERROR;
    }

#if defined(TCL_COMPILE_DEBUG)
    if (tclTraceExec >= 1) {
	CallFrame *framePtr = iPtr->varFramePtr;
	int i;

	if (framePtr->isProcCallFrame & FRAME_IS_LAMBDA) {
	    fprintf(stdout, "Calling lambda ");
	} else {
	    fprintf(stdout, "Calling proc ");
	}
	for (i = 0; i < framePtr->objc; i++) {
	    TclPrintObject(stdout, framePtr->objv[i], 15);
	    fprintf(stdout, " ");
	}
	fprintf(stdout, "\n");
	fflush(stdout);
    }
#endif /*TCL_COMPILE_DEBUG*/

#ifdef USE_DTRACE
    if (TCL_DTRACE_PROC_ARGS_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;
	const char *a[10];
	int i;

	for (i = 0 ; i < 10 ; i++) {
	    a[i] = (l < iPtr->varFramePtr->objc ?
		    TclGetString(iPtr->varFramePtr->objv[l]) : NULL);
	    l++;
	}
	TCL_DTRACE_PROC_ARGS(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
		a[8], a[9]);
    }
    if (TCL_DTRACE_PROC_INFO_ENABLED() && iPtr->cmdFramePtr) {
	Tcl_Obj *info = TclInfoFrame(interp, iPtr->cmdFramePtr);
	const char *a[6]; int i[2];

	TclDTraceInfo(info, a, i);
	TCL_DTRACE_PROC_INFO(a[0], a[1], a[2], a[3], i[0], i[1], a[4], a[5]);
	TclDecrRefCount(info);
    }
    if (TCL_DTRACE_PROC_ENTRY_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;

	TCL_DTRACE_PROC_ENTRY(l < iPtr->varFramePtr->objc ?
		TclGetString(iPtr->varFramePtr->objv[l]) : NULL,
		iPtr->varFramePtr->objc - l - 1,
		(Tcl_Obj **)(iPtr->varFramePtr->objv + l + 1));
    }
    if (TCL_DTRACE_PROC_ENTRY_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;

	TCL_DTRACE_PROC_ENTRY(l < iPtr->varFramePtr->objc ?
		TclGetString(iPtr->varFramePtr->objv[l]) : NULL,
		iPtr->varFramePtr->objc - l - 1,
		(Tcl_Obj **)(iPtr->varFramePtr->objv + l + 1));
    }
#endif /* USE_DTRACE */

    /*
     * Invoke the commands in the procedure's body.
     */

    procPtr->refCount++;
    ByteCodeGetIntRep(procPtr->bodyPtr, &tclByteCodeType, codePtr);

    TclNRAddCallback(interp, InterpProcNR2, procNameObj, errorProc,
	    NULL, NULL);
    return TclNRExecuteByteCode(interp, codePtr);
}

static int
InterpProcNR2(
    ClientData data[],
    Tcl_Interp *interp,
    int result)
{
    Interp *iPtr = (Interp *) interp;
    Proc *procPtr = iPtr->varFramePtr->procPtr;
    CallFrame *freePtr;
    Tcl_Obj *procNameObj = (Tcl_Obj *)data[0];
    ProcErrorProc *errorProc = (ProcErrorProc *)data[1];

    if (TCL_DTRACE_PROC_RETURN_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;

	TCL_DTRACE_PROC_RETURN(l < iPtr->varFramePtr->objc ?
		TclGetString(iPtr->varFramePtr->objv[l]) : NULL, result);
    }
    if (procPtr->refCount-- <= 1) {
	TclProcCleanupProc(procPtr);
    }

    /*
     * Free the stack-allocated compiled locals and CallFrame. It is important
     * to pop the call frame without freeing it first: the compiledLocals
     * cannot be freed before the frame is popped, as the local variables must
     * be deleted. But the compiledLocals must be freed first, as they were
     * allocated later on the stack.
<<<<<<< HEAD
     */

    if (result != TCL_OK) {
	goto process;
    }
<<<<<<< HEAD

    done:
    if (TCL_DTRACE_PROC_RESULT_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;
	Tcl_Obj *r = Tcl_GetObjResult(interp);

=======

    done:
    if (TCL_DTRACE_PROC_RESULT_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;
	Tcl_Obj *r = Tcl_GetObjResult(interp);

>>>>>>> upstream/master
	TCL_DTRACE_PROC_RESULT(l < iPtr->varFramePtr->objc ?
		TclGetString(iPtr->varFramePtr->objv[l]) : NULL, result,
		TclGetString(r), r);
    }

    freePtr = iPtr->framePtr;
    Tcl_PopCallFrame(interp);		/* Pop but do not free. */
    TclStackFree(interp, freePtr->compiledLocals);
					/* Free compiledLocals. */
    TclStackFree(interp, freePtr);	/* Free CallFrame. */
    return result;

    /*
     * Process any non-TCL_OK result code.
     */

=======
     */

    if (result != TCL_OK) {
	goto process;
    }

    done:
    if (TCL_DTRACE_PROC_RESULT_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;
	Tcl_Obj *r = Tcl_GetObjResult(interp);

	TCL_DTRACE_PROC_RESULT(l < iPtr->varFramePtr->objc ?
		TclGetString(iPtr->varFramePtr->objv[l]) : NULL, result,
		TclGetString(r), r);
    }

    freePtr = iPtr->framePtr;
    Tcl_PopCallFrame(interp);		/* Pop but do not free. */
    TclStackFree(interp, freePtr->compiledLocals);
					/* Free compiledLocals. */
    TclStackFree(interp, freePtr);	/* Free CallFrame. */
    return result;

    /*
     * Process any non-TCL_OK result code.
     */

>>>>>>> upstream/master
    process:
    switch (result) {
    case TCL_RETURN:
	/*
	 * If it is a 'return', do the TIP#90 processing now.
	 */

	result = TclUpdateReturnInfo((Interp *) interp);
	break;

    case TCL_CONTINUE:
    case TCL_BREAK:
	/*
	 * It's an error to get to this point from a 'break' or 'continue', so
	 * transform to an error now.
	 */

	Tcl_SetObjResult(interp, Tcl_ObjPrintf(
		"invoked \"%s\" outside of a loop",
		((result == TCL_BREAK) ? "break" : "continue")));
	Tcl_SetErrorCode(interp, "TCL", "RESULT", "UNEXPECTED", NULL);
	result = TCL_ERROR;

	/* FALLTHRU */

    case TCL_ERROR:
	/*
	 * Now it _must_ be an error, so we need to log it as such. This means
	 * filling out the error trace. Luckily, we just hand this off to the
	 * function handed to us as an argument.
	 */

	errorProc(interp, procNameObj);
    }
    goto done;
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
}

/*
 *----------------------------------------------------------------------
 *
 * TclFindProc --
 *
<<<<<<< HEAD
 *	Given the name of a procedure, return a pointer to the record
 *	describing the procedure. The procedure will be looked up using the
 *	usual rules: first in the current namespace and then in the global
 *	namespace.
=======
 *	Called just before a procedure is executed to compile the body to byte
 *	codes. If the type of the body is not "byte code" or if the compile
 *	conditions have changed (namespace context, epoch counters, etc.) then
 *	the body is recompiled. Otherwise, this function does nothing.
>>>>>>> upstream/master
 *
 * Results:
 *	NULL is returned if the name doesn't correspond to any procedure.
 *	Otherwise, the return value is a pointer to the procedure's record. If
 *	the name is found but refers to an imported command that points to a
 *	"real" procedure defined in another namespace, a pointer to that
 *	"real" procedure's structure is returned.
 *
 * Side effects:
<<<<<<< HEAD
 *	None.
 *
 *----------------------------------------------------------------------
 */

<<<<<<< HEAD
Proc *
TclFindProc(
    Interp *iPtr,		/* Interpreter in which to look. */
    const char *procName)	/* Name of desired procedure. */
{
    Tcl_Command cmd;
    Command *cmdPtr;

    cmd = Tcl_FindCommand((Tcl_Interp *) iPtr, procName, NULL, /*flags*/ 0);
    if (cmd == (Tcl_Command) NULL) {
	return NULL;
=======
 *	May change the internal representation of the body object to compiled
 *	code.
 *
 *----------------------------------------------------------------------
 */

=======
>>>>>>> upstream/master
int
TclProcCompileProc(
    Tcl_Interp *interp,		/* Interpreter containing procedure. */
    Proc *procPtr,		/* Data associated with procedure. */
    Tcl_Obj *bodyPtr,		/* Body of proc. (Usually procPtr->bodyPtr,
				 * but could be any code fragment compiled in
				 * the context of this procedure.) */
    Namespace *nsPtr,		/* Namespace containing procedure. */
    const char *description,	/* string describing this body of code. */
    const char *procName)	/* Name of this procedure. */
{
    Interp *iPtr = (Interp *) interp;
    Tcl_CallFrame *framePtr;
    ByteCode *codePtr;

    ByteCodeGetIntRep(bodyPtr, &tclByteCodeType, codePtr);

    /*
     * If necessary, compile the procedure's body. The compiler will allocate
     * frame slots for the procedure's non-argument local variables. If the
     * ByteCode already exists, make sure it hasn't been invalidated by
     * someone redefining a core command (this might make the compiled code
     * wrong). Also, if the code was compiled in/for a different interpreter,
     * we recompile it. Note that compiling the body might increase
     * procPtr->numCompiledLocals if new local variables are found while
     * compiling.
     *
     * Precompiled procedure bodies, however, are immutable and therefore they
     * are not recompiled, even if things have changed.
     */

    if (codePtr != NULL) {
	if (((Interp *) *codePtr->interpHandle == iPtr)
		&& (codePtr->compileEpoch == iPtr->compileEpoch)
		&& (codePtr->nsPtr == nsPtr)
		&& (codePtr->nsEpoch == nsPtr->resolverEpoch)) {
	    return TCL_OK;
	}

	if (codePtr->flags & TCL_BYTECODE_PRECOMPILED) {
	    if ((Interp *) *codePtr->interpHandle != iPtr) {
		Tcl_SetObjResult(interp, Tcl_NewStringObj(
			"a precompiled script jumped interps", -1));
		Tcl_SetErrorCode(interp, "TCL", "OPERATION", "PROC",
			"CROSSINTERPBYTECODE", NULL);
		return TCL_ERROR;
	    }
	    codePtr->compileEpoch = iPtr->compileEpoch;
	    codePtr->nsPtr = nsPtr;
	} else {
	    Tcl_StoreIntRep(bodyPtr, &tclByteCodeType, NULL);
	    codePtr = NULL;
	}
    }

    if (codePtr == NULL) {
	Tcl_HashEntry *hePtr;

#ifdef TCL_COMPILE_DEBUG
	if (tclTraceCompile >= 1) {
	    /*
	     * Display a line summarizing the top level command we are about
	     * to compile.
	     */

	    Tcl_Obj *message;

	    TclNewLiteralStringObj(message, "Compiling ");
	    Tcl_IncrRefCount(message);
	    Tcl_AppendStringsToObj(message, description, " \"", NULL);
	    Tcl_AppendLimitedToObj(message, procName, -1, 50, NULL);
	    fprintf(stdout, "%s\"\n", TclGetString(message));
	    Tcl_DecrRefCount(message);
	}
#else
    (void)description;
    (void)procName;
#endif

	/*
	 * Plug the current procPtr into the interpreter and coerce the code
	 * body to byte codes. The interpreter needs to know which proc it's
	 * compiling so that it can access its list of compiled locals.
	 *
	 * TRICKY NOTE: Be careful to push a call frame with the proper
	 *   namespace context, so that the byte codes are compiled in the
	 *   appropriate class context.
	 */

	iPtr->compiledProcPtr = procPtr;

	if (procPtr->numCompiledLocals > procPtr->numArgs) {
	    CompiledLocal *clPtr = procPtr->firstLocalPtr;
	    CompiledLocal *lastPtr = NULL;
	    int i, numArgs = procPtr->numArgs;

	    for (i = 0; i < numArgs; i++) {
		lastPtr = clPtr;
		clPtr = clPtr->nextPtr;
	    }

	    if (lastPtr) {
		lastPtr->nextPtr = NULL;
	    } else {
		procPtr->firstLocalPtr = NULL;
	    }
	    procPtr->lastLocalPtr = lastPtr;
	    while (clPtr) {
		CompiledLocal *toFree = clPtr;

		clPtr = clPtr->nextPtr;
		if (toFree->resolveInfo) {
		    if (toFree->resolveInfo->deleteProc) {
			toFree->resolveInfo->deleteProc(toFree->resolveInfo);
		    } else {
			Tcl_Free(toFree->resolveInfo);
		    }
		}
		Tcl_Free(toFree);
	    }
	    procPtr->numCompiledLocals = procPtr->numArgs;
	}

	(void) TclPushStackFrame(interp, &framePtr, (Tcl_Namespace *) nsPtr,
		/* isProcCallFrame */ 0);

	/*
	 * TIP #280: We get the invoking context from the cmdFrame which
	 * was saved by 'Tcl_ProcObjCmd' (using linePBodyPtr).
	 */

	hePtr = Tcl_FindHashEntry(iPtr->linePBodyPtr, (char *) procPtr);

	/*
	 * Constructed saved frame has body as word 0. See Tcl_ProcObjCmd.
	 */

	iPtr->invokeWord = 0;
	iPtr->invokeCmdFramePtr = hePtr ? (CmdFrame *)Tcl_GetHashValue(hePtr) : NULL;
	TclSetByteCodeFromAny(interp, bodyPtr, NULL, NULL);
	iPtr->invokeCmdFramePtr = NULL;
	TclPopStackFrame(interp);
    } else if (codePtr->nsEpoch != nsPtr->resolverEpoch) {
	/*
	 * The resolver epoch has changed, but we only need to invalidate the
	 * resolver cache.
	 */

	codePtr->nsEpoch = nsPtr->resolverEpoch;
	codePtr->flags |= TCL_BYTECODE_RESOLVE_VARS;
>>>>>>> upstream/master
    }
    cmdPtr = (Command *) cmd;

    return TclIsProc(cmdPtr);
}

/*
 *----------------------------------------------------------------------
 *
<<<<<<< HEAD
 * TclIsProc --
 *
 *	Tells whether a command is a Tcl procedure or not.
 *
 * Results:
 *	If the given command is actually a Tcl procedure, the return value is
 *	the address of the record describing the procedure. Otherwise the
 *	return value is 0.
 *
 * Side effects:
 *	None.
=======
 * MakeProcError --
 *
 *	Function called by TclObjInterpProc to create the stack information
 *	upon an error from a procedure.
 *
 * Results:
 *	The interpreter's error info trace is set to a value that supplements
 *	the error code.
 *
 * Side effects:
 *	none.
>>>>>>> upstream/master
 *
 *----------------------------------------------------------------------
 */

<<<<<<< HEAD
<<<<<<< HEAD
Proc *
TclIsProc(
    Command *cmdPtr)		/* Command to test. */
<<<<<<< HEAD
{
<<<<<<< HEAD
    Tcl_Command origCmd = TclGetOriginalCommand((Tcl_Command) cmdPtr);

    if (origCmd != NULL) {
	cmdPtr = (Command *) origCmd;
    }
    if (cmdPtr->deleteProc == TclProcDeleteProc) {
	return cmdPtr->objClientData;
<<<<<<< HEAD
    }
    return NULL;
=======
    int overflow, limit = 60, nameLen;
=======
static void
MakeProcError(
    Tcl_Interp *interp,		/* The interpreter in which the procedure was
				 * called. */
    Tcl_Obj *procNameObj)	/* Name of the procedure. Used for error
				 * messages and trace information. */
{
    unsigned int overflow, limit = 60;
    size_t nameLen;
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    const char *procName = TclGetStringFromObj(procNameObj, &nameLen);

    overflow = (nameLen > limit);
    Tcl_AppendObjToErrorInfo(interp, Tcl_ObjPrintf(
	    "\n    (procedure \"%.*s%s\" line %d)",
	    (int)(overflow ? limit :nameLen), procName,
	    (overflow ? "..." : ""), Tcl_GetErrorLine(interp)));
>>>>>>> upstream/master
}

static int
ProcWrongNumArgs(
    Tcl_Interp *interp,
    int skip)
{
<<<<<<< HEAD
    CallFrame *framePtr = ((Interp *)interp)->varFramePtr;
    register Proc *procPtr = framePtr->procPtr;
    register Var *defPtr;
    int localCt = procPtr->numCompiledLocals, numArgs, i;
    Tcl_Obj **desiredObjs;
    const char *final = NULL;

<<<<<<< HEAD
    /*
     * Build up desired argument list for Tcl_WrongNumArgs
     */
=======
    Proc *procPtr = (Proc *)clientData;
>>>>>>> upstream/master

<<<<<<< HEAD
    numArgs = framePtr->procPtr->numArgs;
    desiredObjs = TclStackAlloc(interp,
	    (int) sizeof(Tcl_Obj *) * (numArgs+1));
=======
=======
>>>>>>> upstream/master
    if (procPtr->refCount-- <= 1) {
	TclProcCleanupProc(procPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * TclProcCleanupProc --
 *
 *	This function does all the real work of freeing up a Proc structure.
 *	It's called only when the structure's reference count becomes zero.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Memory gets freed.
 *
 *----------------------------------------------------------------------
 */

void
TclProcCleanupProc(
    Proc *procPtr)	/* Procedure to be deleted. */
{
    CompiledLocal *localPtr;
    Tcl_Obj *bodyPtr = procPtr->bodyPtr;
    Tcl_Obj *defPtr;
    Tcl_ResolvedVarInfo *resVarInfo;
    Tcl_HashEntry *hePtr = NULL;
    CmdFrame *cfPtr = NULL;
    Interp *iPtr = procPtr->iPtr;
>>>>>>> upstream/master

    if (framePtr->isProcCallFrame & FRAME_IS_LAMBDA) {
	desiredObjs[0] = Tcl_NewStringObj("lambdaExpr", -1);
    } else {
	((Interp *) interp)->ensembleRewrite.numInsertedObjs -= skip - 1;

#ifdef AVOID_HACKS_FOR_ITCL
	desiredObjs[0] = framePtr->objv[skip-1];
#else
	desiredObjs[0] = Tcl_NewListObj(skip, framePtr->objv);
#endif /* AVOID_HACKS_FOR_ITCL */
    }
    Tcl_IncrRefCount(desiredObjs[0]);

<<<<<<< HEAD
    defPtr = (Var *) (&framePtr->localCachePtr->varName0 + localCt);
    for (i=1 ; i<=numArgs ; i++, defPtr++) {
	Tcl_Obj *argObj;
	Tcl_Obj *namePtr = localName(framePtr, i-1);
=======
	resVarInfo = localPtr->resolveInfo;
	if (resVarInfo) {
	    if (resVarInfo->deleteProc) {
		resVarInfo->deleteProc(resVarInfo);
	    } else {
		Tcl_Free(resVarInfo);
	    }
	}
>>>>>>> upstream/master

	if (defPtr->value.objPtr != NULL) {
	    TclNewObj(argObj);
	    Tcl_AppendStringsToObj(argObj, "?", TclGetString(namePtr), "?", NULL);
	} else if (defPtr->flags & VAR_IS_ARGS) {
	    numArgs--;
	    final = "?arg ...?";
	    break;
	} else {
	    argObj = namePtr;
	    Tcl_IncrRefCount(namePtr);
	}
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	desiredObjs[i] = argObj;
    }

    Tcl_ResetResult(interp);
    Tcl_WrongNumArgs(interp, numArgs+1, desiredObjs, final);

    for (i=0 ; i<=numArgs ; i++) {
	Tcl_DecrRefCount(desiredObjs[i]);
    }
    TclStackFree(interp, desiredObjs);
    return TCL_ERROR;
}

=======
    }
    return NULL;
}

static int
ProcWrongNumArgs(
    Tcl_Interp *interp,
    int skip)
{
    CallFrame *framePtr = ((Interp *)interp)->varFramePtr;
    register Proc *procPtr = framePtr->procPtr;
    register Var *defPtr;
    int localCt = procPtr->numCompiledLocals, numArgs, i;
    Tcl_Obj **desiredObjs;
    const char *final = NULL;

    /*
     * Build up desired argument list for Tcl_WrongNumArgs
     */

    numArgs = framePtr->procPtr->numArgs;
    desiredObjs = TclStackAlloc(interp,
	    (int) sizeof(Tcl_Obj *) * (numArgs+1));
=======
	ckfree(localPtr);
=======
	Tcl_Free(localPtr);
>>>>>>> upstream/master
=======
	Tcl_Free(localPtr);
>>>>>>> upstream/master
	localPtr = nextPtr;
    }
    Tcl_Free(procPtr);

    /*
     * TIP #280: Release the location data associated with this Proc
     * structure, if any. The interpreter may not exist (For example for
     * procbody structures created by tbcload.
     */

    if (iPtr == NULL) {
	return;
    }
>>>>>>> upstream/master

    if (framePtr->isProcCallFrame & FRAME_IS_LAMBDA) {
	desiredObjs[0] = Tcl_NewStringObj("lambdaExpr", -1);
    } else {
	((Interp *) interp)->ensembleRewrite.numInsertedObjs -= skip - 1;

#ifdef AVOID_HACKS_FOR_ITCL
	desiredObjs[0] = framePtr->objv[skip-1];
#else
	desiredObjs[0] = Tcl_NewListObj(skip, framePtr->objv);
#endif /* AVOID_HACKS_FOR_ITCL */
    }
    Tcl_IncrRefCount(desiredObjs[0]);

<<<<<<< HEAD
    defPtr = (Var *) (&framePtr->localCachePtr->varName0 + localCt);
    for (i=1 ; i<=numArgs ; i++, defPtr++) {
	Tcl_Obj *argObj;
	Tcl_Obj *namePtr = localName(framePtr, i-1);

<<<<<<< HEAD
	if (defPtr->value.objPtr != NULL) {
	    TclNewObj(argObj);
	    Tcl_AppendStringsToObj(argObj, "?", TclGetString(namePtr), "?", NULL);
	} else if (defPtr->flags & VAR_IS_ARGS) {
	    numArgs--;
	    final = "?arg ...?";
	    break;
	} else {
	    argObj = namePtr;
	    Tcl_IncrRefCount(namePtr);
	}
	desiredObjs[i] = argObj;
    }
=======
    cfPtr = (CmdFrame *)Tcl_GetHashValue(hePtr);
>>>>>>> upstream/master

<<<<<<< HEAD
    Tcl_ResetResult(interp);
    Tcl_WrongNumArgs(interp, numArgs+1, desiredObjs, final);
=======
=======
>>>>>>> upstream/master
    if (cfPtr) {
	if (cfPtr->type == TCL_LOCATION_SOURCE) {
	    Tcl_DecrRefCount(cfPtr->data.eval.path);
	    cfPtr->data.eval.path = NULL;
	}
	Tcl_Free(cfPtr->line);
	cfPtr->line = NULL;
	Tcl_Free(cfPtr);
    }
    Tcl_DeleteHashEntry(hePtr);
}

/*
 *----------------------------------------------------------------------
 *
 * TclUpdateReturnInfo --
 *
 *	This function is called when procedures return, and at other points
 *	where the TCL_RETURN code is used. It examines the returnLevel and
 *	returnCode to determine the real return status.
 *
 * Results:
 *	The return value is the true completion code to use for the procedure
 *	or script, instead of TCL_RETURN.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
TclUpdateReturnInfo(
    Interp *iPtr)		/* Interpreter for which TCL_RETURN exception
				 * is being processed. */
{
    int code = TCL_RETURN;
>>>>>>> upstream/master

    for (i=0 ; i<=numArgs ; i++) {
	Tcl_DecrRefCount(desiredObjs[i]);
    }
    TclStackFree(interp, desiredObjs);
    return TCL_ERROR;
=======
static void
MakeProcError(
    Tcl_Interp *interp,		/* The interpreter in which the procedure was
				 * called. */
    Tcl_Obj *procNameObj)	/* Name of the procedure. Used for error
				 * messages and trace information. */
{
    int overflow, limit = 60, nameLen;
    const char *procName = TclGetStringFromObj(procNameObj, &nameLen);

<<<<<<< HEAD
    overflow = (nameLen > limit);
    Tcl_AppendObjToErrorInfo(interp, Tcl_ObjPrintf(
	    "\n    (procedure \"%.*s%s\" line %d)",
	    (overflow ? limit : nameLen), procName,
	    (overflow ? "..." : ""), Tcl_GetErrorLine(interp)));
>>>>>>> upstream/master
=======
	code = iPtr->returnCode;
	iPtr->returnLevel = 1;
	iPtr->returnCode = TCL_OK;
	if (code == TCL_ERROR) {
	    iPtr->flags |= ERR_LEGACY_COPY;
	}
    }
    return code;
>>>>>>> upstream/master
}

>>>>>>> upstream/master
/*
 *----------------------------------------------------------------------
 *
 * TclInitCompiledLocals --
 *
<<<<<<< HEAD
 *	This routine is invoked in order to initialize the compiled locals
 *	table for a new call frame.
 *
 *	DEPRECATED: functionality has been inlined elsewhere; this function
 *	remains to insure binary compatibility with Itcl.
=======
 *	This function is invoked just before a command procedure is removed
 *	from an interpreter. Its job is to release all the resources allocated
 *	to the procedure.
>>>>>>> upstream/master
 *
 * Results:
 *	None.
 *
 * Side effects:
<<<<<<< HEAD
 *	May invoke various name resolvers in order to determine which
 *	variables are being referenced at runtime.
=======
 *	Memory gets freed, unless the procedure is actively being executed.
 *	In this case the cleanup is delayed until the last call to the current
 *	procedure completes.
>>>>>>> upstream/master
 *
 *----------------------------------------------------------------------
 */

void
<<<<<<< HEAD
TclInitCompiledLocals(
    Tcl_Interp *interp,		/* Current interpreter. */
    CallFrame *framePtr,	/* Call frame to initialize. */
    Namespace *nsPtr)		/* Pointer to current namespace. */
{
    Var *varPtr = framePtr->compiledLocals;
    Tcl_Obj *bodyPtr;
    ByteCode *codePtr;
<<<<<<< HEAD

    bodyPtr = framePtr->procPtr->bodyPtr;
    if (bodyPtr->typePtr != &tclByteCodeType) {
	Tcl_Panic("body object for proc attached to frame is not a byte code type");
    }
    codePtr = bodyPtr->internalRep.twoPtrValue.ptr1;

<<<<<<< HEAD
    if (framePtr->numCompiledLocals) {
	if (!codePtr->localCachePtr) {
	    InitLocalCache(framePtr->procPtr) ;
	}
	framePtr->localCachePtr = codePtr->localCachePtr;
	framePtr->localCachePtr->refCount++;
=======
TclProcDeleteProc(
    ClientData clientData)	/* Procedure to be deleted. */
{
    Proc *procPtr = clientData;
=======
    TclNewObj(objPtr);
    if (objPtr) {
	objPtr->typePtr = &tclProcBodyType;
	objPtr->internalRep.twoPtrValue.ptr1 = procPtr;
>>>>>>> upstream/master

    if (procPtr->refCount-- <= 1) {
	TclProcCleanupProc(procPtr);
>>>>>>> upstream/master
    }

    InitResolvedLocals(interp, codePtr, varPtr, nsPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * InitResolvedLocals --
 *
<<<<<<< HEAD
 *	This routine is invoked in order to initialize the compiled locals
 *	table for a new call frame.
=======
 *	This function does all the real work of freeing up a Proc structure.
 *	It's called only when the structure's reference count becomes zero.
>>>>>>> upstream/master
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	May invoke various name resolvers in order to determine which
 *	variables are being referenced at runtime.
 *
 *----------------------------------------------------------------------
 */

<<<<<<< HEAD
static void
<<<<<<< HEAD
InitResolvedLocals(
    Tcl_Interp *interp,		/* Current interpreter. */
    ByteCode *codePtr,
    Var *varPtr,
    Namespace *nsPtr)		/* Pointer to current namespace. */
=======
void
TclProcCleanupProc(
    register Proc *procPtr)	/* Procedure to be deleted. */
>>>>>>> upstream/master
{
    Interp *iPtr = (Interp *) interp;
    int haveResolvers = (nsPtr->compiledVarResProc || iPtr->resolverPtr);
    CompiledLocal *firstLocalPtr, *localPtr;
    int varNum;
    Tcl_ResolvedVarInfo *resVarInfo;
    Tcl_HashEntry *hePtr = NULL;
    CmdFrame *cfPtr = NULL;
    Interp *iPtr = procPtr->iPtr;

    /*
     * Find the localPtr corresponding to varPtr
     */

    varNum = varPtr - iPtr->framePtr->compiledLocals;
    localPtr = iPtr->framePtr->procPtr->firstLocalPtr;
    while (varNum--) {
	localPtr = localPtr->nextPtr;
=======

    bodyPtr = framePtr->procPtr->bodyPtr;
    if (bodyPtr->typePtr != &tclByteCodeType) {
	Tcl_Panic("body object for proc attached to frame is not a byte code type");
>>>>>>> upstream/master
    }
<<<<<<< HEAD
    codePtr = bodyPtr->internalRep.twoPtrValue.ptr1;

<<<<<<< HEAD
    if (!(haveResolvers && (codePtr->flags & TCL_BYTECODE_RESOLVE_VARS))) {
	goto doInitResolvedLocals;
    }

    /*
     * This is the first run after a recompile, or else the resolver epoch
     * has changed: update the resolver cache.
     */

    firstLocalPtr = localPtr;
    for (; localPtr != NULL; localPtr = localPtr->nextPtr) {
	if (localPtr->resolveInfo) {
	    if (localPtr->resolveInfo->deleteProc) {
		localPtr->resolveInfo->deleteProc(localPtr->resolveInfo);
	    } else {
		ckfree(localPtr->resolveInfo);
	    }
	    localPtr->resolveInfo = NULL;
	}
	localPtr->flags &= ~VAR_RESOLVED;
=======
    for (localPtr = procPtr->firstLocalPtr; localPtr != NULL; ) {
	CompiledLocal *nextPtr = localPtr->nextPtr;

	resVarInfo = localPtr->resolveInfo;
	if (resVarInfo) {
	    if (resVarInfo->deleteProc) {
		resVarInfo->deleteProc(resVarInfo);
	    } else {
		ckfree(resVarInfo);
	    }
	}
>>>>>>> upstream/master

	if (haveResolvers &&
		!(localPtr->flags & (VAR_ARGUMENT|VAR_TEMPORARY))) {
	    ResolverScheme *resPtr = iPtr->resolverPtr;
	    Tcl_ResolvedVarInfo *vinfo;
	    int result;

	    if (nsPtr->compiledVarResProc) {
		result = nsPtr->compiledVarResProc(nsPtr->interp,
			localPtr->name, localPtr->nameLength,
			(Tcl_Namespace *) nsPtr, &vinfo);
	    } else {
		result = TCL_CONTINUE;
	    }

	    while ((result == TCL_CONTINUE) && resPtr) {
		if (resPtr->compiledVarResProc) {
		    result = resPtr->compiledVarResProc(nsPtr->interp,
			    localPtr->name, localPtr->nameLength,
			    (Tcl_Namespace *) nsPtr, &vinfo);
		}
		resPtr = resPtr->nextPtr;
	    }
	    if (result == TCL_OK) {
		localPtr->resolveInfo = vinfo;
		localPtr->flags |= VAR_RESOLVED;
	    }
	}
<<<<<<< HEAD
    }
    localPtr = firstLocalPtr;
    codePtr->flags &= ~TCL_BYTECODE_RESOLVE_VARS;

    /*
     * Initialize the array of local variables stored in the call frame. Some
     * variables may have special resolution rules. In that case, we call
     * their "resolver" procs to get our hands on the variable, and we make
     * the compiled local a link to the real variable.
     */

  doInitResolvedLocals:
    for (; localPtr != NULL; varPtr++, localPtr = localPtr->nextPtr) {
	varPtr->flags = 0;
	varPtr->value.objPtr = NULL;

	/*
	 * Now invoke the resolvers to determine the exact variables that
	 * should be used.
	 */

	resVarInfo = localPtr->resolveInfo;
	if (resVarInfo && resVarInfo->fetchProc) {
	    register Var *resolvedVarPtr = (Var *)
		    resVarInfo->fetchProc(interp, resVarInfo);

	    if (resolvedVarPtr) {
		if (TclIsVarInHash(resolvedVarPtr)) {
		    VarHashRefCount(resolvedVarPtr)++;
		}
		varPtr->flags = VAR_LINK;
		varPtr->value.linkPtr = resolvedVarPtr;
	    }
=======
	ckfree(localPtr);
	localPtr = nextPtr;
    }
    ckfree(procPtr);

    /*
     * TIP #280: Release the location data associated with this Proc
     * structure, if any. The interpreter may not exist (For example for
     * procbody structures created by tbcload.
     */

    if (iPtr == NULL) {
	return;
    }

    hePtr = Tcl_FindHashEntry(iPtr->linePBodyPtr, (char *) procPtr);
    if (!hePtr) {
	return;
    }

    cfPtr = Tcl_GetHashValue(hePtr);

    if (cfPtr) {
	if (cfPtr->type == TCL_LOCATION_SOURCE) {
	    Tcl_DecrRefCount(cfPtr->data.eval.path);
	    cfPtr->data.eval.path = NULL;
	}
	ckfree(cfPtr->line);
	cfPtr->line = NULL;
	ckfree(cfPtr);
    }
    Tcl_DeleteHashEntry(hePtr);
=======
ProcBodyDup(
    Tcl_Obj *srcPtr,		/* Object to copy. */
    Tcl_Obj *dupPtr)		/* Target object for the duplication. */
{
    Proc *procPtr = srcPtr->internalRep.twoPtrValue.ptr1;

    dupPtr->typePtr = &tclProcBodyType;
    dupPtr->internalRep.twoPtrValue.ptr1 = procPtr;
    procPtr->refCount++;
>>>>>>> upstream/master
}

/*
 *----------------------------------------------------------------------
 *
 * TclUpdateReturnInfo --
 *
 *	This function is called when procedures return, and at other points
 *	where the TCL_RETURN code is used. It examines the returnLevel and
 *	returnCode to determine the real return status.
 *
 * Results:
 *	The return value is the true completion code to use for the procedure
 *	or script, instead of TCL_RETURN.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
TclUpdateReturnInfo(
    Interp *iPtr)		/* Interpreter for which TCL_RETURN exception
				 * is being processed. */
{
    int code = TCL_RETURN;

    iPtr->returnLevel--;
    if (iPtr->returnLevel < 0) {
	Tcl_Panic("TclUpdateReturnInfo: negative return level");
    }
    if (iPtr->returnLevel == 0) {
	/*
	 * Now we've reached the level to return the requested -code.
	 * Since iPtr->returnLevel and iPtr->returnCode have completed
	 * their task, we now reset them to default values so that any
	 * bare "return TCL_RETURN" that may follow will work [Bug 2152286].
	 */

	code = iPtr->returnCode;
	iPtr->returnLevel = 1;
	iPtr->returnCode = TCL_OK;
	if (code == TCL_ERROR) {
	    iPtr->flags |= ERR_LEGACY_COPY;
>>>>>>> upstream/master
	}
    }
}

<<<<<<< HEAD
void
TclFreeLocalCache(
    Tcl_Interp *interp,
    LocalCache *localCachePtr)
{
    int i;
    Tcl_Obj **namePtrPtr = &localCachePtr->varName0;

    for (i = 0; i < localCachePtr->numVars; i++, namePtrPtr++) {
	register Tcl_Obj *objPtr = *namePtrPtr;

	if (objPtr) {
	    /* TclReleaseLiteral calls Tcl_DecrRefCount for us */
	    TclReleaseLiteral(interp, objPtr);
	}
    }
    ckfree(localCachePtr);
}

static void
InitLocalCache(
    Proc *procPtr)
=======
/*
 *----------------------------------------------------------------------
 *
 * TclGetObjInterpProc --
 *
 *	Returns a pointer to the TclObjInterpProc function; this is different
 *	from the value obtained from the TclObjInterpProc reference on systems
 *	like Windows where import and export versions of a function exported
 *	by a DLL exist.
 *
 * Results:
 *	Returns the internal address of the TclObjInterpProc function.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

TclObjCmdProcType
TclGetObjInterpProc(void)
{
    return (TclObjCmdProcType) TclObjInterpProc;
}

/*
 *----------------------------------------------------------------------
 *
 * TclNewProcBodyObj --
 *
 *	Creates a new object, of type "procbody", whose internal
 *	representation is the given Proc struct. The newly created object's
 *	reference count is 0.
 *
 * Results:
 *	Returns a pointer to a newly allocated Tcl_Obj, NULL on error.
 *
 * Side effects:
 *	The reference count in the ByteCode attached to the Proc is bumped up
 *	by one, since the internal rep stores a pointer to it.
 *
 *----------------------------------------------------------------------
 */

Tcl_Obj *
TclNewProcBodyObj(
    Proc *procPtr)		/* the Proc struct to store as the internal
				 * representation. */
>>>>>>> upstream/master
{
    Interp *iPtr = procPtr->iPtr;
    ByteCode *codePtr = procPtr->bodyPtr->internalRep.twoPtrValue.ptr1;
    int localCt = procPtr->numCompiledLocals;
    int numArgs = procPtr->numArgs, i = 0;

<<<<<<< HEAD
    Tcl_Obj **namePtr;
    Var *varPtr;
    LocalCache *localCachePtr;
    CompiledLocal *localPtr;
    int new;

    /*
     * Cache the names and initial values of local variables; store the
     * cache in both the framePtr for this execution and in the codePtr
     * for future calls.
     */

    localCachePtr = ckalloc(sizeof(LocalCache)
	    + (localCt - 1) * sizeof(Tcl_Obj *)
	    + numArgs * sizeof(Var));

    namePtr = &localCachePtr->varName0;
    varPtr = (Var *) (namePtr + localCt);
    localPtr = procPtr->firstLocalPtr;
    while (localPtr) {
	if (TclIsVarTemporary(localPtr)) {
	    *namePtr = NULL;
	} else {
	    *namePtr = TclCreateLiteral(iPtr, localPtr->name,
		    localPtr->nameLength, /* hash */ (unsigned int) -1,
		    &new, /* nsPtr */ NULL, 0, NULL);
	    Tcl_IncrRefCount(*namePtr);
	}

	if (i < numArgs) {
	    varPtr->flags = (localPtr->flags & VAR_IS_ARGS);
	    varPtr->value.objPtr = localPtr->defValuePtr;
	    varPtr++;
	    i++;
	}
	namePtr++;
	localPtr = localPtr->nextPtr;
    }
=======
    if (framePtr->numCompiledLocals) {
	if (!codePtr->localCachePtr) {
	    InitLocalCache(framePtr->procPtr) ;
	}
	framePtr->localCachePtr = codePtr->localCachePtr;
	framePtr->localCachePtr->refCount++;
    }

    InitResolvedLocals(interp, codePtr, varPtr, nsPtr);
=======
    if (!procPtr) {
	return NULL;
    }

    TclNewObj(objPtr);
    if (objPtr) {
	objPtr->typePtr = &tclProcBodyType;
	objPtr->internalRep.twoPtrValue.ptr1 = procPtr;

	procPtr->refCount++;
    }

    return objPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * ProcBodyDup --
 *
 *	Tcl_ObjType's Dup function for the proc body object. Bumps the
 *	reference count on the Proc stored in the internal representation.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Sets up the object in dupPtr to be a duplicate of the one in srcPtr.
 *
 *----------------------------------------------------------------------
 */

static void
ProcBodyDup(
    Tcl_Obj *srcPtr,		/* Object to copy. */
    Tcl_Obj *dupPtr)		/* Target object for the duplication. */
{
    Proc *procPtr = srcPtr->internalRep.twoPtrValue.ptr1;

    dupPtr->typePtr = &tclProcBodyType;
    dupPtr->internalRep.twoPtrValue.ptr1 = procPtr;
    procPtr->refCount++;
>>>>>>> upstream/master
}

/*
 *----------------------------------------------------------------------
 *
 * InitResolvedLocals --
 *
<<<<<<< HEAD
 *	This routine is invoked in order to initialize the compiled locals
 *	table for a new call frame.
=======
 *	Tcl_ObjType's Free function for the proc body object. The reference
 *	count on its Proc struct is decreased by 1; if the count reaches 0,
 *	the proc is freed.
>>>>>>> upstream/master
 *
 * Results:
 *	None.
 *
 * Side effects:
<<<<<<< HEAD
 *	May invoke various name resolvers in order to determine which
 *	variables are being referenced at runtime.
=======
 *	If the reference count on the Proc struct reaches 0, the struct is
 *	freed.
>>>>>>> upstream/master
 *
 *----------------------------------------------------------------------
 */

static void
<<<<<<< HEAD
InitResolvedLocals(
    Tcl_Interp *interp,		/* Current interpreter. */
    ByteCode *codePtr,
    Var *varPtr,
    Namespace *nsPtr)		/* Pointer to current namespace. */
{
    Interp *iPtr = (Interp *) interp;
    int haveResolvers = (nsPtr->compiledVarResProc || iPtr->resolverPtr);
    CompiledLocal *firstLocalPtr, *localPtr;
    int varNum;
    Tcl_ResolvedVarInfo *resVarInfo;

    /*
     * Find the localPtr corresponding to varPtr
     */

    varNum = varPtr - iPtr->framePtr->compiledLocals;
    localPtr = iPtr->framePtr->procPtr->firstLocalPtr;
    while (varNum--) {
	localPtr = localPtr->nextPtr;
    }

    if (!(haveResolvers && (codePtr->flags & TCL_BYTECODE_RESOLVE_VARS))) {
	goto doInitResolvedLocals;
    }

    /*
     * This is the first run after a recompile, or else the resolver epoch
     * has changed: update the resolver cache.
     */

    firstLocalPtr = localPtr;
    for (; localPtr != NULL; localPtr = localPtr->nextPtr) {
	if (localPtr->resolveInfo) {
	    if (localPtr->resolveInfo->deleteProc) {
		localPtr->resolveInfo->deleteProc(localPtr->resolveInfo);
	    } else {
		ckfree(localPtr->resolveInfo);
	    }
	    localPtr->resolveInfo = NULL;
	}
	localPtr->flags &= ~VAR_RESOLVED;

	if (haveResolvers &&
		!(localPtr->flags & (VAR_ARGUMENT|VAR_TEMPORARY))) {
	    ResolverScheme *resPtr = iPtr->resolverPtr;
	    Tcl_ResolvedVarInfo *vinfo;
	    int result;

	    if (nsPtr->compiledVarResProc) {
		result = nsPtr->compiledVarResProc(nsPtr->interp,
			localPtr->name, localPtr->nameLength,
			(Tcl_Namespace *) nsPtr, &vinfo);
	    } else {
		result = TCL_CONTINUE;
	    }

	    while ((result == TCL_CONTINUE) && resPtr) {
		if (resPtr->compiledVarResProc) {
		    result = resPtr->compiledVarResProc(nsPtr->interp,
			    localPtr->name, localPtr->nameLength,
			    (Tcl_Namespace *) nsPtr, &vinfo);
		}
		resPtr = resPtr->nextPtr;
	    }
	    if (result == TCL_OK) {
		localPtr->resolveInfo = vinfo;
		localPtr->flags |= VAR_RESOLVED;
	    }
	}
    }
    localPtr = firstLocalPtr;
    codePtr->flags &= ~TCL_BYTECODE_RESOLVE_VARS;

    /*
     * Initialize the array of local variables stored in the call frame. Some
     * variables may have special resolution rules. In that case, we call
     * their "resolver" procs to get our hands on the variable, and we make
     * the compiled local a link to the real variable.
     */

  doInitResolvedLocals:
    for (; localPtr != NULL; varPtr++, localPtr = localPtr->nextPtr) {
	varPtr->flags = 0;
	varPtr->value.objPtr = NULL;

	/*
	 * Now invoke the resolvers to determine the exact variables that
	 * should be used.
	 */

	resVarInfo = localPtr->resolveInfo;
	if (resVarInfo && resVarInfo->fetchProc) {
	    register Var *resolvedVarPtr = (Var *)
		    resVarInfo->fetchProc(interp, resVarInfo);

	    if (resolvedVarPtr) {
		if (TclIsVarInHash(resolvedVarPtr)) {
		    VarHashRefCount(resolvedVarPtr)++;
		}
		varPtr->flags = VAR_LINK;
		varPtr->value.linkPtr = resolvedVarPtr;
	    }
	}
    }
}

void
TclFreeLocalCache(
    Tcl_Interp *interp,
    LocalCache *localCachePtr)
{
    int i;
    Tcl_Obj **namePtrPtr = &localCachePtr->varName0;

    for (i = 0; i < localCachePtr->numVars; i++, namePtrPtr++) {
	register Tcl_Obj *objPtr = *namePtrPtr;

	if (objPtr) {
	    /* TclReleaseLiteral calls Tcl_DecrRefCount for us */
	    TclReleaseLiteral(interp, objPtr);
	}
    }
    ckfree(localCachePtr);
}

static void
InitLocalCache(
    Proc *procPtr)
{
    Interp *iPtr = procPtr->iPtr;
    ByteCode *codePtr = procPtr->bodyPtr->internalRep.twoPtrValue.ptr1;
    int localCt = procPtr->numCompiledLocals;
    int numArgs = procPtr->numArgs, i = 0;

    Tcl_Obj **namePtr;
    Var *varPtr;
    LocalCache *localCachePtr;
    CompiledLocal *localPtr;
    int new;

    /*
     * Cache the names and initial values of local variables; store the
     * cache in both the framePtr for this execution and in the codePtr
     * for future calls.
     */

    localCachePtr = ckalloc(sizeof(LocalCache)
	    + (localCt - 1) * sizeof(Tcl_Obj *)
	    + numArgs * sizeof(Var));

    namePtr = &localCachePtr->varName0;
    varPtr = (Var *) (namePtr + localCt);
    localPtr = procPtr->firstLocalPtr;
    while (localPtr) {
	if (TclIsVarTemporary(localPtr)) {
	    *namePtr = NULL;
	} else {
	    *namePtr = TclCreateLiteral(iPtr, localPtr->name,
		    localPtr->nameLength, /* hash */ (unsigned int) -1,
		    &new, /* nsPtr */ NULL, 0, NULL);
	    Tcl_IncrRefCount(*namePtr);
	}

	if (i < numArgs) {
	    varPtr->flags = (localPtr->flags & VAR_IS_ARGS);
	    varPtr->value.objPtr = localPtr->defValuePtr;
	    varPtr++;
	    i++;
	}
	namePtr++;
	localPtr = localPtr->nextPtr;
    }
>>>>>>> upstream/master
    codePtr->localCachePtr = localCachePtr;
    localCachePtr->refCount = 1;
    localCachePtr->numVars = localCt;
=======
{
    Tcl_Command origCmd = TclGetOriginalCommand((Tcl_Command) cmdPtr);

    if (origCmd != NULL) {
	cmdPtr = (Command *) origCmd;
    }
    if (cmdPtr->deleteProc == TclProcDeleteProc) {
	return cmdPtr->objClientData;
    }
    return NULL;
}

static int
ProcWrongNumArgs(
    Tcl_Interp *interp,
    int skip)
{
    CallFrame *framePtr = ((Interp *)interp)->varFramePtr;
    register Proc *procPtr = framePtr->procPtr;
    register Var *defPtr;
    int localCt = procPtr->numCompiledLocals, numArgs, i;
    Tcl_Obj **desiredObjs;
    const char *final = NULL;

    /*
     * Build up desired argument list for Tcl_WrongNumArgs
     */

    numArgs = framePtr->procPtr->numArgs;
    desiredObjs = TclStackAlloc(interp,
	    (int) sizeof(Tcl_Obj *) * (numArgs+1));

    if (framePtr->isProcCallFrame & FRAME_IS_LAMBDA) {
	desiredObjs[0] = Tcl_NewStringObj("lambdaExpr", -1);
    } else {
#ifdef AVOID_HACKS_FOR_ITCL
	desiredObjs[0] = framePtr->objv[skip-1];
#else
	desiredObjs[0] = Tcl_NewListObj(1, framePtr->objv + skip - 1);
#endif /* AVOID_HACKS_FOR_ITCL */
    }
    Tcl_IncrRefCount(desiredObjs[0]);

    defPtr = (Var *) (&framePtr->localCachePtr->varName0 + localCt);
    for (i=1 ; i<=numArgs ; i++, defPtr++) {
	Tcl_Obj *argObj;
	Tcl_Obj *namePtr = localName(framePtr, i-1);

	if (defPtr->value.objPtr != NULL) {
	    TclNewObj(argObj);
	    Tcl_AppendStringsToObj(argObj, "?", TclGetString(namePtr), "?", NULL);
	} else if (defPtr->flags & VAR_IS_ARGS) {
	    numArgs--;
	    final = "?arg ...?";
	    break;
	} else {
	    argObj = namePtr;
	    Tcl_IncrRefCount(namePtr);
	}
	desiredObjs[i] = argObj;
    }

    Tcl_ResetResult(interp);
    Tcl_WrongNumArgs(interp, numArgs+1, desiredObjs, final);

    for (i=0 ; i<=numArgs ; i++) {
	Tcl_DecrRefCount(desiredObjs[i]);
    }
    TclStackFree(interp, desiredObjs);
    return TCL_ERROR;
>>>>>>> upstream/master
}

/*
 *----------------------------------------------------------------------
 *
<<<<<<< HEAD
 * InitArgsAndLocals --
 *
 *	This routine is invoked in order to initialize the arguments and other
 *	compiled locals table for a new call frame.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	Allocates memory on the stack for the compiled local variables, the
 *	caller is responsible for freeing them. Initialises all variables. May
 *	invoke various name resolvers in order to determine which variables
 *	are being referenced at runtime.
=======
 * TclInitCompiledLocals --
 *
 *	This routine is invoked in order to initialize the compiled locals
 *	table for a new call frame.
 *
 *	DEPRECATED: functionality has been inlined elsewhere; this function
 *	remains to insure binary compatibility with Itcl.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	May invoke various name resolvers in order to determine which
 *	variables are being referenced at runtime.
>>>>>>> upstream/master
 *
 *----------------------------------------------------------------------
 */

<<<<<<< HEAD
static int
InitArgsAndLocals(
    register Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    Tcl_Obj *procNameObj,	/* Procedure name for error reporting. */
    int skip)			/* Number of initial arguments to be skipped,
				 * i.e., words in the "command name". */
{
    CallFrame *framePtr = ((Interp *)interp)->varFramePtr;
    register Proc *procPtr = framePtr->procPtr;
    ByteCode *codePtr = procPtr->bodyPtr->internalRep.twoPtrValue.ptr1;
    register Var *varPtr, *defPtr;
    int localCt = procPtr->numCompiledLocals, numArgs, argCt, i, imax;
    Tcl_Obj *const *argObjs;

    /*
     * Make sure that the local cache of variable names and initial values has
     * been initialised properly .
     */

    if (localCt) {
	if (!codePtr->localCachePtr) {
	    InitLocalCache(procPtr) ;
	}
	framePtr->localCachePtr = codePtr->localCachePtr;
	framePtr->localCachePtr->refCount++;
	defPtr = (Var *) (&framePtr->localCachePtr->varName0 + localCt);
    } else {
	defPtr = NULL;
    }

    /*
     * Create the "compiledLocals" array. Make sure it is large enough to hold
     * all the procedure's compiled local variables, including its formal
     * parameters.
     */

    varPtr = TclStackAlloc(interp, (int)(localCt * sizeof(Var)));
    framePtr->compiledLocals = varPtr;
    framePtr->numCompiledLocals = localCt;

    /*
     * Match and assign the call's actual parameters to the procedure's formal
     * arguments. The formal arguments are described by the first numArgs
     * entries in both the Proc structure's local variable list and the call
     * frame's local variable array.
     */

    numArgs = procPtr->numArgs;
    argCt = framePtr->objc - skip;	/* Set it to the number of args to the
					 * procedure. */
    argObjs = framePtr->objv + skip;
    if (numArgs == 0) {
	if (argCt) {
	    goto incorrectArgs;
	} else {
	    goto correctArgs;
	}
    }
    imax = ((argCt < numArgs-1) ? argCt : numArgs-1);
    for (i = 0; i < imax; i++, varPtr++, defPtr ? defPtr++ : defPtr) {
	/*
	 * "Normal" arguments; last formal is special, depends on it being
	 * 'args'.
	 */

	Tcl_Obj *objPtr = argObjs[i];

	varPtr->flags = 0;
	varPtr->value.objPtr = objPtr;
	Tcl_IncrRefCount(objPtr);	/* Local var is a reference. */
    }
    for (; i < numArgs-1; i++, varPtr++, defPtr ? defPtr++ : defPtr) {
	/*
	 * This loop is entered if argCt < (numArgs-1). Set default values;
	 * last formal is special.
	 */

	Tcl_Obj *objPtr = defPtr ? defPtr->value.objPtr : NULL;

	if (!objPtr) {
	    goto incorrectArgs;
	}
	varPtr->flags = 0;
	varPtr->value.objPtr = objPtr;
	Tcl_IncrRefCount(objPtr);	/* Local var reference. */
    }

    /*
     * When we get here, the last formal argument remains to be defined:
     * defPtr and varPtr point to the last argument to be initialized.
     */

    varPtr->flags = 0;
    if (defPtr && defPtr->flags & VAR_IS_ARGS) {
	Tcl_Obj *listPtr = Tcl_NewListObj(argCt-i, argObjs+i);

	varPtr->value.objPtr = listPtr;
	Tcl_IncrRefCount(listPtr);	/* Local var is a reference. */
    } else if (argCt == numArgs) {
	Tcl_Obj *objPtr = argObjs[i];

	varPtr->value.objPtr = objPtr;
	Tcl_IncrRefCount(objPtr);	/* Local var is a reference. */
    } else if ((argCt < numArgs) && defPtr && defPtr->value.objPtr) {
	Tcl_Obj *objPtr = defPtr->value.objPtr;

	varPtr->value.objPtr = objPtr;
	Tcl_IncrRefCount(objPtr);	/* Local var is a reference. */
    } else {
	goto incorrectArgs;
    }
    varPtr++;

    /*
     * Initialise and resolve the remaining compiledLocals. In the absence of
     * resolvers, they are undefined local vars: (flags=0, value=NULL).
     */

  correctArgs:
    if (numArgs < localCt) {
	if (!framePtr->nsPtr->compiledVarResProc
		&& !((Interp *)interp)->resolverPtr) {
	    memset(varPtr, 0, (localCt - numArgs)*sizeof(Var));
	} else {
	    InitResolvedLocals(interp, codePtr, varPtr, framePtr->nsPtr);
	}
    }

    return TCL_OK;

    /*
     * Initialise all compiled locals to avoid problems at DeleteLocalVars.
     */

  incorrectArgs:
    memset(varPtr, 0,
	    ((framePtr->compiledLocals + localCt)-varPtr) * sizeof(Var));
    return ProcWrongNumArgs(interp, skip);
=======
void
TclInitCompiledLocals(
    Tcl_Interp *interp,		/* Current interpreter. */
    CallFrame *framePtr,	/* Call frame to initialize. */
    Namespace *nsPtr)		/* Pointer to current namespace. */
{
    Var *varPtr = framePtr->compiledLocals;
    Tcl_Obj *bodyPtr;
    ByteCode *codePtr;

    bodyPtr = framePtr->procPtr->bodyPtr;
    if (bodyPtr->typePtr != &tclByteCodeType) {
	Tcl_Panic("body object for proc attached to frame is not a byte code type");
    }
    codePtr = bodyPtr->internalRep.twoPtrValue.ptr1;

    if (framePtr->numCompiledLocals) {
	if (!codePtr->localCachePtr) {
	    InitLocalCache(framePtr->procPtr) ;
	}
	framePtr->localCachePtr = codePtr->localCachePtr;
	framePtr->localCachePtr->refCount++;
    }

    InitResolvedLocals(interp, codePtr, varPtr, nsPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * InitResolvedLocals --
 *
 *	This routine is invoked in order to initialize the compiled locals
 *	table for a new call frame.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	May invoke various name resolvers in order to determine which
 *	variables are being referenced at runtime.
 *
 *----------------------------------------------------------------------
 */

static void
InitResolvedLocals(
    Tcl_Interp *interp,		/* Current interpreter. */
    ByteCode *codePtr,
    Var *varPtr,
    Namespace *nsPtr)		/* Pointer to current namespace. */
{
    Interp *iPtr = (Interp *) interp;
    int haveResolvers = (nsPtr->compiledVarResProc || iPtr->resolverPtr);
    CompiledLocal *firstLocalPtr, *localPtr;
    int varNum;
    Tcl_ResolvedVarInfo *resVarInfo;

    /*
     * Find the localPtr corresponding to varPtr
     */

    varNum = varPtr - iPtr->framePtr->compiledLocals;
    localPtr = iPtr->framePtr->procPtr->firstLocalPtr;
    while (varNum--) {
	localPtr = localPtr->nextPtr;
    }

    if (!(haveResolvers && (codePtr->flags & TCL_BYTECODE_RESOLVE_VARS))) {
	goto doInitResolvedLocals;
    }

    /*
     * This is the first run after a recompile, or else the resolver epoch
     * has changed: update the resolver cache.
     */

    firstLocalPtr = localPtr;
    for (; localPtr != NULL; localPtr = localPtr->nextPtr) {
	if (localPtr->resolveInfo) {
	    if (localPtr->resolveInfo->deleteProc) {
		localPtr->resolveInfo->deleteProc(localPtr->resolveInfo);
	    } else {
		ckfree(localPtr->resolveInfo);
	    }
	    localPtr->resolveInfo = NULL;
	}
	localPtr->flags &= ~VAR_RESOLVED;

	if (haveResolvers &&
		!(localPtr->flags & (VAR_ARGUMENT|VAR_TEMPORARY))) {
	    ResolverScheme *resPtr = iPtr->resolverPtr;
	    Tcl_ResolvedVarInfo *vinfo;
	    int result;

	    if (nsPtr->compiledVarResProc) {
		result = nsPtr->compiledVarResProc(nsPtr->interp,
			localPtr->name, localPtr->nameLength,
			(Tcl_Namespace *) nsPtr, &vinfo);
	    } else {
		result = TCL_CONTINUE;
	    }

	    while ((result == TCL_CONTINUE) && resPtr) {
		if (resPtr->compiledVarResProc) {
		    result = resPtr->compiledVarResProc(nsPtr->interp,
			    localPtr->name, localPtr->nameLength,
			    (Tcl_Namespace *) nsPtr, &vinfo);
		}
		resPtr = resPtr->nextPtr;
	    }
	    if (result == TCL_OK) {
		localPtr->resolveInfo = vinfo;
		localPtr->flags |= VAR_RESOLVED;
	    }
	}
    }
    localPtr = firstLocalPtr;
    codePtr->flags &= ~TCL_BYTECODE_RESOLVE_VARS;

    /*
     * Initialize the array of local variables stored in the call frame. Some
     * variables may have special resolution rules. In that case, we call
     * their "resolver" procs to get our hands on the variable, and we make
     * the compiled local a link to the real variable.
     */

  doInitResolvedLocals:
    for (; localPtr != NULL; varPtr++, localPtr = localPtr->nextPtr) {
	varPtr->flags = 0;
	varPtr->value.objPtr = NULL;

	/*
	 * Now invoke the resolvers to determine the exact variables that
	 * should be used.
	 */

	resVarInfo = localPtr->resolveInfo;
	if (resVarInfo && resVarInfo->fetchProc) {
	    register Var *resolvedVarPtr = (Var *)
		    resVarInfo->fetchProc(interp, resVarInfo);

	    if (resolvedVarPtr) {
		if (TclIsVarInHash(resolvedVarPtr)) {
		    VarHashRefCount(resolvedVarPtr)++;
		}
		varPtr->flags = VAR_LINK;
		varPtr->value.linkPtr = resolvedVarPtr;
	    }
	}
    }
}

void
TclFreeLocalCache(
    Tcl_Interp *interp,
    LocalCache *localCachePtr)
{
    int i;
    Tcl_Obj **namePtrPtr = &localCachePtr->varName0;

    for (i = 0; i < localCachePtr->numVars; i++, namePtrPtr++) {
	register Tcl_Obj *objPtr = *namePtrPtr;

	if (objPtr) {
	    /* TclReleaseLiteral calls Tcl_DecrRefCount for us */
	    TclReleaseLiteral(interp, objPtr);
	}
    }
    ckfree(localCachePtr);
}

static void
InitLocalCache(
    Proc *procPtr)
{
    Interp *iPtr = procPtr->iPtr;
    ByteCode *codePtr = procPtr->bodyPtr->internalRep.twoPtrValue.ptr1;
    int localCt = procPtr->numCompiledLocals;
    int numArgs = procPtr->numArgs, i = 0;

    Tcl_Obj **namePtr;
    Var *varPtr;
    LocalCache *localCachePtr;
    CompiledLocal *localPtr;
    int new;

    /*
     * Cache the names and initial values of local variables; store the
     * cache in both the framePtr for this execution and in the codePtr
     * for future calls.
     */

    localCachePtr = ckalloc(sizeof(LocalCache)
	    + (localCt - 1) * sizeof(Tcl_Obj *)
	    + numArgs * sizeof(Var));

    namePtr = &localCachePtr->varName0;
    varPtr = (Var *) (namePtr + localCt);
    localPtr = procPtr->firstLocalPtr;
    while (localPtr) {
	if (TclIsVarTemporary(localPtr)) {
	    *namePtr = NULL;
	} else {
	    *namePtr = TclCreateLiteral(iPtr, localPtr->name,
		    localPtr->nameLength, /* hash */ (unsigned int) -1,
		    &new, /* nsPtr */ NULL, 0, NULL);
	    Tcl_IncrRefCount(*namePtr);
	}

	if (i < numArgs) {
	    varPtr->flags = (localPtr->flags & VAR_IS_ARGS);
	    varPtr->value.objPtr = localPtr->defValuePtr;
	    varPtr++;
	    i++;
	}
	namePtr++;
	localPtr = localPtr->nextPtr;
    }
    codePtr->localCachePtr = localCachePtr;
    localCachePtr->refCount = 1;
    localCachePtr->numVars = localCt;
>>>>>>> upstream/master
}

/*
 *----------------------------------------------------------------------
 *
<<<<<<< HEAD
 * TclPushProcCallFrame --
 *
 *	Compiles a proc body if necessary, then pushes a CallFrame suitable
 *	for executing it.
=======
 * InitArgsAndLocals --
 *
 *	This routine is invoked in order to initialize the arguments and other
 *	compiled locals table for a new call frame.
>>>>>>> upstream/master
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
<<<<<<< HEAD
 *	The proc's body may be recompiled. A CallFrame is pushed, it will have
 *	to be popped by the caller.
=======
 *	Allocates memory on the stack for the compiled local variables, the
 *	caller is responsible for freeing them. Initialises all variables. May
 *	invoke various name resolvers in order to determine which variables
 *	are being referenced at runtime.
>>>>>>> upstream/master
 *
 *----------------------------------------------------------------------
 */

<<<<<<< HEAD
int
TclPushProcCallFrame(
    ClientData clientData,	/* Record describing procedure to be
				 * interpreted. */
    register Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    int objc,			/* Count of number of arguments to this
				 * procedure. */
    Tcl_Obj *const objv[],	/* Argument value objects. */
    int isLambda)		/* 1 if this is a call by ApplyObjCmd: it
				 * needs special rules for error msg */
{
    Proc *procPtr = clientData;
    Namespace *nsPtr = procPtr->cmdPtr->nsPtr;
    CallFrame *framePtr, **framePtrPtr;
    int result;
    ByteCode *codePtr;

    /*
     * If necessary (i.e. if we haven't got a suitable compilation already
     * cached) compile the procedure's body. The compiler will allocate frame
     * slots for the procedure's non-argument local variables. Note that
     * compiling the body might increase procPtr->numCompiledLocals if new
     * local variables are found while compiling.
     */

    if (procPtr->bodyPtr->typePtr == &tclByteCodeType) {
	Interp *iPtr = (Interp *) interp;

	/*
	 * When we've got bytecode, this is the check for validity. That is,
	 * the bytecode must be for the right interpreter (no cross-leaks!),
	 * the code must be from the current epoch (so subcommand compilation
	 * is up-to-date), the namespace must match (so variable handling
	 * is right) and the resolverEpoch must match (so that new shadowed
	 * commands and/or resolver changes are considered).
	 */

	codePtr = procPtr->bodyPtr->internalRep.twoPtrValue.ptr1;
	if (((Interp *) *codePtr->interpHandle != iPtr)
		|| (codePtr->compileEpoch != iPtr->compileEpoch)
		|| (codePtr->nsPtr != nsPtr)
		|| (codePtr->nsEpoch != nsPtr->resolverEpoch)) {
	    goto doCompilation;
	}
    } else {
    doCompilation:
	result = TclProcCompileProc(interp, procPtr, procPtr->bodyPtr, nsPtr,
		(isLambda ? "body of lambda term" : "body of proc"),
		TclGetString(objv[isLambda]));
	if (result != TCL_OK) {
	    return result;
	}
    }

    /*
     * Set up and push a new call frame for the new procedure invocation.
     * This call frame will execute in the proc's namespace, which might be
     * different than the current namespace. The proc's namespace is that of
     * its command, which can change if the command is renamed from one
     * namespace to another.
     */

    framePtrPtr = &framePtr;
    (void) TclPushStackFrame(interp, (Tcl_CallFrame **) framePtrPtr,
	    (Tcl_Namespace *) nsPtr,
	    (isLambda? (FRAME_IS_PROC|FRAME_IS_LAMBDA) : FRAME_IS_PROC));

    framePtr->objc = objc;
    framePtr->objv = objv;
    framePtr->procPtr = procPtr;

    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * TclObjInterpProc --
 *
 *	When a Tcl procedure gets invoked during bytecode evaluation, this
 *	object-based routine gets invoked to interpret the procedure.
 *
 * Results:
 *	A standard Tcl object result value.
 *
 * Side effects:
 *	Depends on the commands in the procedure.
 *
 *----------------------------------------------------------------------
 */
=======
static int
InitArgsAndLocals(
    register Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    Tcl_Obj *procNameObj,	/* Procedure name for error reporting. */
    int skip)			/* Number of initial arguments to be skipped,
				 * i.e., words in the "command name". */
{
    CallFrame *framePtr = ((Interp *)interp)->varFramePtr;
    register Proc *procPtr = framePtr->procPtr;
    ByteCode *codePtr = procPtr->bodyPtr->internalRep.twoPtrValue.ptr1;
    register Var *varPtr, *defPtr;
    int localCt = procPtr->numCompiledLocals, numArgs, argCt, i, imax;
    Tcl_Obj *const *argObjs;

    /*
     * Make sure that the local cache of variable names and initial values has
     * been initialised properly .
     */

    if (localCt) {
	if (!codePtr->localCachePtr) {
	    InitLocalCache(procPtr) ;
	}
	framePtr->localCachePtr = codePtr->localCachePtr;
	framePtr->localCachePtr->refCount++;
	defPtr = (Var *) (&framePtr->localCachePtr->varName0 + localCt);
    } else {
	defPtr = NULL;
    }

    /*
     * Create the "compiledLocals" array. Make sure it is large enough to hold
     * all the procedure's compiled local variables, including its formal
     * parameters.
     */

    varPtr = TclStackAlloc(interp, (int)(localCt * sizeof(Var)));
    framePtr->compiledLocals = varPtr;
    framePtr->numCompiledLocals = localCt;

    /*
     * Match and assign the call's actual parameters to the procedure's formal
     * arguments. The formal arguments are described by the first numArgs
     * entries in both the Proc structure's local variable list and the call
     * frame's local variable array.
     */

    numArgs = procPtr->numArgs;
    argCt = framePtr->objc - skip;	/* Set it to the number of args to the
					 * procedure. */
    argObjs = framePtr->objv + skip;
    if (numArgs == 0) {
	if (argCt) {
	    goto incorrectArgs;
	} else {
	    goto correctArgs;
	}
    }
    imax = ((argCt < numArgs-1) ? argCt : numArgs-1);
    for (i = 0; i < imax; i++, varPtr++, defPtr ? defPtr++ : defPtr) {
	/*
	 * "Normal" arguments; last formal is special, depends on it being
	 * 'args'.
	 */

	Tcl_Obj *objPtr = argObjs[i];

	varPtr->flags = 0;
	varPtr->value.objPtr = objPtr;
	Tcl_IncrRefCount(objPtr);	/* Local var is a reference. */
    }
    for (; i < numArgs-1; i++, varPtr++, defPtr ? defPtr++ : defPtr) {
	/*
	 * This loop is entered if argCt < (numArgs-1). Set default values;
	 * last formal is special.
	 */

	Tcl_Obj *objPtr = defPtr ? defPtr->value.objPtr : NULL;

	if (!objPtr) {
	    goto incorrectArgs;
	}
	varPtr->flags = 0;
	varPtr->value.objPtr = objPtr;
	Tcl_IncrRefCount(objPtr);	/* Local var reference. */
    }

    /*
     * When we get here, the last formal argument remains to be defined:
     * defPtr and varPtr point to the last argument to be initialized.
     */

    varPtr->flags = 0;
    if (defPtr && defPtr->flags & VAR_IS_ARGS) {
	Tcl_Obj *listPtr = Tcl_NewListObj(argCt-i, argObjs+i);

	varPtr->value.objPtr = listPtr;
	Tcl_IncrRefCount(listPtr);	/* Local var is a reference. */
    } else if (argCt == numArgs) {
	Tcl_Obj *objPtr = argObjs[i];

	varPtr->value.objPtr = objPtr;
	Tcl_IncrRefCount(objPtr);	/* Local var is a reference. */
    } else if ((argCt < numArgs) && defPtr && defPtr->value.objPtr) {
	Tcl_Obj *objPtr = defPtr->value.objPtr;

	varPtr->value.objPtr = objPtr;
	Tcl_IncrRefCount(objPtr);	/* Local var is a reference. */
    } else {
	goto incorrectArgs;
    }
    varPtr++;

    /*
     * Initialise and resolve the remaining compiledLocals. In the absence of
     * resolvers, they are undefined local vars: (flags=0, value=NULL).
     */

  correctArgs:
    if (numArgs < localCt) {
	if (!framePtr->nsPtr->compiledVarResProc
		&& !((Interp *)interp)->resolverPtr) {
	    memset(varPtr, 0, (localCt - numArgs)*sizeof(Var));
	} else {
	    InitResolvedLocals(interp, codePtr, varPtr, framePtr->nsPtr);
	}
    }

    return TCL_OK;
>>>>>>> upstream/master

int
TclObjInterpProc(
    ClientData clientData,	/* Record describing procedure to be
				 * interpreted. */
    register Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    int objc,			/* Count of number of arguments to this
				 * procedure. */
    Tcl_Obj *const objv[])	/* Argument value objects. */
{
    /*
<<<<<<< HEAD
     * Not used much in the core; external interface for iTcl
     */

    return Tcl_NRCallObjProc(interp, TclNRInterpProc, clientData, objc, objv);
}

int
TclNRInterpProc(
=======
     * Initialise all compiled locals to avoid problems at DeleteLocalVars.
     */

  incorrectArgs:
    if ((skip != 1) &&
	    TclInitRewriteEnsemble(interp, skip-1, 0, framePtr->objv)) {
	TclNRAddCallback(interp, TclClearRootEnsemble, NULL, NULL, NULL, NULL);
    }
    memset(varPtr, 0,
	    ((framePtr->compiledLocals + localCt)-varPtr) * sizeof(Var));
    return ProcWrongNumArgs(interp, skip);
}

/*
 *----------------------------------------------------------------------
 *
 * TclPushProcCallFrame --
 *
 *	Compiles a proc body if necessary, then pushes a CallFrame suitable
 *	for executing it.
 *
 * Results:
 *	A standard Tcl object result value.
 *
 * Side effects:
 *	The proc's body may be recompiled. A CallFrame is pushed, it will have
 *	to be popped by the caller.
 *
 *----------------------------------------------------------------------
 */

int
TclPushProcCallFrame(
>>>>>>> upstream/master
    ClientData clientData,	/* Record describing procedure to be
				 * interpreted. */
    register Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    int objc,			/* Count of number of arguments to this
				 * procedure. */
<<<<<<< HEAD
    Tcl_Obj *const objv[])	/* Argument value objects. */
{
    int result = TclPushProcCallFrame(clientData, interp, objc, objv,
	    /*isLambda*/ 0);

    if (result != TCL_OK) {
	return TCL_ERROR;
    }
    return TclNRInterpProcCore(interp, objv[0], 1, &MakeProcError);
}

/*
 *----------------------------------------------------------------------
 *
 * TclNRInterpProcCore --
 *
 *	When a Tcl procedure, lambda term or anything else that works like a
 *	procedure gets invoked during bytecode evaluation, this object-based
 *	routine gets invoked to interpret the body.
 *
 * Results:
 *	A standard Tcl object result value.
 *
 * Side effects:
 *	Nearly anything; depends on the commands in the procedure body.
 *
 *----------------------------------------------------------------------
 */

int
TclNRInterpProcCore(
    register Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    Tcl_Obj *procNameObj,	/* Procedure name for error reporting. */
    int skip,			/* Number of initial arguments to be skipped,
				 * i.e., words in the "command name". */
    ProcErrorProc *errorProc)	/* How to convert results from the script into
				 * results of the overall procedure. */
{
    Interp *iPtr = (Interp *) interp;
    register Proc *procPtr = iPtr->varFramePtr->procPtr;
    int result;
    CallFrame *freePtr;
    ByteCode *codePtr;

    result = InitArgsAndLocals(interp, procNameObj, skip);
    if (result != TCL_OK) {
	freePtr = iPtr->framePtr;
	Tcl_PopCallFrame(interp);	/* Pop but do not free. */
	TclStackFree(interp, freePtr->compiledLocals);
					/* Free compiledLocals. */
	TclStackFree(interp, freePtr);	/* Free CallFrame. */
	return TCL_ERROR;
    }

#if defined(TCL_COMPILE_DEBUG)
    if (tclTraceExec >= 1) {
	register CallFrame *framePtr = iPtr->varFramePtr;
	register int i;

	if (framePtr->isProcCallFrame & FRAME_IS_LAMBDA) {
	    fprintf(stdout, "Calling lambda ");
	} else {
	    fprintf(stdout, "Calling proc ");
	}
	for (i = 0; i < framePtr->objc; i++) {
	    TclPrintObject(stdout, framePtr->objv[i], 15);
	    fprintf(stdout, " ");
	}
	fprintf(stdout, "\n");
	fflush(stdout);
    }
#endif /*TCL_COMPILE_DEBUG*/

#ifdef USE_DTRACE
    if (TCL_DTRACE_PROC_ARGS_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;
	const char *a[10];
	int i;

	for (i = 0 ; i < 10 ; i++) {
	    a[i] = (l < iPtr->varFramePtr->objc ?
		    TclGetString(iPtr->varFramePtr->objv[l]) : NULL);
	    l++;
	}
	TCL_DTRACE_PROC_ARGS(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
		a[8], a[9]);
    }
    if (TCL_DTRACE_PROC_INFO_ENABLED() && iPtr->cmdFramePtr) {
	Tcl_Obj *info = TclInfoFrame(interp, iPtr->cmdFramePtr);
	const char *a[6]; int i[2];

	TclDTraceInfo(info, a, i);
	TCL_DTRACE_PROC_INFO(a[0], a[1], a[2], a[3], i[0], i[1], a[4], a[5]);
	TclDecrRefCount(info);
    }
    if (TCL_DTRACE_PROC_ENTRY_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;

	TCL_DTRACE_PROC_ENTRY(l < iPtr->varFramePtr->objc ?
		TclGetString(iPtr->varFramePtr->objv[l]) : NULL,
		iPtr->varFramePtr->objc - l - 1,
		(Tcl_Obj **)(iPtr->varFramePtr->objv + l + 1));
    }
    if (TCL_DTRACE_PROC_ENTRY_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;

	TCL_DTRACE_PROC_ENTRY(l < iPtr->varFramePtr->objc ?
		TclGetString(iPtr->varFramePtr->objv[l]) : NULL,
		iPtr->varFramePtr->objc - l - 1,
		(Tcl_Obj **)(iPtr->varFramePtr->objv + l + 1));
=======
    Tcl_Obj *const objv[],	/* Argument value objects. */
    int isLambda)		/* 1 if this is a call by ApplyObjCmd: it
				 * needs special rules for error msg */
{
    Proc *procPtr = clientData;
    Namespace *nsPtr = procPtr->cmdPtr->nsPtr;
    CallFrame *framePtr, **framePtrPtr;
    int result;
    ByteCode *codePtr;

    /*
     * If necessary (i.e. if we haven't got a suitable compilation already
     * cached) compile the procedure's body. The compiler will allocate frame
     * slots for the procedure's non-argument local variables. Note that
     * compiling the body might increase procPtr->numCompiledLocals if new
     * local variables are found while compiling.
     */

    if (procPtr->bodyPtr->typePtr == &tclByteCodeType) {
	Interp *iPtr = (Interp *) interp;

	/*
	 * When we've got bytecode, this is the check for validity. That is,
	 * the bytecode must be for the right interpreter (no cross-leaks!),
	 * the code must be from the current epoch (so subcommand compilation
	 * is up-to-date), the namespace must match (so variable handling
	 * is right) and the resolverEpoch must match (so that new shadowed
	 * commands and/or resolver changes are considered).
	 */

	codePtr = procPtr->bodyPtr->internalRep.twoPtrValue.ptr1;
	if (((Interp *) *codePtr->interpHandle != iPtr)
		|| (codePtr->compileEpoch != iPtr->compileEpoch)
		|| (codePtr->nsPtr != nsPtr)
		|| (codePtr->nsEpoch != nsPtr->resolverEpoch)) {
	    goto doCompilation;
	}
    } else {
    doCompilation:
	result = TclProcCompileProc(interp, procPtr, procPtr->bodyPtr, nsPtr,
		(isLambda ? "body of lambda term" : "body of proc"),
		TclGetString(objv[isLambda]));
	if (result != TCL_OK) {
	    return result;
	}
>>>>>>> upstream/master
    }
#endif /* USE_DTRACE */

    /*
     * Set up and push a new call frame for the new procedure invocation.
     * This call frame will execute in the proc's namespace, which might be
     * different than the current namespace. The proc's namespace is that of
     * its command, which can change if the command is renamed from one
     * namespace to another.
     */

    framePtrPtr = &framePtr;
    (void) TclPushStackFrame(interp, (Tcl_CallFrame **) framePtrPtr,
	    (Tcl_Namespace *) nsPtr,
	    (isLambda? (FRAME_IS_PROC|FRAME_IS_LAMBDA) : FRAME_IS_PROC));

    framePtr->objc = objc;
    framePtr->objv = objv;
    framePtr->procPtr = procPtr;

    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * TclObjInterpProc --
 *
 *	When a Tcl procedure gets invoked during bytecode evaluation, this
 *	object-based routine gets invoked to interpret the procedure.
 *
 * Results:
 *	A standard Tcl object result value.
 *
 * Side effects:
 *	Depends on the commands in the procedure.
 *
 *----------------------------------------------------------------------
 */

int
TclObjInterpProc(
    ClientData clientData,	/* Record describing procedure to be
				 * interpreted. */
    register Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    int objc,			/* Count of number of arguments to this
				 * procedure. */
    Tcl_Obj *const objv[])	/* Argument value objects. */
{
    /*
     * Not used much in the core; external interface for iTcl
     */
<<<<<<< HEAD

<<<<<<< HEAD
    procPtr->refCount++;
    codePtr = procPtr->bodyPtr->internalRep.twoPtrValue.ptr1;

    TclNRAddCallback(interp, InterpProcNR2, procNameObj, errorProc,
	    NULL, NULL);
    return TclNRExecuteByteCode(interp, codePtr);
}

static int
InterpProcNR2(
    ClientData data[],
    Tcl_Interp *interp,
    int result)
{
    Interp *iPtr = (Interp *) interp;
    Proc *procPtr = iPtr->varFramePtr->procPtr;
    CallFrame *freePtr;
    Tcl_Obj *procNameObj = data[0];
    ProcErrorProc *errorProc = (ProcErrorProc *)data[1];

=======
=======
    return Tcl_NRCallObjProc(interp, TclNRInterpProc, clientData, objc, objv);
}

int
TclNRInterpProc(
    ClientData clientData,	/* Record describing procedure to be
				 * interpreted. */
    register Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    int objc,			/* Count of number of arguments to this
				 * procedure. */
    Tcl_Obj *const objv[])	/* Argument value objects. */
{
    int result = TclPushProcCallFrame(clientData, interp, objc, objv,
	    /*isLambda*/ 0);

    if (result != TCL_OK) {
	return TCL_ERROR;
    }
    return TclNRInterpProcCore(interp, objv[0], 1, &MakeProcError);
}

/*
 *----------------------------------------------------------------------
 *
 * TclNRInterpProcCore --
 *
 *	When a Tcl procedure, lambda term or anything else that works like a
 *	procedure gets invoked during bytecode evaluation, this object-based
 *	routine gets invoked to interpret the body.
 *
 * Results:
 *	A standard Tcl object result value.
 *
 * Side effects:
 *	Nearly anything; depends on the commands in the procedure body.
 *
 *----------------------------------------------------------------------
 */

int
TclNRInterpProcCore(
    register Tcl_Interp *interp,/* Interpreter in which procedure was
				 * invoked. */
    Tcl_Obj *procNameObj,	/* Procedure name for error reporting. */
    int skip,			/* Number of initial arguments to be skipped,
				 * i.e., words in the "command name". */
    ProcErrorProc *errorProc)	/* How to convert results from the script into
				 * results of the overall procedure. */
{
    Interp *iPtr = (Interp *) interp;
    register Proc *procPtr = iPtr->varFramePtr->procPtr;
    int result;
    CallFrame *freePtr;
    ByteCode *codePtr;

    result = InitArgsAndLocals(interp, procNameObj, skip);
    if (result != TCL_OK) {
	freePtr = iPtr->framePtr;
	Tcl_PopCallFrame(interp);	/* Pop but do not free. */
	TclStackFree(interp, freePtr->compiledLocals);
					/* Free compiledLocals. */
	TclStackFree(interp, freePtr);	/* Free CallFrame. */
	return TCL_ERROR;
    }

#if defined(TCL_COMPILE_DEBUG)
    if (tclTraceExec >= 1) {
	register CallFrame *framePtr = iPtr->varFramePtr;
	register int i;

	if (framePtr->isProcCallFrame & FRAME_IS_LAMBDA) {
	    fprintf(stdout, "Calling lambda ");
	} else {
	    fprintf(stdout, "Calling proc ");
	}
	for (i = 0; i < framePtr->objc; i++) {
	    TclPrintObject(stdout, framePtr->objv[i], 15);
	    fprintf(stdout, " ");
	}
	fprintf(stdout, "\n");
	fflush(stdout);
    }
#endif /*TCL_COMPILE_DEBUG*/

#ifdef USE_DTRACE
    if (TCL_DTRACE_PROC_ARGS_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;
	const char *a[10];
	int i;

	for (i = 0 ; i < 10 ; i++) {
	    a[i] = (l < iPtr->varFramePtr->objc ?
		    TclGetString(iPtr->varFramePtr->objv[l]) : NULL);
	    l++;
	}
	TCL_DTRACE_PROC_ARGS(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
		a[8], a[9]);
    }
    if (TCL_DTRACE_PROC_INFO_ENABLED() && iPtr->cmdFramePtr) {
	Tcl_Obj *info = TclInfoFrame(interp, iPtr->cmdFramePtr);
	const char *a[6]; int i[2];

	TclDTraceInfo(info, a, i);
	TCL_DTRACE_PROC_INFO(a[0], a[1], a[2], a[3], i[0], i[1], a[4], a[5]);
	TclDecrRefCount(info);
    }
    if (TCL_DTRACE_PROC_ENTRY_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;

	TCL_DTRACE_PROC_ENTRY(l < iPtr->varFramePtr->objc ?
		TclGetString(iPtr->varFramePtr->objv[l]) : NULL,
		iPtr->varFramePtr->objc - l - 1,
		(Tcl_Obj **)(iPtr->varFramePtr->objv + l + 1));
    }
    if (TCL_DTRACE_PROC_ENTRY_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;

	TCL_DTRACE_PROC_ENTRY(l < iPtr->varFramePtr->objc ?
		TclGetString(iPtr->varFramePtr->objv[l]) : NULL,
		iPtr->varFramePtr->objc - l - 1,
		(Tcl_Obj **)(iPtr->varFramePtr->objv + l + 1));
    }
#endif /* USE_DTRACE */

    /*
     * Invoke the commands in the procedure's body.
     */
>>>>>>> upstream/master

    procPtr->refCount++;
    codePtr = procPtr->bodyPtr->internalRep.twoPtrValue.ptr1;

    TclNRAddCallback(interp, InterpProcNR2, procNameObj, errorProc,
	    NULL, NULL);
    return TclNRExecuteByteCode(interp, codePtr);
}

static int
InterpProcNR2(
    ClientData data[],
    Tcl_Interp *interp,
    int result)
{
    Interp *iPtr = (Interp *) interp;
    Proc *procPtr = iPtr->varFramePtr->procPtr;
    CallFrame *freePtr;
    Tcl_Obj *procNameObj = data[0];
    ProcErrorProc *errorProc = (ProcErrorProc *)data[1];

<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    if (TCL_DTRACE_PROC_RETURN_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;

	TCL_DTRACE_PROC_RETURN(l < iPtr->varFramePtr->objc ?
		TclGetString(iPtr->varFramePtr->objv[l]) : NULL, result);
    }
    if (procPtr->refCount-- <= 1) {
	TclProcCleanupProc(procPtr);
    }

    /*
     * Free the stack-allocated compiled locals and CallFrame. It is important
     * to pop the call frame without freeing it first: the compiledLocals
     * cannot be freed before the frame is popped, as the local variables must
     * be deleted. But the compiledLocals must be freed first, as they were
     * allocated later on the stack.
     */

    if (result != TCL_OK) {
	goto process;
<<<<<<< HEAD
<<<<<<< HEAD
    }
<<<<<<< HEAD
    
    done: 
=======

    done:
>>>>>>> upstream/master
    if (TCL_DTRACE_PROC_RESULT_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;
	Tcl_Obj *r = Tcl_GetObjResult(interp);

	TCL_DTRACE_PROC_RESULT(l < iPtr->varFramePtr->objc ?
		TclGetString(iPtr->varFramePtr->objv[l]) : NULL, result,
		TclGetString(r), r);
    }
=======
    }

    done:
    if (TCL_DTRACE_PROC_RESULT_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;
	Tcl_Obj *r = Tcl_GetObjResult(interp);

	TCL_DTRACE_PROC_RESULT(l < iPtr->varFramePtr->objc ?
		TclGetString(iPtr->varFramePtr->objv[l]) : NULL, result,
		TclGetString(r), r);
    }
>>>>>>> upstream/master
=======
    }

    done:
    if (TCL_DTRACE_PROC_RESULT_ENABLED()) {
	int l = iPtr->varFramePtr->isProcCallFrame & FRAME_IS_LAMBDA ? 1 : 0;
	Tcl_Obj *r = Tcl_GetObjResult(interp);

	TCL_DTRACE_PROC_RESULT(l < iPtr->varFramePtr->objc ?
		TclGetString(iPtr->varFramePtr->objv[l]) : NULL, result,
		TclGetString(r), r);
    }
>>>>>>> upstream/master

    freePtr = iPtr->framePtr;
    Tcl_PopCallFrame(interp);		/* Pop but do not free. */
    TclStackFree(interp, freePtr->compiledLocals);
					/* Free compiledLocals. */
    TclStackFree(interp, freePtr);	/* Free CallFrame. */
    return result;

    /*
     * Process any non-TCL_OK result code.
     */

    process:
    switch (result) {
    case TCL_RETURN:
	/*
	 * If it is a 'return', do the TIP#90 processing now.
	 */

	result = TclUpdateReturnInfo((Interp *) interp);
	break;

    case TCL_CONTINUE:
    case TCL_BREAK:
	/*
	 * It's an error to get to this point from a 'break' or 'continue', so
	 * transform to an error now.
	 */

	Tcl_SetObjResult(interp, Tcl_ObjPrintf(
		"invoked \"%s\" outside of a loop",
		((result == TCL_BREAK) ? "break" : "continue")));
	Tcl_SetErrorCode(interp, "TCL", "RESULT", "UNEXPECTED", NULL);
	result = TCL_ERROR;

	/*
	 * Fall through to the TCL_ERROR handling code.
	 */

    case TCL_ERROR:
	/*
	 * Now it _must_ be an error, so we need to log it as such. This means
	 * filling out the error trace. Luckily, we just hand this off to the
	 * function handed to us as an argument.
	 */

	errorProc(interp, procNameObj);
    }
    goto done;
}

/*
 *----------------------------------------------------------------------
 *
 * TclProcCompileProc --
 *
 *	Called just before a procedure is executed to compile the body to byte
 *	codes. If the type of the body is not "byte code" or if the compile
 *	conditions have changed (namespace context, epoch counters, etc.) then
 *	the body is recompiled. Otherwise, this function does nothing.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	May change the internal representation of the body object to compiled
 *	code.
 *
 *----------------------------------------------------------------------
 */

int
TclProcCompileProc(
    Tcl_Interp *interp,		/* Interpreter containing procedure. */
    Proc *procPtr,		/* Data associated with procedure. */
    Tcl_Obj *bodyPtr,		/* Body of proc. (Usually procPtr->bodyPtr,
				 * but could be any code fragment compiled in
				 * the context of this procedure.) */
    Namespace *nsPtr,		/* Namespace containing procedure. */
    const char *description,	/* string describing this body of code. */
    const char *procName)	/* Name of this procedure. */
{
    Interp *iPtr = (Interp *) interp;
    Tcl_CallFrame *framePtr;
    ByteCode *codePtr = bodyPtr->internalRep.twoPtrValue.ptr1;

    /*
     * If necessary, compile the procedure's body. The compiler will allocate
     * frame slots for the procedure's non-argument local variables. If the
     * ByteCode already exists, make sure it hasn't been invalidated by
     * someone redefining a core command (this might make the compiled code
     * wrong). Also, if the code was compiled in/for a different interpreter,
     * we recompile it. Note that compiling the body might increase
     * procPtr->numCompiledLocals if new local variables are found while
     * compiling.
     *
     * Precompiled procedure bodies, however, are immutable and therefore they
     * are not recompiled, even if things have changed.
     */

    if (bodyPtr->typePtr == &tclByteCodeType) {
	if (((Interp *) *codePtr->interpHandle == iPtr)
		&& (codePtr->compileEpoch == iPtr->compileEpoch)
		&& (codePtr->nsPtr == nsPtr)
		&& (codePtr->nsEpoch == nsPtr->resolverEpoch)) {
	    return TCL_OK;
	}

	if (codePtr->flags & TCL_BYTECODE_PRECOMPILED) {
	    if ((Interp *) *codePtr->interpHandle != iPtr) {
		Tcl_SetObjResult(interp, Tcl_NewStringObj(
			"a precompiled script jumped interps", -1));
		Tcl_SetErrorCode(interp, "TCL", "OPERATION", "PROC",
			"CROSSINTERPBYTECODE", NULL);
		return TCL_ERROR;
	    }
	    codePtr->compileEpoch = iPtr->compileEpoch;
	    codePtr->nsPtr = nsPtr;
	} else {
	    TclFreeIntRep(bodyPtr);
	}
    }

    if (bodyPtr->typePtr != &tclByteCodeType) {
	Tcl_HashEntry *hePtr;

#ifdef TCL_COMPILE_DEBUG
	if (tclTraceCompile >= 1) {
	    /*
	     * Display a line summarizing the top level command we are about
	     * to compile.
	     */

	    Tcl_Obj *message;

	    TclNewLiteralStringObj(message, "Compiling ");
	    Tcl_IncrRefCount(message);
	    Tcl_AppendStringsToObj(message, description, " \"", NULL);
	    Tcl_AppendLimitedToObj(message, procName, -1, 50, NULL);
	    fprintf(stdout, "%s\"\n", TclGetString(message));
	    Tcl_DecrRefCount(message);
	}
#endif

<<<<<<< HEAD
	/*
	 * Plug the current procPtr into the interpreter and coerce the code
	 * body to byte codes. The interpreter needs to know which proc it's
	 * compiling so that it can access its list of compiled locals.
	 *
	 * TRICKY NOTE: Be careful to push a call frame with the proper
	 *   namespace context, so that the byte codes are compiled in the
	 *   appropriate class context.
	 */

	iPtr->compiledProcPtr = procPtr;

	if (procPtr->numCompiledLocals > procPtr->numArgs) {
	    CompiledLocal *clPtr = procPtr->firstLocalPtr;
	    CompiledLocal *lastPtr = NULL;
	    int i, numArgs = procPtr->numArgs;

	    for (i = 0; i < numArgs; i++) {
		lastPtr = clPtr;
		clPtr = clPtr->nextPtr;
	    }

	    if (lastPtr) {
		lastPtr->nextPtr = NULL;
	    } else {
		procPtr->firstLocalPtr = NULL;
	    }
	    procPtr->lastLocalPtr = lastPtr;
	    while (clPtr) {
		CompiledLocal *toFree = clPtr;

		clPtr = clPtr->nextPtr;
		if (toFree->resolveInfo) {
		    if (toFree->resolveInfo->deleteProc) {
			toFree->resolveInfo->deleteProc(toFree->resolveInfo);
		    } else {
			ckfree(toFree->resolveInfo);
		    }
		}
		ckfree(toFree);
	    }
	    procPtr->numCompiledLocals = procPtr->numArgs;
	}

	(void) TclPushStackFrame(interp, &framePtr, (Tcl_Namespace *) nsPtr,
		/* isProcCallFrame */ 0);

	/*
	 * TIP #280: We get the invoking context from the cmdFrame which
	 * was saved by 'Tcl_ProcObjCmd' (using linePBodyPtr).
	 */

	hePtr = Tcl_FindHashEntry(iPtr->linePBodyPtr, (char *) procPtr);

	/*
	 * Constructed saved frame has body as word 0. See Tcl_ProcObjCmd.
	 */

	iPtr->invokeWord = 0;
	iPtr->invokeCmdFramePtr = (hePtr ? Tcl_GetHashValue(hePtr) : NULL);
	TclSetByteCodeFromAny(interp, bodyPtr, NULL, NULL);
	iPtr->invokeCmdFramePtr = NULL;
	TclPopStackFrame(interp);
    } else if (codePtr->nsEpoch != nsPtr->resolverEpoch) {
	/*
	 * The resolver epoch has changed, but we only need to invalidate the
	 * resolver cache.
	 */

=======
	/*
	 * Plug the current procPtr into the interpreter and coerce the code
	 * body to byte codes. The interpreter needs to know which proc it's
	 * compiling so that it can access its list of compiled locals.
	 *
	 * TRICKY NOTE: Be careful to push a call frame with the proper
	 *   namespace context, so that the byte codes are compiled in the
	 *   appropriate class context.
	 */

	iPtr->compiledProcPtr = procPtr;

	if (procPtr->numCompiledLocals > procPtr->numArgs) {
	    CompiledLocal *clPtr = procPtr->firstLocalPtr;
	    CompiledLocal *lastPtr = NULL;
	    int i, numArgs = procPtr->numArgs;

	    for (i = 0; i < numArgs; i++) {
		lastPtr = clPtr;
		clPtr = clPtr->nextPtr;
	    }

	    if (lastPtr) {
		lastPtr->nextPtr = NULL;
	    } else {
		procPtr->firstLocalPtr = NULL;
	    }
	    procPtr->lastLocalPtr = lastPtr;
	    while (clPtr) {
		CompiledLocal *toFree = clPtr;

		clPtr = clPtr->nextPtr;
		if (toFree->resolveInfo) {
		    if (toFree->resolveInfo->deleteProc) {
			toFree->resolveInfo->deleteProc(toFree->resolveInfo);
		    } else {
			ckfree(toFree->resolveInfo);
		    }
		}
		ckfree(toFree);
	    }
	    procPtr->numCompiledLocals = procPtr->numArgs;
	}

	(void) TclPushStackFrame(interp, &framePtr, (Tcl_Namespace *) nsPtr,
		/* isProcCallFrame */ 0);

	/*
	 * TIP #280: We get the invoking context from the cmdFrame which
	 * was saved by 'Tcl_ProcObjCmd' (using linePBodyPtr).
	 */

	hePtr = Tcl_FindHashEntry(iPtr->linePBodyPtr, (char *) procPtr);

	/*
	 * Constructed saved frame has body as word 0. See Tcl_ProcObjCmd.
	 */

	iPtr->invokeWord = 0;
	iPtr->invokeCmdFramePtr = (hePtr ? Tcl_GetHashValue(hePtr) : NULL);
	TclSetByteCodeFromAny(interp, bodyPtr, NULL, NULL);
	iPtr->invokeCmdFramePtr = NULL;
	TclPopStackFrame(interp);
    } else if (codePtr->nsEpoch != nsPtr->resolverEpoch) {
	/*
	 * The resolver epoch has changed, but we only need to invalidate the
	 * resolver cache.
	 */

>>>>>>> upstream/master
	codePtr->nsEpoch = nsPtr->resolverEpoch;
	codePtr->flags |= TCL_BYTECODE_RESOLVE_VARS;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * MakeProcError --
 *
 *	Function called by TclObjInterpProc to create the stack information
 *	upon an error from a procedure.
 *
 * Results:
 *	The interpreter's error info trace is set to a value that supplements
 *	the error code.
 *
 * Side effects:
 *	none.
 *
 *----------------------------------------------------------------------
 */

static void
MakeProcError(
    Tcl_Interp *interp,		/* The interpreter in which the procedure was
				 * called. */
    Tcl_Obj *procNameObj)	/* Name of the procedure. Used for error
				 * messages and trace information. */
{
    int overflow, limit = 60, nameLen;
<<<<<<< HEAD
    const char *procName = Tcl_GetStringFromObj(procNameObj, &nameLen);
=======
    const char *procName = TclGetStringFromObj(procNameObj, &nameLen);
>>>>>>> upstream/master

    overflow = (nameLen > limit);
    Tcl_AppendObjToErrorInfo(interp, Tcl_ObjPrintf(
	    "\n    (procedure \"%.*s%s\" line %d)",
	    (overflow ? limit : nameLen), procName,
	    (overflow ? "..." : ""), Tcl_GetErrorLine(interp)));
}

/*
 *----------------------------------------------------------------------
 *
 * TclProcDeleteProc --
 *
 *	This function is invoked just before a command procedure is removed
 *	from an interpreter. Its job is to release all the resources allocated
 *	to the procedure.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Memory gets freed, unless the procedure is actively being executed.
 *	In this case the cleanup is delayed until the last call to the current
 *	procedure completes.
 *
 *----------------------------------------------------------------------
 */

void
TclProcDeleteProc(
    ClientData clientData)	/* Procedure to be deleted. */
{
    Proc *procPtr = clientData;

    if (procPtr->refCount-- <= 1) {
	TclProcCleanupProc(procPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * TclProcCleanupProc --
 *
 *	This function does all the real work of freeing up a Proc structure.
 *	It's called only when the structure's reference count becomes zero.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Memory gets freed.
 *
 *----------------------------------------------------------------------
 */

void
TclProcCleanupProc(
    register Proc *procPtr)	/* Procedure to be deleted. */
{
    register CompiledLocal *localPtr;
    Tcl_Obj *bodyPtr = procPtr->bodyPtr;
    Tcl_Obj *defPtr;
    Tcl_ResolvedVarInfo *resVarInfo;
    Tcl_HashEntry *hePtr = NULL;
    CmdFrame *cfPtr = NULL;
    Interp *iPtr = procPtr->iPtr;

    if (bodyPtr != NULL) {
	Tcl_DecrRefCount(bodyPtr);
    }
    for (localPtr = procPtr->firstLocalPtr; localPtr != NULL; ) {
	CompiledLocal *nextPtr = localPtr->nextPtr;

	resVarInfo = localPtr->resolveInfo;
	if (resVarInfo) {
	    if (resVarInfo->deleteProc) {
		resVarInfo->deleteProc(resVarInfo);
	    } else {
		ckfree(resVarInfo);
	    }
	}

	if (localPtr->defValuePtr != NULL) {
	    defPtr = localPtr->defValuePtr;
	    Tcl_DecrRefCount(defPtr);
	}
	ckfree(localPtr);
	localPtr = nextPtr;
    }
    ckfree(procPtr);

    /*
     * TIP #280: Release the location data associated with this Proc
     * structure, if any. The interpreter may not exist (For example for
     * procbody structures created by tbcload.
     */

    if (iPtr == NULL) {
	return;
    }

    hePtr = Tcl_FindHashEntry(iPtr->linePBodyPtr, (char *) procPtr);
    if (!hePtr) {
	return;
    }

    cfPtr = Tcl_GetHashValue(hePtr);

    if (cfPtr) {
	if (cfPtr->type == TCL_LOCATION_SOURCE) {
	    Tcl_DecrRefCount(cfPtr->data.eval.path);
	    cfPtr->data.eval.path = NULL;
	}
	ckfree(cfPtr->line);
	cfPtr->line = NULL;
	ckfree(cfPtr);
    }
    Tcl_DeleteHashEntry(hePtr);
}

/*
 *----------------------------------------------------------------------
 *
 * TclUpdateReturnInfo --
 *
 *	This function is called when procedures return, and at other points
 *	where the TCL_RETURN code is used. It examines the returnLevel and
 *	returnCode to determine the real return status.
 *
 * Results:
 *	The return value is the true completion code to use for the procedure
 *	or script, instead of TCL_RETURN.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
TclUpdateReturnInfo(
    Interp *iPtr)		/* Interpreter for which TCL_RETURN exception
				 * is being processed. */
{
    int code = TCL_RETURN;

    iPtr->returnLevel--;
    if (iPtr->returnLevel < 0) {
	Tcl_Panic("TclUpdateReturnInfo: negative return level");
    }
    if (iPtr->returnLevel == 0) {
	/*
	 * Now we've reached the level to return the requested -code.
	 * Since iPtr->returnLevel and iPtr->returnCode have completed
	 * their task, we now reset them to default values so that any
	 * bare "return TCL_RETURN" that may follow will work [Bug 2152286].
	 */

	code = iPtr->returnCode;
	iPtr->returnLevel = 1;
	iPtr->returnCode = TCL_OK;
	if (code == TCL_ERROR) {
	    iPtr->flags |= ERR_LEGACY_COPY;
	}
    }
    return code;
}

/*
 *----------------------------------------------------------------------
 *
 * TclGetObjInterpProc --
 *
 *	Returns a pointer to the TclObjInterpProc function; this is different
 *	from the value obtained from the TclObjInterpProc reference on systems
 *	like Windows where import and export versions of a function exported
 *	by a DLL exist.
 *
 * Results:
 *	Returns the internal address of the TclObjInterpProc function.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

TclObjCmdProcType
TclGetObjInterpProc(void)
{
    return (TclObjCmdProcType) TclObjInterpProc;
}

/*
 *----------------------------------------------------------------------
 *
 * TclNewProcBodyObj --
 *
 *	Creates a new object, of type "procbody", whose internal
 *	representation is the given Proc struct. The newly created object's
 *	reference count is 0.
 *
 * Results:
 *	Returns a pointer to a newly allocated Tcl_Obj, NULL on error.
 *
 * Side effects:
 *	The reference count in the ByteCode attached to the Proc is bumped up
 *	by one, since the internal rep stores a pointer to it.
 *
 *----------------------------------------------------------------------
 */

Tcl_Obj *
TclNewProcBodyObj(
    Proc *procPtr)		/* the Proc struct to store as the internal
				 * representation. */
{
    Tcl_Obj *objPtr;

    if (!procPtr) {
	return NULL;
    }

    TclNewObj(objPtr);
    if (objPtr) {
	objPtr->typePtr = &tclProcBodyType;
	objPtr->internalRep.twoPtrValue.ptr1 = procPtr;

	procPtr->refCount++;
    }

    return objPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * ProcBodyDup --
 *
 *	Tcl_ObjType's Dup function for the proc body object. Bumps the
 *	reference count on the Proc stored in the internal representation.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Sets up the object in dupPtr to be a duplicate of the one in srcPtr.
 *
 *----------------------------------------------------------------------
 */

static void
ProcBodyDup(
    Tcl_Obj *srcPtr,		/* Object to copy. */
    Tcl_Obj *dupPtr)		/* Target object for the duplication. */
{
    Proc *procPtr = srcPtr->internalRep.twoPtrValue.ptr1;

    dupPtr->typePtr = &tclProcBodyType;
    dupPtr->internalRep.twoPtrValue.ptr1 = procPtr;
    procPtr->refCount++;
}

/*
 *----------------------------------------------------------------------
 *
 * ProcBodyFree --
 *
 *	Tcl_ObjType's Free function for the proc body object. The reference
 *	count on its Proc struct is decreased by 1; if the count reaches 0,
 *	the proc is freed.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If the reference count on the Proc struct reaches 0, the struct is
 *	freed.
<<<<<<< HEAD
 *
 *----------------------------------------------------------------------
 */

static void
ProcBodyFree(
    Tcl_Obj *objPtr)		/* The object to clean up. */
{
    Proc *procPtr = objPtr->internalRep.twoPtrValue.ptr1;

    if (procPtr->refCount-- <= 1) {
	TclProcCleanupProc(procPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * DupLambdaInternalRep, FreeLambdaInternalRep, SetLambdaFromAny --
 *
 *	How to manage the internal representations of lambda term objects.
 *	Syntactically they look like a two- or three-element list, where the
 *	first element is the formal arguments, the second is the the body, and
 *	the (optional) third is the namespace to execute the lambda term
 *	within (the global namespace is assumed if it is absent).
=======
>>>>>>> upstream/master
 *
 *----------------------------------------------------------------------
 */

static void
<<<<<<< HEAD
DupLambdaInternalRep(
    Tcl_Obj *srcPtr,		/* Object with internal rep to copy. */
    register Tcl_Obj *copyPtr)	/* Object with internal rep to set. */
{
    Proc *procPtr = srcPtr->internalRep.twoPtrValue.ptr1;
    Tcl_Obj *nsObjPtr = srcPtr->internalRep.twoPtrValue.ptr2;

    copyPtr->internalRep.twoPtrValue.ptr1 = procPtr;
    copyPtr->internalRep.twoPtrValue.ptr2 = nsObjPtr;

    procPtr->refCount++;
    Tcl_IncrRefCount(nsObjPtr);
    copyPtr->typePtr = &tclLambdaType;
}

static void
FreeLambdaInternalRep(
    register Tcl_Obj *objPtr)	/* CmdName object with internal representation
				 * to free. */
<<<<<<< HEAD
{
    Proc *procPtr = objPtr->internalRep.twoPtrValue.ptr1;
    Tcl_Obj *nsObjPtr = objPtr->internalRep.twoPtrValue.ptr2;

    if (procPtr->refCount-- == 1) {
	TclProcCleanupProc(procPtr);
    }
    TclDecrRefCount(nsObjPtr);
    objPtr->typePtr = NULL;
}

static int
SetLambdaFromAny(
    Tcl_Interp *interp,		/* Used for error reporting if not NULL. */
    register Tcl_Obj *objPtr)	/* The object to convert. */
{
    Interp *iPtr = (Interp *) interp;
    const char *name;
    Tcl_Obj *argsPtr, *bodyPtr, *nsObjPtr, **objv;
    int isNew, objc, result;
    CmdFrame *cfPtr = NULL;
    Proc *procPtr;

    if (interp == NULL) {
	return TCL_ERROR;
    }

    /*
     * Convert objPtr to list type first; if it cannot be converted, or if its
     * length is not 2, then it cannot be converted to tclLambdaType.
     */

    result = TclListObjGetElements(NULL, objPtr, &objc, &objv);
    if ((result != TCL_OK) || ((objc != 2) && (objc != 3))) {
	Tcl_SetObjResult(interp, Tcl_ObjPrintf(
		"can't interpret \"%s\" as a lambda expression",
		Tcl_GetString(objPtr)));
	Tcl_SetErrorCode(interp, "TCL", "VALUE", "LAMBDA", NULL);
	return TCL_ERROR;
    }

    argsPtr = objv[0];
    bodyPtr = objv[1];

    /*
     * Create and initialize the Proc struct. The cmdPtr field is set to NULL
     * to signal that this is an anonymous function.
     */

    name = TclGetString(objPtr);

    if (TclCreateProc(interp, /*ignored nsPtr*/ NULL, name, argsPtr, bodyPtr,
	    &procPtr) != TCL_OK) {
	Tcl_AppendObjToErrorInfo(interp, Tcl_ObjPrintf(
		"\n    (parsing lambda expression \"%s\")", name));
	return TCL_ERROR;
    }

    /*
     * CAREFUL: TclCreateProc returns refCount==1! [Bug 1578454]
     * procPtr->refCount = 1;
     */

    procPtr->cmdPtr = NULL;

    /*
     * TIP #280: Remember the line the apply body is starting on. In a Byte
     * code context we ask the engine to provide us with the necessary
     * information. This is for the initialization of the byte code compiler
     * when the body is used for the first time.
     *
     * NOTE: The body is the second word in the 'objPtr'. Its location,
     * accessible through 'context.line[1]' (see below) is therefore only the
     * first approximation of the actual line the body is on. We have to use
     * the string rep of the 'objPtr' to determine the exact line. This is
     * available already through 'name'. Use 'TclListLines', see 'switch'
     * (tclCmdMZ.c).
     *
     * This code is nearly identical to the #280 code in Tcl_ProcObjCmd, see
     * this file. The differences are the different index of the body in the
     * line array of the context, and the special processing mentioned in the
     * previous paragraph to track into the list. Find a way to factor the
     * common elements into a single function.
     */

    if (iPtr->cmdFramePtr) {
	CmdFrame *contextPtr = TclStackAlloc(interp, sizeof(CmdFrame));

	*contextPtr = *iPtr->cmdFramePtr;
	if (contextPtr->type == TCL_LOCATION_BC) {
	    /*
	     * Retrieve the source context from the bytecode. This call
	     * accounts for the reference to the source file, if any, held in
	     * 'context.data.eval.path'.
	     */

	    TclGetSrcInfoForPc(contextPtr);
	} else if (contextPtr->type == TCL_LOCATION_SOURCE) {
	    /*
	     * We created a new reference to the source file path name when we
	     * created 'context' above. Account for the reference.
	     */

	    Tcl_IncrRefCount(contextPtr->data.eval.path);

	}

	if (contextPtr->type == TCL_LOCATION_SOURCE) {
	    /*
	     * We can record source location within a lambda only if the body
	     * was not created by substitution.
	     */

	    if (contextPtr->line
		    && (contextPtr->nline >= 2) && (contextPtr->line[1] >= 0)) {
		int buf[2];

		/*
		 * Move from approximation (line of list cmd word) to actual
		 * location (line of 2nd list element).
		 */

		cfPtr = ckalloc(sizeof(CmdFrame));
		TclListLines(objPtr, contextPtr->line[1], 2, buf, NULL);

		cfPtr->level = -1;
		cfPtr->type = contextPtr->type;
		cfPtr->line = ckalloc(sizeof(int));
		cfPtr->line[0] = buf[1];
		cfPtr->nline = 1;
		cfPtr->framePtr = NULL;
		cfPtr->nextPtr = NULL;

		cfPtr->data.eval.path = contextPtr->data.eval.path;
		Tcl_IncrRefCount(cfPtr->data.eval.path);

		cfPtr->cmd = NULL;
		cfPtr->len = 0;
	    }

	    /*
	     * 'contextPtr' is going out of scope. Release the reference that
	     * it's holding to the source file path
	     */

	    Tcl_DecrRefCount(contextPtr->data.eval.path);
	}
	TclStackFree(interp, contextPtr);
    }
    Tcl_SetHashValue(Tcl_CreateHashEntry(iPtr->linePBodyPtr, procPtr,
	    &isNew), cfPtr);

    /*
     * Set the namespace for this lambda: given by objv[2] understood as a
     * global reference, or else global per default.
     */

    if (objc == 2) {
	TclNewLiteralStringObj(nsObjPtr, "::");
    } else {
	const char *nsName = TclGetString(objv[2]);

	if ((*nsName != ':') || (*(nsName+1) != ':')) {
	    TclNewLiteralStringObj(nsObjPtr, "::");
	    Tcl_AppendObjToObj(nsObjPtr, objv[2]);
	} else {
	    nsObjPtr = objv[2];
	}
    }
=======
{
    Proc *procPtr = objPtr->internalRep.twoPtrValue.ptr1;
    Tcl_Obj *nsObjPtr = objPtr->internalRep.twoPtrValue.ptr2;

    if (procPtr->refCount-- == 1) {
	TclProcCleanupProc(procPtr);
    }
    TclDecrRefCount(nsObjPtr);
    objPtr->typePtr = NULL;
}

static int
SetLambdaFromAny(
    Tcl_Interp *interp,		/* Used for error reporting if not NULL. */
    register Tcl_Obj *objPtr)	/* The object to convert. */
{
    Interp *iPtr = (Interp *) interp;
    const char *name;
    Tcl_Obj *argsPtr, *bodyPtr, *nsObjPtr, **objv;
    int isNew, objc, result;
    CmdFrame *cfPtr = NULL;
    Proc *procPtr;

    if (interp == NULL) {
	return TCL_ERROR;
    }

    /*
     * Convert objPtr to list type first; if it cannot be converted, or if its
     * length is not 2, then it cannot be converted to tclLambdaType.
     */

    result = TclListObjGetElements(NULL, objPtr, &objc, &objv);
    if ((result != TCL_OK) || ((objc != 2) && (objc != 3))) {
	Tcl_SetObjResult(interp, Tcl_ObjPrintf(
		"can't interpret \"%s\" as a lambda expression",
		Tcl_GetString(objPtr)));
	Tcl_SetErrorCode(interp, "TCL", "VALUE", "LAMBDA", NULL);
	return TCL_ERROR;
    }

    argsPtr = objv[0];
    bodyPtr = objv[1];

    /*
     * Create and initialize the Proc struct. The cmdPtr field is set to NULL
     * to signal that this is an anonymous function.
     */

    name = TclGetString(objPtr);

    if (TclCreateProc(interp, /*ignored nsPtr*/ NULL, name, argsPtr, bodyPtr,
	    &procPtr) != TCL_OK) {
	Tcl_AppendObjToErrorInfo(interp, Tcl_ObjPrintf(
		"\n    (parsing lambda expression \"%s\")", name));
	return TCL_ERROR;
    }

    /*
     * CAREFUL: TclCreateProc returns refCount==1! [Bug 1578454]
     * procPtr->refCount = 1;
     */

    procPtr->cmdPtr = NULL;

    /*
     * TIP #280: Remember the line the apply body is starting on. In a Byte
     * code context we ask the engine to provide us with the necessary
     * information. This is for the initialization of the byte code compiler
     * when the body is used for the first time.
     *
     * NOTE: The body is the second word in the 'objPtr'. Its location,
     * accessible through 'context.line[1]' (see below) is therefore only the
     * first approximation of the actual line the body is on. We have to use
     * the string rep of the 'objPtr' to determine the exact line. This is
     * available already through 'name'. Use 'TclListLines', see 'switch'
     * (tclCmdMZ.c).
     *
     * This code is nearly identical to the #280 code in Tcl_ProcObjCmd, see
     * this file. The differences are the different index of the body in the
     * line array of the context, and the special processing mentioned in the
     * previous paragraph to track into the list. Find a way to factor the
     * common elements into a single function.
     */

    if (iPtr->cmdFramePtr) {
	CmdFrame *contextPtr = TclStackAlloc(interp, sizeof(CmdFrame));

	*contextPtr = *iPtr->cmdFramePtr;
	if (contextPtr->type == TCL_LOCATION_BC) {
	    /*
	     * Retrieve the source context from the bytecode. This call
	     * accounts for the reference to the source file, if any, held in
	     * 'context.data.eval.path'.
	     */

	    TclGetSrcInfoForPc(contextPtr);
	} else if (contextPtr->type == TCL_LOCATION_SOURCE) {
	    /*
	     * We created a new reference to the source file path name when we
	     * created 'context' above. Account for the reference.
	     */

	    Tcl_IncrRefCount(contextPtr->data.eval.path);

	}

	if (contextPtr->type == TCL_LOCATION_SOURCE) {
	    /*
	     * We can record source location within a lambda only if the body
	     * was not created by substitution.
	     */

	    if (contextPtr->line
		    && (contextPtr->nline >= 2) && (contextPtr->line[1] >= 0)) {
		int buf[2];

		/*
		 * Move from approximation (line of list cmd word) to actual
		 * location (line of 2nd list element).
		 */

		cfPtr = ckalloc(sizeof(CmdFrame));
		TclListLines(objPtr, contextPtr->line[1], 2, buf, NULL);

		cfPtr->level = -1;
		cfPtr->type = contextPtr->type;
		cfPtr->line = ckalloc(sizeof(int));
		cfPtr->line[0] = buf[1];
		cfPtr->nline = 1;
		cfPtr->framePtr = NULL;
		cfPtr->nextPtr = NULL;

		cfPtr->data.eval.path = contextPtr->data.eval.path;
		Tcl_IncrRefCount(cfPtr->data.eval.path);

		cfPtr->cmd = NULL;
		cfPtr->len = 0;
	    }

	    /*
	     * 'contextPtr' is going out of scope. Release the reference that
	     * it's holding to the source file path
	     */

	    Tcl_DecrRefCount(contextPtr->data.eval.path);
	}
	TclStackFree(interp, contextPtr);
    }
    Tcl_SetHashValue(Tcl_CreateHashEntry(iPtr->linePBodyPtr, procPtr,
	    &isNew), cfPtr);

    /*
     * Set the namespace for this lambda: given by objv[2] understood as a
     * global reference, or else global per default.
     */

    if (objc == 2) {
	TclNewLiteralStringObj(nsObjPtr, "::");
    } else {
	const char *nsName = TclGetString(objv[2]);

	if ((*nsName != ':') || (*(nsName+1) != ':')) {
	    TclNewLiteralStringObj(nsObjPtr, "::");
	    Tcl_AppendObjToObj(nsObjPtr, objv[2]);
	} else {
	    nsObjPtr = objv[2];
	}
=======
ProcBodyFree(
    Tcl_Obj *objPtr)		/* The object to clean up. */
{
    Proc *procPtr = objPtr->internalRep.twoPtrValue.ptr1;

    if (procPtr->refCount-- <= 1) {
	TclProcCleanupProc(procPtr);
>>>>>>> upstream/master
    }
>>>>>>> upstream/master

    Tcl_IncrRefCount(nsObjPtr);

    /*
     * Free the list internalrep of objPtr - this will free argsPtr, but
     * bodyPtr retains a reference from the Proc structure. Then finish the
     * conversion to tclLambdaType.
     */

    TclFreeIntRep(objPtr);

    objPtr->internalRep.twoPtrValue.ptr1 = procPtr;
    objPtr->internalRep.twoPtrValue.ptr2 = nsObjPtr;
    objPtr->typePtr = &tclLambdaType;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
<<<<<<< HEAD
 * Tcl_ApplyObjCmd --
 *
=======
 * DupLambdaInternalRep, FreeLambdaInternalRep, SetLambdaFromAny --
 *
 *	How to manage the internal representations of lambda term objects.
 *	Syntactically they look like a two- or three-element list, where the
 *	first element is the formal arguments, the second is the the body, and
 *	the (optional) third is the namespace to execute the lambda term
 *	within (the global namespace is assumed if it is absent).
 *
 *----------------------------------------------------------------------
 */

static void
DupLambdaInternalRep(
    Tcl_Obj *srcPtr,		/* Object with internal rep to copy. */
    register Tcl_Obj *copyPtr)	/* Object with internal rep to set. */
{
    Proc *procPtr = srcPtr->internalRep.twoPtrValue.ptr1;
    Tcl_Obj *nsObjPtr = srcPtr->internalRep.twoPtrValue.ptr2;

    copyPtr->internalRep.twoPtrValue.ptr1 = procPtr;
    copyPtr->internalRep.twoPtrValue.ptr2 = nsObjPtr;

    procPtr->refCount++;
    Tcl_IncrRefCount(nsObjPtr);
    copyPtr->typePtr = &tclLambdaType;
}

static void
FreeLambdaInternalRep(
    register Tcl_Obj *objPtr)	/* CmdName object with internal representation
				 * to free. */
{
    Proc *procPtr = objPtr->internalRep.twoPtrValue.ptr1;
    Tcl_Obj *nsObjPtr = objPtr->internalRep.twoPtrValue.ptr2;

    if (procPtr->refCount-- == 1) {
	TclProcCleanupProc(procPtr);
    }
    TclDecrRefCount(nsObjPtr);
    objPtr->typePtr = NULL;
}

static int
SetLambdaFromAny(
    Tcl_Interp *interp,		/* Used for error reporting if not NULL. */
    register Tcl_Obj *objPtr)	/* The object to convert. */
{
    Interp *iPtr = (Interp *) interp;
    const char *name;
    Tcl_Obj *argsPtr, *bodyPtr, *nsObjPtr, **objv;
    int isNew, objc, result;
    CmdFrame *cfPtr = NULL;
    Proc *procPtr;

    if (interp == NULL) {
	return TCL_ERROR;
    }

    /*
     * Convert objPtr to list type first; if it cannot be converted, or if its
     * length is not 2, then it cannot be converted to tclLambdaType.
     */

    result = TclListObjGetElements(NULL, objPtr, &objc, &objv);
    if ((result != TCL_OK) || ((objc != 2) && (objc != 3))) {
	Tcl_SetObjResult(interp, Tcl_ObjPrintf(
		"can't interpret \"%s\" as a lambda expression",
		Tcl_GetString(objPtr)));
	Tcl_SetErrorCode(interp, "TCL", "VALUE", "LAMBDA", NULL);
	return TCL_ERROR;
    }

    argsPtr = objv[0];
    bodyPtr = objv[1];

    /*
     * Create and initialize the Proc struct. The cmdPtr field is set to NULL
     * to signal that this is an anonymous function.
     */

    name = TclGetString(objPtr);

    if (TclCreateProc(interp, /*ignored nsPtr*/ NULL, name, argsPtr, bodyPtr,
	    &procPtr) != TCL_OK) {
	Tcl_AppendObjToErrorInfo(interp, Tcl_ObjPrintf(
		"\n    (parsing lambda expression \"%s\")", name));
	return TCL_ERROR;
    }

    /*
     * CAREFUL: TclCreateProc returns refCount==1! [Bug 1578454]
     * procPtr->refCount = 1;
     */

    procPtr->cmdPtr = NULL;

    /*
     * TIP #280: Remember the line the apply body is starting on. In a Byte
     * code context we ask the engine to provide us with the necessary
     * information. This is for the initialization of the byte code compiler
     * when the body is used for the first time.
     *
     * NOTE: The body is the second word in the 'objPtr'. Its location,
     * accessible through 'context.line[1]' (see below) is therefore only the
     * first approximation of the actual line the body is on. We have to use
     * the string rep of the 'objPtr' to determine the exact line. This is
     * available already through 'name'. Use 'TclListLines', see 'switch'
     * (tclCmdMZ.c).
     *
     * This code is nearly identical to the #280 code in Tcl_ProcObjCmd, see
     * this file. The differences are the different index of the body in the
     * line array of the context, and the special processing mentioned in the
     * previous paragraph to track into the list. Find a way to factor the
     * common elements into a single function.
     */

    if (iPtr->cmdFramePtr) {
	CmdFrame *contextPtr = TclStackAlloc(interp, sizeof(CmdFrame));

	*contextPtr = *iPtr->cmdFramePtr;
	if (contextPtr->type == TCL_LOCATION_BC) {
	    /*
	     * Retrieve the source context from the bytecode. This call
	     * accounts for the reference to the source file, if any, held in
	     * 'context.data.eval.path'.
	     */

	    TclGetSrcInfoForPc(contextPtr);
	} else if (contextPtr->type == TCL_LOCATION_SOURCE) {
	    /*
	     * We created a new reference to the source file path name when we
	     * created 'context' above. Account for the reference.
	     */

	    Tcl_IncrRefCount(contextPtr->data.eval.path);

	}

	if (contextPtr->type == TCL_LOCATION_SOURCE) {
	    /*
	     * We can record source location within a lambda only if the body
	     * was not created by substitution.
	     */

	    if (contextPtr->line
		    && (contextPtr->nline >= 2) && (contextPtr->line[1] >= 0)) {
		int buf[2];

		/*
		 * Move from approximation (line of list cmd word) to actual
		 * location (line of 2nd list element).
		 */

		cfPtr = ckalloc(sizeof(CmdFrame));
		TclListLines(objPtr, contextPtr->line[1], 2, buf, NULL);

		cfPtr->level = -1;
		cfPtr->type = contextPtr->type;
		cfPtr->line = ckalloc(sizeof(int));
		cfPtr->line[0] = buf[1];
		cfPtr->nline = 1;
		cfPtr->framePtr = NULL;
		cfPtr->nextPtr = NULL;

		cfPtr->data.eval.path = contextPtr->data.eval.path;
		Tcl_IncrRefCount(cfPtr->data.eval.path);

		cfPtr->cmd = NULL;
		cfPtr->len = 0;
	    }

	    /*
	     * 'contextPtr' is going out of scope. Release the reference that
	     * it's holding to the source file path
	     */

	    Tcl_DecrRefCount(contextPtr->data.eval.path);
	}
	TclStackFree(interp, contextPtr);
    }
    Tcl_SetHashValue(Tcl_CreateHashEntry(iPtr->linePBodyPtr, procPtr,
	    &isNew), cfPtr);

<<<<<<< HEAD
    /*
     * Set the namespace for this lambda: given by objv[2] understood as a
     * global reference, or else global per default.
     */

    if (objc == 2) {
	TclNewLiteralStringObj(nsObjPtr, "::");
    } else {
	const char *nsName = TclGetString(objv[2]);

	if ((*nsName != ':') || (*(nsName+1) != ':')) {
	    TclNewLiteralStringObj(nsObjPtr, "::");
	    Tcl_AppendObjToObj(nsObjPtr, objv[2]);
	} else {
	    nsObjPtr = objv[2];
	}
=======
    TclNewObj(objPtr);
    if (objPtr) {
	ProcSetIntRep(objPtr, procPtr);
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    }

    Tcl_IncrRefCount(nsObjPtr);

<<<<<<< HEAD
    /*
     * Free the list internalrep of objPtr - this will free argsPtr, but
     * bodyPtr retains a reference from the Proc structure. Then finish the
     * conversion to tclLambdaType.
     */

    TclFreeIntRep(objPtr);
=======
static void
ProcBodyDup(
    Tcl_Obj *srcPtr,		/* Object to copy. */
    Tcl_Obj *dupPtr)		/* Target object for the duplication. */
{
    Proc *procPtr;
    ProcGetIntRep(srcPtr, procPtr);

    ProcSetIntRep(dupPtr, procPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * ProcBodyFree --
 *
 *	Tcl_ObjType's Free function for the proc body object. The reference
 *	count on its Proc struct is decreased by 1; if the count reaches 0,
 *	the proc is freed.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If the reference count on the Proc struct reaches 0, the struct is
 *	freed.
 *
 *----------------------------------------------------------------------
 */
>>>>>>> upstream/master

    objPtr->internalRep.twoPtrValue.ptr1 = procPtr;
    objPtr->internalRep.twoPtrValue.ptr2 = nsObjPtr;
    objPtr->typePtr = &tclLambdaType;
    return TCL_OK;
=======
ProcBodyFree(
    Tcl_Obj *objPtr)		/* The object to clean up. */
{
    Proc *procPtr;

    ProcGetIntRep(objPtr, procPtr);

    if (procPtr->refCount-- <= 1) {
	TclProcCleanupProc(procPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * DupLambdaInternalRep, FreeLambdaInternalRep, SetLambdaFromAny --
 *
 *	How to manage the internal representations of lambda term objects.
 *	Syntactically they look like a two- or three-element list, where the
 *	first element is the formal arguments, the second is the the body, and
 *	the (optional) third is the namespace to execute the lambda term
 *	within (the global namespace is assumed if it is absent).
 *
 *----------------------------------------------------------------------
 */

static void
DupLambdaInternalRep(
    Tcl_Obj *srcPtr,		/* Object with internal rep to copy. */
    Tcl_Obj *copyPtr)	/* Object with internal rep to set. */
{
    Proc *procPtr;
    Tcl_Obj *nsObjPtr;

    LambdaGetIntRep(srcPtr, procPtr, nsObjPtr);
    assert(procPtr != NULL);

    procPtr->refCount++;

    LambdaSetIntRep(copyPtr, procPtr, nsObjPtr);
}

static void
FreeLambdaInternalRep(
    Tcl_Obj *objPtr)	/* CmdName object with internal representation
				 * to free. */
{
    Proc *procPtr;
    Tcl_Obj *nsObjPtr;
<<<<<<< HEAD

    LambdaGetIntRep(objPtr, procPtr, nsObjPtr);
    assert(procPtr != NULL);

=======

    LambdaGetIntRep(objPtr, procPtr, nsObjPtr);
    assert(procPtr != NULL);

>>>>>>> upstream/master
    if (procPtr->refCount-- <= 1) {
	TclProcCleanupProc(procPtr);
    }
    TclDecrRefCount(nsObjPtr);
}

static int
SetLambdaFromAny(
    Tcl_Interp *interp,		/* Used for error reporting if not NULL. */
    Tcl_Obj *objPtr)	/* The object to convert. */
{
    Interp *iPtr = (Interp *) interp;
    const char *name;
    Tcl_Obj *argsPtr, *bodyPtr, *nsObjPtr, **objv;
    int isNew, objc, result;
    CmdFrame *cfPtr = NULL;
    Proc *procPtr;

    if (interp == NULL) {
	return TCL_ERROR;
    }

    /*
     * Convert objPtr to list type first; if it cannot be converted, or if its
     * length is not 2, then it cannot be converted to lambdaType.
     */

    result = TclListObjGetElements(NULL, objPtr, &objc, &objv);
    if ((result != TCL_OK) || ((objc != 2) && (objc != 3))) {
	Tcl_SetObjResult(interp, Tcl_ObjPrintf(
		"can't interpret \"%s\" as a lambda expression",
<<<<<<< HEAD
		Tcl_GetString(objPtr)));
=======
		TclGetString(objPtr)));
>>>>>>> upstream/master
	Tcl_SetErrorCode(interp, "TCL", "VALUE", "LAMBDA", NULL);
	return TCL_ERROR;
    }

    argsPtr = objv[0];
    bodyPtr = objv[1];

    /*
     * Create and initialize the Proc struct. The cmdPtr field is set to NULL
     * to signal that this is an anonymous function.
     */

    name = TclGetString(objPtr);

    if (TclCreateProc(interp, /*ignored nsPtr*/ NULL, name, argsPtr, bodyPtr,
	    &procPtr) != TCL_OK) {
	Tcl_AppendObjToErrorInfo(interp, Tcl_ObjPrintf(
		"\n    (parsing lambda expression \"%s\")", name));
	return TCL_ERROR;
    }

    /*
     * CAREFUL: TclCreateProc returns refCount==1! [Bug 1578454]
     * procPtr->refCount = 1;
     */

    procPtr->cmdPtr = NULL;

    /*
     * TIP #280: Remember the line the apply body is starting on. In a Byte
     * code context we ask the engine to provide us with the necessary
     * information. This is for the initialization of the byte code compiler
     * when the body is used for the first time.
     *
     * NOTE: The body is the second word in the 'objPtr'. Its location,
     * accessible through 'context.line[1]' (see below) is therefore only the
     * first approximation of the actual line the body is on. We have to use
     * the string rep of the 'objPtr' to determine the exact line. This is
     * available already through 'name'. Use 'TclListLines', see 'switch'
     * (tclCmdMZ.c).
     *
     * This code is nearly identical to the #280 code in Tcl_ProcObjCmd, see
     * this file. The differences are the different index of the body in the
     * line array of the context, and the special processing mentioned in the
     * previous paragraph to track into the list. Find a way to factor the
     * common elements into a single function.
     */

    if (iPtr->cmdFramePtr) {
	CmdFrame *contextPtr = (CmdFrame *)TclStackAlloc(interp, sizeof(CmdFrame));

	*contextPtr = *iPtr->cmdFramePtr;
	if (contextPtr->type == TCL_LOCATION_BC) {
	    /*
	     * Retrieve the source context from the bytecode. This call
	     * accounts for the reference to the source file, if any, held in
	     * 'context.data.eval.path'.
	     */

	    TclGetSrcInfoForPc(contextPtr);
	} else if (contextPtr->type == TCL_LOCATION_SOURCE) {
	    /*
	     * We created a new reference to the source file path name when we
	     * created 'context' above. Account for the reference.
	     */

	    Tcl_IncrRefCount(contextPtr->data.eval.path);

	}

	if (contextPtr->type == TCL_LOCATION_SOURCE) {
	    /*
	     * We can record source location within a lambda only if the body
	     * was not created by substitution.
	     */

	    if (contextPtr->line
		    && (contextPtr->nline >= 2) && (contextPtr->line[1] >= 0)) {
		int buf[2];

		/*
		 * Move from approximation (line of list cmd word) to actual
		 * location (line of 2nd list element).
		 */

		cfPtr = (CmdFrame *)Tcl_Alloc(sizeof(CmdFrame));
		TclListLines(objPtr, contextPtr->line[1], 2, buf, NULL);

		cfPtr->level = -1;
		cfPtr->type = contextPtr->type;
		cfPtr->line = (int *)Tcl_Alloc(sizeof(int));
		cfPtr->line[0] = buf[1];
		cfPtr->nline = 1;
		cfPtr->framePtr = NULL;
		cfPtr->nextPtr = NULL;

		cfPtr->data.eval.path = contextPtr->data.eval.path;
		Tcl_IncrRefCount(cfPtr->data.eval.path);

		cfPtr->cmd = NULL;
		cfPtr->len = 0;
	    }

	    /*
	     * 'contextPtr' is going out of scope. Release the reference that
	     * it's holding to the source file path
	     */

	    Tcl_DecrRefCount(contextPtr->data.eval.path);
	}
	TclStackFree(interp, contextPtr);
    }
    Tcl_SetHashValue(Tcl_CreateHashEntry(iPtr->linePBodyPtr, procPtr,
	    &isNew), cfPtr);

    /*
     * Set the namespace for this lambda: given by objv[2] understood as a
     * global reference, or else global per default.
     */

    if (objc == 2) {
	TclNewLiteralStringObj(nsObjPtr, "::");
    } else {
	const char *nsName = TclGetString(objv[2]);

	if ((*nsName != ':') || (*(nsName+1) != ':')) {
	    TclNewLiteralStringObj(nsObjPtr, "::");
	    Tcl_AppendObjToObj(nsObjPtr, objv[2]);
	} else {
	    nsObjPtr = objv[2];
	}
    }

    /*
     * Free the list internalrep of objPtr - this will free argsPtr, but
     * bodyPtr retains a reference from the Proc structure. Then finish the
     * conversion to lambdaType.
     */

    LambdaSetIntRep(objPtr, procPtr, nsObjPtr);
    return TCL_OK;
}

Proc *
TclGetLambdaFromObj(
    Tcl_Interp *interp,
    Tcl_Obj *objPtr,
    Tcl_Obj **nsObjPtrPtr)
{
    Proc *procPtr;
    Tcl_Obj *nsObjPtr;

    LambdaGetIntRep(objPtr, procPtr, nsObjPtr);

    if (procPtr == NULL) {
	if (SetLambdaFromAny(interp, objPtr) != TCL_OK) {
	    return NULL;
	}
	LambdaGetIntRep(objPtr, procPtr, nsObjPtr);
    }

    assert(procPtr != NULL);
    if (procPtr->iPtr != (Interp *)interp) {
	return NULL;
    }

    *nsObjPtrPtr = nsObjPtr;
    return procPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ApplyObjCmd --
 *
 *	This object-based function is invoked to process the "apply" Tcl
 *	command. See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl object result value.
 *
 * Side effects:
 *	Depends on the content of the lambda term (i.e., objv[1]).
 *
 *----------------------------------------------------------------------
 */

int
Tcl_ApplyObjCmd(
    ClientData clientData,
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const objv[])	/* Argument objects. */
{
    return Tcl_NRCallObjProc(interp, TclNRApplyObjCmd, clientData, objc, objv);
}

int
TclNRApplyObjCmd(
    TCL_UNUSED(ClientData),
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const objv[])	/* Argument objects. */
{
    Proc *procPtr = NULL;
    Tcl_Obj *lambdaPtr, *nsObjPtr;
    int result;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> upstream/master
    Tcl_Namespace *nsPtr;
    ApplyExtraData *extraPtr;

    if (objc < 2) {
	Tcl_WrongNumArgs(interp, 1, objv, "lambdaExpr ?arg ...?");
	return TCL_ERROR;
    }

    /*
     * Set lambdaPtr, convert it to tclLambdaType in the current interp if
     * necessary.
     */

    lambdaPtr = objv[1];
<<<<<<< HEAD
    if (lambdaPtr->typePtr == &tclLambdaType) {
	procPtr = lambdaPtr->internalRep.twoPtrValue.ptr1;
    }

    if ((procPtr == NULL) || (procPtr->iPtr != iPtr)) {
	result = SetLambdaFromAny(interp, lambdaPtr);
	if (result != TCL_OK) {
	    return result;
	}
	procPtr = lambdaPtr->internalRep.twoPtrValue.ptr1;
    }

    /*
     * Find the namespace where this lambda should run, and push a call frame
     * for that namespace. Note that TclObjInterpProc() will pop it.
     */

    nsObjPtr = lambdaPtr->internalRep.twoPtrValue.ptr2;
    result = TclGetNamespaceFromObj(interp, nsObjPtr, &nsPtr);
    if (result != TCL_OK) {
	return TCL_ERROR;
    }

    extraPtr = TclStackAlloc(interp, sizeof(ApplyExtraData));
    memset(&extraPtr->cmd, 0, sizeof(Command));
    procPtr->cmdPtr = &extraPtr->cmd;
    extraPtr->cmd.nsPtr = (Namespace *) nsPtr;

    /*
     * TIP#280 (semi-)HACK!
     *
     * Using cmd.clientData to tell [info frame] how to render the lambdaPtr.
     * The InfoFrameCmd will detect this case by testing cmd.hPtr for NULL.
     * This condition holds here because of the memset() above, and nowhere
     * else (in the core). Regular commands always have a valid hPtr, and
     * lambda's never.
     */

    extraPtr->efi.length = 1;
    extraPtr->efi.fields[0].name = "lambda";
    extraPtr->efi.fields[0].proc = NULL;
    extraPtr->efi.fields[0].clientData = lambdaPtr;
    extraPtr->cmd.clientData = &extraPtr->efi;

    result = TclPushProcCallFrame(procPtr, interp, objc, objv, 1);
    if (result == TCL_OK) {
	TclNRAddCallback(interp, ApplyNR2, extraPtr, NULL, NULL, NULL);
	result = TclNRInterpProcCore(interp, objv[1], 2, &MakeLambdaError);
    }
    return result;
}

static int
ApplyNR2(
    ClientData data[],
    Tcl_Interp *interp,
    int result)
{
    ApplyExtraData *extraPtr = data[0];

    TclStackFree(interp, extraPtr);
    return result;
>>>>>>> upstream/master
}

/*
 *----------------------------------------------------------------------
 *
<<<<<<< HEAD
 * Tcl_ApplyObjCmd --
 *
>>>>>>> upstream/master
 *	This object-based function is invoked to process the "apply" Tcl
 *	command. See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl object result value.
 *
 * Side effects:
 *	Depends on the content of the lambda term (i.e., objv[1]).
 *
 *----------------------------------------------------------------------
 */

int
Tcl_ApplyObjCmd(
    ClientData dummy,		/* Not used. */
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const objv[])	/* Argument objects. */
<<<<<<< HEAD
<<<<<<< HEAD
{
    return Tcl_NRCallObjProc(interp, TclNRApplyObjCmd, dummy, objc, objv);
}

int
TclNRApplyObjCmd(
    ClientData dummy,		/* Not used. */
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const objv[])	/* Argument objects. */
{
=======
=======
>>>>>>> upstream/master
{
    return Tcl_NRCallObjProc(interp, TclNRApplyObjCmd, dummy, objc, objv);
}

int
TclNRApplyObjCmd(
    ClientData dummy,		/* Not used. */
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const objv[])	/* Argument objects. */
{
<<<<<<< HEAD
>>>>>>> upstream/master
    Interp *iPtr = (Interp *) interp;
    Proc *procPtr = NULL;
    Tcl_Obj *lambdaPtr, *nsObjPtr;
    int result, isRootEnsemble;
=======
    Interp *iPtr = (Interp *) interp;
    Proc *procPtr = NULL;
    Tcl_Obj *lambdaPtr, *nsObjPtr;
    int result;
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    Tcl_Namespace *nsPtr;
    ApplyExtraData *extraPtr;

    if (objc < 2) {
	Tcl_WrongNumArgs(interp, 1, objv, "lambdaExpr ?arg ...?");
	return TCL_ERROR;
    }

    /*
     * Set lambdaPtr, convert it to tclLambdaType in the current interp if
     * necessary.
     */

    lambdaPtr = objv[1];
    procPtr = TclGetLambdaFromObj(interp, lambdaPtr, &nsObjPtr);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#define JOE_EXTENSION 0
/*
 * Note: this code is NOT FUNCTIONAL due to the NR implementation; DO NOT
 * ENABLE! Leaving here as reminder to (a) TIP the suggestion, and (b) adapt
 * the code. (MS)
 */

#if JOE_EXTENSION
    else {
	/*
	 * Joe English's suggestion to allow cmdNames to function as lambdas.
	 */

	Tcl_Obj *elemPtr;
	int numElem;

	if ((lambdaPtr->typePtr == &tclCmdNameType) ||
		(TclListObjGetElements(interp, lambdaPtr, &numElem,
		&elemPtr) == TCL_OK && numElem == 1)) {
	    return Tcl_EvalObjv(interp, objc-1, objv+1, 0);
	}
    }
#endif

=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    if ((procPtr == NULL) || (procPtr->iPtr != iPtr)) {
	result = SetLambdaFromAny(interp, lambdaPtr);
	if (result != TCL_OK) {
	    return result;
	}
	procPtr = lambdaPtr->internalRep.twoPtrValue.ptr1;
=======
    if (procPtr == NULL) {
	return TCL_ERROR;
>>>>>>> upstream/master
=======
    procPtr = TclGetLambdaFromObj(interp, lambdaPtr, &nsObjPtr);

    if (procPtr == NULL) {
	return TCL_ERROR;
>>>>>>> upstream/master
    }

    /*
     * Push a call frame for the lambda namespace.
     * Note that TclObjInterpProc() will pop it.
     */

    result = TclGetNamespaceFromObj(interp, nsObjPtr, &nsPtr);
    if (result != TCL_OK) {
	return TCL_ERROR;
    }

    extraPtr = (ApplyExtraData *)TclStackAlloc(interp, sizeof(ApplyExtraData));
    memset(&extraPtr->cmd, 0, sizeof(Command));
    procPtr->cmdPtr = &extraPtr->cmd;
    extraPtr->cmd.nsPtr = (Namespace *) nsPtr;

    /*
     * TIP#280 (semi-)HACK!
     *
     * Using cmd.clientData to tell [info frame] how to render the lambdaPtr.
     * The InfoFrameCmd will detect this case by testing cmd.hPtr for NULL.
     * This condition holds here because of the memset() above, and nowhere
     * else (in the core). Regular commands always have a valid hPtr, and
     * lambda's never.
     */

    extraPtr->efi.length = 1;
    extraPtr->efi.fields[0].name = "lambda";
    extraPtr->efi.fields[0].proc = NULL;
    extraPtr->efi.fields[0].clientData = lambdaPtr;
    extraPtr->cmd.clientData = &extraPtr->efi;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    isRootEnsemble = (iPtr->ensembleRewrite.sourceObjs == NULL);
    if (isRootEnsemble) {
	iPtr->ensembleRewrite.sourceObjs = objv;
	iPtr->ensembleRewrite.numRemovedObjs = 1;
	iPtr->ensembleRewrite.numInsertedObjs = 0;
    } else {
	iPtr->ensembleRewrite.numInsertedObjs -= 1;
    }
    extraPtr->isRootEnsemble = isRootEnsemble;

=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    result = TclPushProcCallFrame(procPtr, interp, objc, objv, 1);
    if (result == TCL_OK) {
	TclNRAddCallback(interp, ApplyNR2, extraPtr, NULL, NULL, NULL);
	result = TclNRInterpProcCore(interp, objv[1], 2, &MakeLambdaError);
    }
    return result;
}

static int
ApplyNR2(
    ClientData data[],
    Tcl_Interp *interp,
    int result)
{
    ApplyExtraData *extraPtr = (ApplyExtraData *)data[0];

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    if (extraPtr->isRootEnsemble) {
	((Interp *) interp)->ensembleRewrite.sourceObjs = NULL;
    }

=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    TclStackFree(interp, extraPtr);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
=======
>>>>>>> upstream/master
 * MakeLambdaError --
 *
 *	Function called by TclObjInterpProc to create the stack information
 *	upon an error from a lambda term.
 *
 * Results:
 *	The interpreter's error info trace is set to a value that supplements
 *	the error code.
 *
 * Side effects:
 *	none.
 *
 *----------------------------------------------------------------------
 */

static void
MakeLambdaError(
    Tcl_Interp *interp,		/* The interpreter in which the procedure was
				 * called. */
    Tcl_Obj *procNameObj)	/* Name of the procedure. Used for error
				 * messages and trace information. */
{
<<<<<<< HEAD
<<<<<<< HEAD
    int overflow, limit = 60, nameLen;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    const char *procName = Tcl_GetStringFromObj(procNameObj, &nameLen);
=======
=======
    unsigned int overflow, limit = 60;
    size_t nameLen;
>>>>>>> upstream/master
    const char *procName = TclGetStringFromObj(procNameObj, &nameLen);
>>>>>>> upstream/master
=======
    const char *procName = TclGetStringFromObj(procNameObj, &nameLen);
>>>>>>> upstream/master
=======
    const char *procName = TclGetStringFromObj(procNameObj, &nameLen);
>>>>>>> upstream/master
=======
    unsigned int overflow, limit = 60;
    size_t nameLen;
    const char *procName = TclGetStringFromObj(procNameObj, &nameLen);
>>>>>>> upstream/master

    overflow = (nameLen > limit);
    Tcl_AppendObjToErrorInfo(interp, Tcl_ObjPrintf(
	    "\n    (lambda term \"%.*s%s\" line %d)",
	    (int)(overflow ? limit : nameLen), procName,
	    (overflow ? "..." : ""), Tcl_GetErrorLine(interp)));
}

/*
<<<<<<< HEAD
=======
 *----------------------------------------------------------------------
 *
 * TclGetCmdFrameForProcedure --
<<<<<<< HEAD
 *
 *	How to get the CmdFrame information for a procedure.
<<<<<<< HEAD
 *
 * Results:
 *	A pointer to the CmdFrame (only guaranteed to be valid until the next
 *	Tcl command is processed or the interpreter's state is otherwise
 *	modified) or a NULL if the information is not available.
 *
=======
 *
=======
 *
 *	How to get the CmdFrame information for a procedure.
 *
>>>>>>> upstream/master
 * Results:
 *	A pointer to the CmdFrame (only guaranteed to be valid until the next
 *	Tcl command is processed or the interpreter's state is otherwise
 *	modified) or a NULL if the information is not available.
 *
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
 * Side effects:
 *	none.
 *
 *----------------------------------------------------------------------
 */

CmdFrame *
TclGetCmdFrameForProcedure(
    Proc *procPtr)		/* The procedure whose cmd-frame is to be
				 * looked up. */
{
    Tcl_HashEntry *hePtr;

    if (procPtr == NULL || procPtr->iPtr == NULL) {
	return NULL;
    }
    hePtr = Tcl_FindHashEntry(procPtr->iPtr->linePBodyPtr, procPtr);
    if (hePtr == NULL) {
	return NULL;
    }
    return (CmdFrame *) Tcl_GetHashValue(hePtr);
}

/*
>>>>>>> upstream/master
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * fill-column: 78
 * End:
 */
