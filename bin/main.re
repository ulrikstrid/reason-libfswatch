open Reason_fswatch;

Libfswatch.init();

let handle = Libfswatch.make_handle(Libfswatch.Fsevents_monitor_type);
let _ = Libfswatch.add_path(handle, "./lib");
let _ = Libfswatch.add_path(handle, "./bin");

let _ =
  Libfswatch.set_callback(handle, events => {
    List.iter(
      ({path, flags}: Libfswatch.callback_event) => {
        print_endline(path);
        List.iter(
          flag =>
            Libfswatch.(
              switch (flag) {
              | NoOp => print_endline("NoOp")
              | PlatformSpecific => print_endline("PlatformSpecific")
              | Updated => print_endline("Updated")
              | Created => print_endline("Created")
              | Removed => print_endline("Removed")
              | Renamed => print_endline("Renamed")
              | IsFile => print_endline("IsFile")
              | _ => print_endline("other")
              }
            ),
          flags,
        );
      },
      events,
    )
  });

let _ = Libfswatch.start(handle);
