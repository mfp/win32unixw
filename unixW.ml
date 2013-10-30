
open Unix

exception Invalid_UTF8 of string * string

module UTF : sig
  type utf16
  type utf16z

  val of_utf16 : string -> utf16 -> string
  val to_utf16z : string -> string -> utf16z
  val utf8_of_utf16z : string -> string
end =
struct
  type utf16 = string
  type utf16z = string

  external utf8_to_utf16 : string -> string option = "win32unixw_convert_utf8_to_utf16"
  external utf16_to_utf8 : string -> string option = "win32unixw_convert_utf16_to_utf8"

  let to_utf16z funcname s = match utf8_to_utf16 s with
      None -> raise (Invalid_UTF8 (funcname, s))
    | Some s -> s ^ "\000\000"

  let of_utf16 funcname s =
    match utf16_to_utf8 s with
        None ->
          invalid_arg
            (Printf.sprintf
               "UnixW.%s: WinAPI returned non-UTF16 string %S" funcname s)
      | Some s -> s

  let utf8_of_utf16z s =
    let len = String.length s in
    let s'  = match s with
        "" | "\000\000" -> ""
      | s when s.[len - 2] = '\000' && s.[len - 1] = '\000' -> String.sub s 0 (len - 2)
      | s -> s
    in
      match utf16_to_utf8 s' with
          None -> s (* best effort conversion failed, just return orig *)
        | Some s -> s
end

open UTF

DEFINE WRAP(x) =
  try
    x
  with Unix.Unix_error (err, func, arg) ->
    raise (Unix.Unix_error (err, func, utf8_of_utf16z arg))

external getenv : utf16z -> utf16 = "unix_getenvW"
let getenv s = WRAP (of_utf16 "getenv" (getenv (to_utf16z "getenv" s)))

external putenv : utf16z -> unit = "unix_putenvW"
let putenv k v = WRAP (putenv (to_utf16z "putenv" (String.concat "=" [k; v])))

external openfile : utf16z -> open_flag list -> file_perm -> file_descr = "unix_openW"
let openfile fname flags perms = WRAP (openfile (to_utf16z "openfile" fname) flags perms)

external stat : utf16z -> stats = "unix_statW"

let lstat s = WRAP (stat (to_utf16z "lstat" s))
let stat s  = WRAP (stat (to_utf16z "stat" s))

module LargeFile =
struct
  open LargeFile
  external stat : utf16z -> stats = "unix_statW_64"

  let lstat s = WRAP (stat (to_utf16z "lstat" s))
  let stat s  = WRAP (stat (to_utf16z "stat" s))
end

external unlink : utf16z -> unit = "unix_unlinkW"
let unlink s = WRAP (unlink (to_utf16z "unlinK" s))

external rename : utf16z -> utf16z -> unit = "unix_renameW"
let rename src dst = WRAP (rename (to_utf16z "rename" src) (to_utf16z "rename" dst))

external link : utf16z -> utf16z -> unit = "unix_linkW"
let link src dst = WRAP (link (to_utf16z "link" src) (to_utf16z "link" dst))

external chmod : utf16z -> file_perm -> unit = "unix_chmodW"
let chmod s p = WRAP (chmod (to_utf16z "chmod" s) p)

external access : utf16z -> access_permission list -> unit = "unix_accessW"
let access s l = WRAP (access (to_utf16z "access" s) l)

external mkdir : utf16z -> file_perm -> unit = "unix_mkdirW"
let mkdir s p = WRAP (mkdir (to_utf16z "mkdir" s) p)

external rmdir : utf16z -> unit = "unix_rmdirW"
let rmdir s = WRAP (rmdir (to_utf16z "rmdir" s))

external chdir : utf16z -> unit = "unix_chdirW"
let chdir s = WRAP (chdir (to_utf16z "chdir" s))

external getcwd : unit -> utf16 = "unix_getcwdW"
let getcwd () = WRAP (of_utf16 "getcwd" (getcwd ()))

type dir_entry =
    Dir_empty
  | Dir_read of string
  | Dir_toread

type dir_handle =
  { dirname: string; mutable handle: int; mutable entry_read: dir_entry }

external findfirst : utf16z -> utf16 * int = "win_findfirstW"
external findnext : int -> utf16 = "win_findnextW"

let opendir dirname =
  try
    let (first_entry, handle) = WRAP (findfirst (to_utf16z "opendir" (Filename.concat dirname "*.*"))) in
    { dirname = dirname; handle = handle; entry_read = Dir_read (of_utf16 "opendir" first_entry) }
  with End_of_file ->
    { dirname = dirname; handle = 0; entry_read = Dir_empty }

let readdir d =
  match d.entry_read with
    Dir_empty -> raise End_of_file
  | Dir_read name -> d.entry_read <- Dir_toread; name
  | Dir_toread -> of_utf16 "readdir" (WRAP (findnext d.handle))

external win_findclose : int -> unit = "win_findclose"

let closedir d =
  match d.entry_read with
    Dir_empty -> ()
  | _ -> win_findclose d.handle

let rewinddir d =
  closedir d;
  try
    let (first_entry, handle) = findfirst (to_utf16z "rewinddir" (d.dirname ^ "\\*.*")) in
    d.handle <- handle; d.entry_read <- Dir_read (of_utf16 "rewinddir" first_entry)
  with End_of_file ->
    d.handle <- 0; d.entry_read <- Dir_empty
