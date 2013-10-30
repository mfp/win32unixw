/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*            Xavier Leroy, projet Cristal, INRIA Rocquencourt         */
/*  Modified by Mauricio Fernández, <mfp@acm.org>                      */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique, 2013 Mauricio Fernández.                           */
/*  All rights reserved.  This file is distributed                     */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file LICENSE.        */
/*                                                                     */
/***********************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <caml/mlvalues.h>
#include <caml/unixsupport.h>

CAMLprim value unix_chmodW(value path, value perm)
{
  int ret;
  ret = _wchmod((LPCWSTR)String_val(path), Int_val(perm));
  if (ret == -1) uerror("chmod", path);
  return Val_unit;
}
