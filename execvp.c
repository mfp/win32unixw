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

CAMLprim value unix_execvpW(value path, value args)
{
  wchar_t ** argv;
  argv = (wchar_t **)cstringvect(args);
  (void) _wexecvp((LPCWSTR)String_val(path), argv);
  stat_free((char *) argv);
  uerror("execvp", path);
  return Val_unit;                  /* never reached, but suppress warnings */
                                /* from smart compilers */
}

CAMLprim value unix_execvpeW(value path, value args, value env_)
{
  wchar_t ** argv;
  wchar_t ** env;
  argv = (wchar_t **)cstringvect(args);
  env  = (wchar_t **)cstringvect(env_);
  (void) _wexecvpe((LPCWSTR)String_val(path), argv, env);
  stat_free((char *) argv);
  stat_free((char *) env);
  uerror("execvpe", path);
  return Val_unit;                  /* never reached, but suppress warnings */
                                /* from smart compilers */
}
