/*
 * Pseudo TTY library
 *
 * Copyright (C) 2015 Lawrence Woodman <lwoodman@vlifesystems.com>
 *
 * Licensed under an MIT licence.  Please see LICENCE.md for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <tcl.h>


static int
Pty_Open(ClientData clientData,
         Tcl_Interp *interp,
         int objc,
         Tcl_Obj *CONST objv[])
{
  intptr_t masterfd;
  char slaveName[256];
  Tcl_Channel masterChan;
  Tcl_Channel slaveChan;
  char *argStr;
  int lengthArgStr;
  int argNum;
  int nogrant = 0;
  int numArgsLeft;
  Tcl_Obj *result;

  for (argNum = 1; argNum < objc; argNum++) {
    argStr = Tcl_GetStringFromObj (objv[argNum], &lengthArgStr);
    if (argStr[0] != '-') {
      break;
    }

    if (strncmp(argStr, "-nogrant", lengthArgStr) == 0) {
      nogrant = 1;
    } else {
      Tcl_AppendResult(interp,
                       "bad option \"", argStr,
                       "\": must be -nogrant",
                       (char *) NULL);
      return TCL_ERROR;
    }
  }

  numArgsLeft = objc - argNum;

  if (numArgsLeft != 0) {
    Tcl_WrongNumArgs(interp, 1, objv, "?options?");
    return TCL_ERROR;
  }

  masterfd = posix_openpt(O_RDWR | O_NOCTTY);

  if (masterfd == -1) {
    Tcl_AppendResult(interp,
                     "posix_openpt failed \"",
                     Tcl_ErrnoMsg(Tcl_GetErrno()), "\"",
                     (char *) NULL);
    return TCL_ERROR;
  }

  if (!nogrant) {
    if (grantpt(masterfd) != 0) {
      Tcl_AppendResult(interp,
                       "grantpt failed \"",
                       Tcl_ErrnoMsg(Tcl_GetErrno()), "\"",
                       (char *) NULL);
      return TCL_ERROR;
    }
  }

  if (unlockpt(masterfd) != 0) {
    Tcl_AppendResult(interp,
                     "unlockpt failed \"",
                     Tcl_ErrnoMsg(Tcl_GetErrno()), "\"",
                     (char *) NULL);
    return TCL_ERROR;
  }

  if (ptsname_r(masterfd, slaveName, sizeof(slaveName)) != 0) {
      Tcl_AppendResult(interp,
                       "ptsname_r failed \"",
                       Tcl_ErrnoMsg(Tcl_GetErrno()), "\"",
                       (char *) NULL);
      return TCL_ERROR;
  }

  masterChan =
    Tcl_MakeFileChannel((ClientData)masterfd, TCL_READABLE | TCL_WRITABLE);

  if (masterChan == (Tcl_Channel)NULL) {
      return TCL_ERROR;
  }

  Tcl_RegisterChannel(interp, masterChan);

  result = Tcl_NewListObj(0, NULL);

  Tcl_ListObjAppendElement(interp,
                           result,
                           Tcl_NewStringObj(Tcl_GetChannelName(masterChan),
                                            -1));
  Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj(slaveName, -1));
  Tcl_SetObjResult(interp, result);

  return TCL_OK;
}


int
Pty_Init(Tcl_Interp *interp)
{
  Tcl_CreateObjCommand(interp,
                       "pty::open",
                       Pty_Open,
                       (ClientData) NULL,
                       (Tcl_CmdDeleteProc *) NULL);

  if ( Tcl_PkgProvide(interp, "pty", "0.1") != TCL_OK ) {
    return TCL_ERROR;
  }

  return TCL_OK;
}
