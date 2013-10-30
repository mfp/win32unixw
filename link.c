/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*                 File contributed by Lionel Fourquaux                */
/*                                                                     */
/*  Copyright 2001 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../../LICENSE.  */
/*                                                                     */
/***********************************************************************/

#include <caml/mlvalues.h>
#include <caml/fail.h>
#include <caml/unixsupport.h>
#include <windows.h>

typedef
BOOL (WINAPI *tCreateHardLink)(
  LPCWSTR lpFileName,
  LPCWSTR lpExistingFileName,
  LPSECURITY_ATTRIBUTES lpSecurityAttributes
);

CAMLprim value unix_linkW(value path1, value path2)
{
  HMODULE hModKernel32;
  tCreateHardLink pCreateHardLink;
  hModKernel32 = GetModuleHandle("KERNEL32.DLL");
  pCreateHardLink =
    (tCreateHardLink) GetProcAddress(hModKernel32, "CreateHardLinkW");
  if (pCreateHardLink == NULL)
    invalid_argument("Unix.link not implemented");
  if (! pCreateHardLink((LPCWSTR)String_val(path2), (LPCWSTR)String_val(path1), NULL)) {
    win32_maperr(GetLastError());
    uerror("link", path2);
  }
  return Val_unit;
}
