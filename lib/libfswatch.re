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

type c_callback_event = {
  path: string,
  string_flags: list(string),
};

type status;
type handle;

type callback = list(callback_event) => unit;

external init: unit => status = "_fsw_init_library";

external make_handle: monitor => handle = "_fsw_init_session";

external add_path: (handle, string) => status = "_fsw_add_path";

external set_callback: handle => status = "_fsw_set_callback";

let flag_of_string = str => switch (str) {
  | "NoOp" => NoOp
  | "PlatformSpecific" => PlatformSpecific
  | "Created" => Created
  | "Updated" => Updated
  | "Removed" => Removed
  | "Renamed" => Renamed
  | "OwnerModified" => OwnerModified
  | "AttributeModified" => AttributeModified
  | "MovedFrom" => MovedFrom
  | "MovedTo" => MovedTo
  | "IsFile" => IsFile
  | "IsDir" => IsDir
  | "IsSymLink" => IsSymLink
  | "Link" => Link
  | "Overflow" => Overflow
  | _ => Unknown
}

let set_callback = (handle, callback) => {
  let callback = events => {
    List.map(({path, string_flags}) => {
      {
        path,
        flags: List.map(flag_of_string, string_flags)
      }
    }, events)
    |> callback;
  };
  Callback.register("fswatch_callback", callback);
  set_callback(handle);
};

external start: handle => status = "_fsw_start_monitor";

external stop: handle => status = "_fsw_stop_monitor";
