use core::str;
use std::cell::Cell;
use std::ffi::CStr;

use std::collections::HashMap;
use std::os::raw::c_void;

use objc2::rc::Id;
use objc2::runtime::{AnyObject, ClassBuilder, NSObject, Sel};
use objc2::{class, msg_send, msg_send_id, sel};

use crate::internal::GspInternalWindowState;
use crate::*;

type CallbackFn = extern "C" fn();

extern "C" fn application_did_finish_launching(object: &mut AnyObject, _: Sel, _: &AnyObject) {
    println!("[COCOA-CALLBACK]: applicationDidFinishLaunching:");
    let delegate_class = class!(ApplicationDelegate);

    unsafe {
        let window_states_ivar = delegate_class.instance_variable("_window_states").unwrap();
        let window_states: &mut Cell<usize> = window_states_ivar.load_mut::<Cell<usize>>(object);

        let hash: HashMap<GspWindowHandle, GspInternalWindowState> = HashMap::new();
        window_states.set(Box::into_raw(Box::new(hash)) as usize);
    }

    let ivar = delegate_class
        .instance_variable("_launch_callback")
        .unwrap();
    unsafe {
        let number: &Cell<CallbackFn> = ivar.load::<Cell<CallbackFn>>(object);
        let callback = number.get();
        callback();
    }
}

#[no_mangle]
pub extern "C" fn gsp_run(app_info: GspAppInfo) -> i32 {
    println!("OpenGSP v0.1.0 Alpha");

    // try to get app name from c string
    let app_name: &str = unsafe {
        match CStr::from_ptr(app_info.name).to_str() {
            Ok(name) => name,
            Err(_) => "ERROR",
        }
    };

    // try to get app developer from c string
    let app_developer: &str = unsafe {
        match CStr::from_ptr(app_info.developer).to_str() {
            Ok(dev) => dev,
            Err(_) => "ERROR",
        }
    };

    println!("Starting application '{}'", app_name);
    println!(
        "v{}.{}.{}, developed by '{}'",
        app_info.version.major, app_info.version.minor, app_info.version.build, app_developer
    );

    unsafe {
        let _pool: Id<NSObject> =
            msg_send_id![msg_send_id![class!(NSAutoreleasePool), alloc], init];

        let app: Id<NSObject> = msg_send_id![class!(NSApplication), sharedApplication];

        let superclass = class!(NSObject);
        let mut decl = ClassBuilder::new("ApplicationDelegate", superclass).unwrap();

        decl.add_ivar::<Cell<GspAppLaunchCallback>>("_launch_callback");
        decl.add_ivar::<Cell<usize>>("_window_states");
        decl.add_method(
            sel!(applicationDidFinishLaunching:),
            application_did_finish_launching as extern "C" fn(_, _, _),
        );

        let delegate_class = decl.register();
        let delegate_object: &mut AnyObject = msg_send![delegate_class, alloc];
        let delegate_object: &mut AnyObject = msg_send![delegate_object, init];

        let launch_callback_ivar = delegate_class
            .instance_variable("_launch_callback")
            .unwrap();
        let launch_callback: &mut Cell<CallbackFn> =
            launch_callback_ivar.load_mut::<Cell<CallbackFn>>(delegate_object);
        launch_callback.set(app_info.launch_callback);

        let () = msg_send![&app, setDelegate: delegate_object];

        return msg_send![&app, run];
    }
}
