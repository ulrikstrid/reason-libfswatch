#include <libfswatch_types.h>
#include <libfswatch.h>

#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/alloc.h>


CAMLprim value
_fsw_init_library(value unit)
{
    FSW_STATUS status = fsw_init_library();

    if (status != FSW_OK)
    {
        exit(1);
    }

    return (value) status;
}

CAMLprim value
_fsw_init_session(value m)
{
    enum fsw_monitor_type monitor;
    switch (Int_val(m)) {
        case 0: monitor = system_default_monitor_type; break;
        case 1: monitor = fsevents_monitor_type; break;
        case 2: monitor = kqueue_monitor_type; break;
        case 3: monitor = inotify_monitor_type; break;
        case 4: monitor = windows_monitor_type; break;
        case 5: monitor = poll_monitor_type; break;
        case 6: monitor = fen_monitor_type; break;
    }
    FSW_HANDLE handle = fsw_init_session(monitor);

    return (value) handle;
}

CAMLprim value
_fsw_add_path(FSW_HANDLE handle, const char * path)
{
    FSW_STATUS status = fsw_add_path(handle, path);
    return (value) status;
}

static value * closure_f = NULL;

void fswatch_callback(fsw_cevent const * const events, const unsigned int event_num, void * data)
{
    CAMLparam0();
    if (closure_f == NULL) {
        closure_f = caml_named_value("fswatch_callback");
    }

    // Create empty list for events
    CAMLlocal2( events_list, cons );
    events_list = Val_emptylist;

    unsigned int i;
    for (i = 0; i < event_num; i++)
    {
        // Allocate callback_event record
        cons = caml_alloc(2, 0);
        CAMLlocal1( callback_event );
        callback_event = caml_alloc( 2, 0 );
        
        // Store path in callback_event
        Store_field( callback_event, 0, caml_copy_string(events[i].path) );

        unsigned int flags_num = events[i].flags_num;
        // Create empty list for flags
        CAMLlocal2( flags_list, flags_cons );
        flags_list = Val_emptylist;

        unsigned int n;
        for (n = 0; n < flags_num; ++n)
        {
            flags_cons = caml_alloc(2, 0);

            // Store callback_event in events_list
            Store_field( flags_cons, 0, caml_copy_string(fsw_get_event_flag_name(events[i].flags[n])) ); // head
            Store_field( flags_cons, 1, flags_list ); // tail

            flags_list = flags_cons;
        }

        // Store flags in callback_event
        Store_field( callback_event, 1, flags_list);

        // Store callback_event in events_list
        Store_field( cons, 0, callback_event ); // head
        Store_field( cons, 1, events_list ); // tail

        events_list = cons;
    }

    caml_callback(*closure_f, events_list);
}

CAMLprim value
_fsw_set_callback(FSW_HANDLE handle, void * data)
{
    FSW_STATUS status = fsw_set_callback(handle, fswatch_callback, data);
    return (value) status;
}

CAMLprim value
_fsw_start_monitor(FSW_HANDLE handle)
{
    FSW_STATUS status = fsw_start_monitor(handle);
    return (value) status;
}

CAMLprim value
_fsw_stop_monitor(FSW_HANDLE handle)
{
    FSW_STATUS status = fsw_stop_monitor(handle);
    return (value) status;
}
