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
#include <caml/unixsupport.h>

CAMLprim value unix_mkdirW(path, perm)
     value path, perm;
{
  if (_wmkdir((const wchar_t *)String_val(path)) == -1) uerror("mkdir", path);
  return Val_unit;
}
