/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*   Pascal Cuoq and Xavier Leroy, projet Cristal, INRIA Rocquencourt  */
/*  Modified by Mauricio Fernández, <mfp@acm.org>                      */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file LICENSE.        */
/*                                                                     */
/***********************************************************************/

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <errno.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/unixsupport.h>
#include "utfutil.h"

CAMLprim value win_findfirstW(name)
     value name;
{
  HANDLE h;
  value v;
  WIN32_FIND_DATAW fileinfo;
  value valname = Val_unit;
  value valh = Val_unit;

  Begin_roots2 (valname,valh);
    h = FindFirstFileW((LPCWSTR)String_val(name),&fileinfo);
    if (h == INVALID_HANDLE_VALUE) {
      DWORD err = GetLastError();
      if (err == ERROR_NO_MORE_FILES)
        raise_end_of_file();
      else {
        win32_maperr(err);
        uerror("opendir", Nothing);
      }
    }
    valname = copy_utf16_string(fileinfo.cFileName);
    valh = win_alloc_handle(h);
    v = alloc_small(2, 0);
    Field(v,0) = valname;
    Field(v,1) = valh;
  End_roots();
  return v;
}

CAMLprim value win_findnextW(valh)
     value valh;
{
  WIN32_FIND_DATAW fileinfo;
  BOOL retcode;

  retcode = FindNextFileW(Handle_val(valh), &fileinfo);
  if (!retcode) {
    DWORD err = GetLastError();
    if (err == ERROR_NO_MORE_FILES)
      raise_end_of_file();
    else {
      win32_maperr(err);
      uerror("readdir", Nothing);
    }
  }
  return copy_utf16_string(fileinfo.cFileName);
}
