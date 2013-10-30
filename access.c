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

#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/unixsupport.h>

#ifdef HAS_UNISTD
# include <unistd.h>
#else
# ifndef _WIN32
#  include <sys/file.h>
#  ifndef R_OK
#   define R_OK    4/* test for read permission */
#   define W_OK    2/* test for write permission */
#   define X_OK    1/* test for execute (search) permission */
#   define F_OK    0/* test for presence of file */
#  endif
# else
#  ifndef R_OK
#   define R_OK    4/* test for read permission */
#   define W_OK    2/* test for write permission */
#   define X_OK    4/* test for execute permission - not implemented in Win32 */
#   define F_OK    0/* test for presence of file */
#  endif
# endif
#endif

static int access_permission_table[] = {
  R_OK, W_OK, X_OK, F_OK
};

CAMLprim value unix_accessW(value path, value perms)
{
  int ret, cv_flags;

  cv_flags = convert_flag_list(perms, access_permission_table);
  ret = _waccess((LPCWSTR)String_val(path), cv_flags);
  if (ret == -1)
    uerror("access", path);
  return Val_unit;
}
