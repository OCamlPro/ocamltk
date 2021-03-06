/* Here, CYGWIN is equivalent to WIN32 */
#ifdef __CYGWIN__
#define _WIN32
#endif

#if defined(__CYGWIN__) || defined( _WIN32)
#include <wtypes.h>
#include <winbase.h>
#include <winsock.h>
#endif
#include <tcl.h>
#include <tk.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include "camltk.h"

/*
 * File descriptor callbacks
 */

void FileProc(ClientData clientdata, int mask)
{
  callback2(*handler_code,Val_int(clientdata),Val_int(0));
}

/* Map Unix.file_descr values to Tcl file handles */

#ifndef _WIN32

/* Under Unix, we use file handlers */

/* Map Unix.file_descr values to Tcl file handles (for tcl 7)
   or Unix file descriptors (for tcl 8). */

#if (TCL_MAJOR_VERSION < 8)
static Tcl_File tcl_filehandle(value fd)
{
  return Tcl_GetFile((ClientData)Long_val(fd), TCL_UNIX_FD);
}
#else
#define tcl_filehandle(fd) Int_val(fd)
#define Tcl_File int
#endif

value camltk_add_file_input(value fd, value cbid)    /* ML */
{
  CheckInit();
  Tcl_CreateFileHandler(tcl_filehandle(fd), TCL_READABLE, 
                       FileProc, (ClientData)(Long_val(cbid)));
  return Val_unit;
}

/* We have to free the Tcl handle when we are finished using it (Tcl
 * asks us to, and moreover it is probably dangerous to keep the same
 * handle over two allocations of the same fd by the kernel). 
 * But we don't know when we are finished with the fd, so we free it
 * in rem_file (it doesn't close the fd anyway). For fds for which we
 * repeatedly add/rem, this will cause some overhead.
 */
value camltk_rem_file_input(value fd, value cbid) /* ML */
{
  Tcl_File fh = tcl_filehandle(fd);
  Tcl_DeleteFileHandler(fh);
#if (TCL_MAJOR_VERSION < 8)
  Tcl_FreeFile(fh);
#endif
  return Val_unit;
}

value camltk_add_file_output(value fd, value cbid)    /* ML */
{
  CheckInit();
  Tcl_CreateFileHandler(tcl_filehandle(fd), TCL_WRITABLE, 
                       FileProc, (ClientData) (Long_val(cbid)));
  return Val_unit;
}

value camltk_rem_file_output(value fd, value cbid) /* ML */
{
  Tcl_File fh = tcl_filehandle(fd);
  Tcl_DeleteFileHandler(fh);
#if (TCL_MAJOR_VERSION < 8)
  Tcl_FreeFile(fh);
#endif
  return Val_unit;
}

#else

/* Under Win32 or Cygwin, we go through the generic channel abstraction */

/* Map Unix.file_descr values to Tcl channels */

#define Handle_val(v) (*((HANDLE *)(v)))

static Tcl_Channel tcl_channel(value fd, int flags)
{
  HANDLE h = Handle_val(fd);
  int optval, optsize;

  optsize = sizeof(optval);
  if (getsockopt((SOCKET) h, SOL_SOCKET, SO_TYPE,
                 (char *)&optval, &optsize) == 0)
    return Tcl_MakeTcpClientChannel((ClientData) h);
  else
    return Tcl_MakeFileChannel((ClientData) h, flags);
}

value camltk_add_file_input(value fd, value cbid)    /* ML */
{
  CheckInit();
  Tcl_CreateChannelHandler(tcl_channel(fd, TCL_READABLE),
                           TCL_READABLE,
                           FileProc, (ClientData) (Int_val(cbid)));
  return Val_unit;
}

value camltk_rem_file_input(value fd, value cbid) /* ML */
{
  Tcl_DeleteChannelHandler(tcl_channel(fd, TCL_READABLE),
                           FileProc, (ClientData) (Int_val(cbid)));
  return Val_unit;
}

value camltk_add_file_output(value fd, value cbid)    /* ML */
{
  CheckInit();
  Tcl_CreateChannelHandler(tcl_channel(fd, TCL_WRITABLE),
                           TCL_WRITABLE,
                           FileProc, (ClientData) (Int_val(cbid)));
  return Val_unit;
}

value camltk_rem_file_output(value fd, value cbid) /* ML */
{
  Tcl_DeleteChannelHandler(tcl_channel(fd, TCL_WRITABLE),
                           FileProc, (ClientData) (Int_val(cbid)));
  return Val_unit;
}

#endif
