/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*            Xavier Leroy, projet Cristal, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../../LICENSE.  */
/*                                                                     */
/***********************************************************************/

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/unixsupport.h>
#include <process.h>

extern char ** cstringvect();

CAMLprim value unix_execveW(value path, value args, value env)
{
  wchar_t ** argv;
  wchar_t ** envp;
  argv = (wchar_t **)cstringvect(args);
  envp = (wchar_t **)cstringvect(env);
  (void) _wexecve((LPCWSTR)String_val(path), argv, envp);
  stat_free((char *) argv);
  stat_free((char *) envp);
  uerror("execve", path);
  return Val_unit;                  /* never reached, but suppress warnings */
                                /* from smart compilers */
}
