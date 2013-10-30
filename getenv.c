/***********************************************************************/
/* Copyright (C) 2013 Mauricio Fernández <mfp@acm.org>                 */
/***********************************************************************/

#include <stdlib.h>
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/unixsupport.h>
#include <caml/memory.h>
#include "utfutil.h"

CAMLprim value unix_getenvW(value s)
{
  CAMLparam1(s);

  WCHAR *v;
  v = _wgetenv((LPCWSTR)s);
  if(v == NULL) caml_raise_not_found();

  CAMLreturn(copy_utf16_string(v));
}
