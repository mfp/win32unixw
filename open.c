/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*  Xavier Leroy and Pascal Cuoq, projet Cristal, INRIA Rocquencourt   */
/*  Modified by Mauricio Fernández, <mfp@acm.org>                      */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique, 2013 Mauricio Fernández.                           */
/*  All rights reserved.  This file is distributed                     */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file LICENSE.        */
/*                                                                     */
/***********************************************************************/

#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/unixsupport.h>
#include <fcntl.h>

static int open_access_flags[14] = {
  GENERIC_READ, GENERIC_WRITE, GENERIC_READ|GENERIC_WRITE,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static int open_create_flags[14] = {
  0, 0, 0, 0, 0, O_CREAT, O_TRUNC, O_EXCL, 0, 0, 0, 0, 0, 0
};

static int open_share_flags[14] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, FILE_SHARE_DELETE, 0
};

static int open_cloexec_flags[14] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
};

static int open_append_flags[14] = {
  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

CAMLprim value unix_openW(value path, value flags, value perm)
{
  int fileaccess, createflags, fileattrib, filecreate, sharemode, cloexec, append;
  SECURITY_ATTRIBUTES attr;
  HANDLE h;

  fileaccess = convert_flag_list(flags, open_access_flags);
  sharemode = FILE_SHARE_READ | FILE_SHARE_WRITE | convert_flag_list(flags, open_share_flags);

  createflags = convert_flag_list(flags, open_create_flags);
  if ((createflags & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL))
    filecreate = CREATE_NEW;
  else if ((createflags & (O_CREAT | O_TRUNC)) == (O_CREAT | O_TRUNC))
    filecreate = CREATE_ALWAYS;
  else if (createflags & O_TRUNC)
    filecreate = TRUNCATE_EXISTING;
  else if (createflags & O_CREAT)
    filecreate = OPEN_ALWAYS;
  else
    filecreate = OPEN_EXISTING;

  if ((createflags & O_CREAT) && (Int_val(perm) & 0200) == 0)
    fileattrib = FILE_ATTRIBUTE_READONLY;
  else
    fileattrib = FILE_ATTRIBUTE_NORMAL;

  cloexec = convert_flag_list(flags, open_cloexec_flags);
  append  = convert_flag_list(flags, open_append_flags);
  attr.nLength = sizeof(attr);
  attr.lpSecurityDescriptor = NULL;
  attr.bInheritHandle = cloexec ? FALSE : TRUE;

  h = CreateFileW((LPCWSTR)String_val(path), fileaccess,
                 sharemode, &attr,
                 filecreate, fileattrib, NULL);
  if (h == INVALID_HANDLE_VALUE) {
    win32_maperr(GetLastError());
    uerror("open", path);
  }

  if(append) SetFilePointer(h, 0, NULL, FILE_END);
  return win_alloc_handle(h);
}
