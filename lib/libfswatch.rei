type monitor =
  /* System default monitor. */
  | System_default_monitor_type
  /* OS X FSEvents monitor. */
  | Fsevents_monitor_type
  /* BSD kqueue monitor. */
  | Kqueue_monitor_type
  /* Linux inotify monitor. */
  | Inotify_monitor_type
  /* Windows monitor. */
  | Windows_monitor_type
  /* stat()-based poll monitor. */
  | Poll_monitor_type
  /* Solaris/Illumos monitor.  */
  | Fen_monitor_type;

type event_flag =
  | NoOp
  | PlatformSpecific
  | Created
  | Updated
  | Removed
  | Renamed
  | OwnerModified
  | AttributeModified
  | MovedFrom
  | MovedTo
  | IsFile
  | IsDir
  | IsSymLink
  | Link
  | Overflow
  | Unknown;

type callback_event = {
  path: string,
  flags: list(event_flag),
};

type status;
type handle;

type callback = list(callback_event) => unit;

let init: unit => status;

let make_handle: monitor => handle;

let add_path: (handle, string) => status;

let set_callback: (handle, callback) => status;

let start: handle => status;

let stop: handle => status;
