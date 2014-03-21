(***********************************************************************)
(*                                                                     *)
(*                                OCaml                                *)
(*                                                                     *)
(*            Xavier Leroy, projet Cristal, INRIA Rocquencourt         *)
(*  Modified by Mauricio Fernández, <mfp@acm.org>                      *)
(*                                                                     *)
(*  Copyright 1996 Institut National de Recherche en Informatique et   *)
(*  en Automatique.  All rights reserved.  This file is distributed    *)
(*  under the terms of the GNU Library General Public License, with    *)
(*  the special exception on linking described in file LICENSE.        *)
(*                                                                     *)
(*  Copyright 2013 Mauricio Fernández <mfp@acm.org>                    *)
(*                                                                     *)
(***********************************************************************)

(** Unicode-aware Unix operations on Win32.
  *
  * The functions operate/return UTF-8 strings that are converted
  * internally to/from UTF-16LE.
  *
  * *)

(** Exception raised by [UnixW] functions when supplied invalid UTF8 input
  * strings. *)
exception Invalid_UTF8 of string (** function name *) * string (** invalid UTF8 string *)

open Unix

(** Interface to the Unix system *)

(** {6 Access to the process environment} *)

val getenv : string -> string
(** Return the value associated to a variable in the process environment.
  * Raise [Not_found] if the variable is unbound. *)

(** [Unix.putenv name value] sets the value associated to a variable in the
  * process environment. The variable will be removed from the environment if
  * [value] is the empty string. *)
val putenv : string -> string -> unit


(** {6 Process handling} *)

val execv : string -> string array -> 'a
(** [execv prog args] execute the program in file [prog], with
   the arguments [args], and the current process environment.
   These [execv*] functions never return: on success, the current
   program is replaced by the new one;
   on failure, a {!Unix.Unix_error} exception is raised. *)

val execve : string -> string array -> string array -> 'a
(** Same as {!Unix.execv}, except that the third argument provides the
   environment to the program executed. *)

val execvp : string -> string array -> 'a
(** Same as {!Unix.execv}, except that
   the program is searched in the path. *)

val execvpe : string -> string array -> string array -> 'a
(** Same as {!Unix.execve}, except that
   the program is searched in the path. *)

(** {6 Basic file input/output} *)

val openfile : string -> open_flag list -> file_perm -> file_descr
(** Open the named file with the given flags. Third argument is
   the permissions to give to the file if it is created. Return
   a file descriptor on the named file. *)

(** {6 File status} *)

val stat : string -> stats
(** Return the information for the named file. *)

val lstat : string -> stats
(** Same as {!Unix.stat}, but in case the file is a symbolic link,
   return the information for the link itself. *)

(** {6 File operations on large files} *)

module LargeFile :
  sig
    open LargeFile
    val stat : string -> stats
    val lstat : string -> stats
  end
(** File operations on large files.
  This sub-module provides 64-bit variants of the functions
  {!Unix.stat}, {!Unix.lstat} and {!Unix.fstat} (for obtaining
  information on files).  These alternate functions represent
  positions and sizes by 64-bit integers (type [int64]) instead of
  regular integers (type [int]), thus allowing operating on files
  whose sizes are greater than [max_int]. *)


(** {6 Operations on file names} *)


val unlink : string -> unit
(** Removes the named file *)

val rename : string -> string -> unit
(** [rename old new] changes the name of a file from [old] to [new]. *)

val link : string -> string -> unit
(** [link source dest] creates a hard link named [dest] to the file
   named [source]. *)


(** {6 File permissions and ownership} *)

val chmod : string -> file_perm -> unit
(** Change the permissions of the named file. *)

val access : string -> access_permission list -> unit
(** Check that the process has the given permissions over the named
   file. Raise [Unix_error] otherwise. *)

(** {6 Directories} *)

val mkdir : string -> file_perm -> unit
(** Create a directory with the given permissions. *)

val rmdir : string -> unit
(** Remove an empty directory. *)

val chdir : string -> unit
(** Change the process working directory. *)

val getcwd : unit -> string
(** Return the name of the current working directory. *)

type dir_handle
(** The type of descriptors over opened directories. *)

val opendir : string -> dir_handle
(** Open a descriptor on a directory *)

val readdir : dir_handle -> string
(** Return the next entry in a directory.
   @raise End_of_file when the end of the directory has been reached. *)

val rewinddir : dir_handle -> unit
(** Reposition the descriptor to the beginning of the directory *)

val closedir : dir_handle -> unit
(** Close a directory descriptor. *)
