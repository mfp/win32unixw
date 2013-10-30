/***********************************************************************/
/* Copyright (C) 2013 Mauricio Fernández <mfp@acm.org>                 */
/***********************************************************************/

#include <stdlib.h>
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/unixsupport.h>
#include <caml/memory.h>

CAMLprim value unix_putenvW(value s)
{
  if (_wputenv((LPCWSTR)s) == -1) uerror("putenv", s);
  return Val_unit;
}
