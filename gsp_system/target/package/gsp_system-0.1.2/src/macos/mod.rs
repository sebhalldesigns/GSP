use std::cell::Cell;

use std::os::raw::c_void;

use objc2::rc::Id;
use objc2::runtime::{AnyObject, ClassBuilder, NSObject, Sel};
use objc2::{class, msg_send, msg_send_id, sel};

type CallbackFn = extern "C" fn();

extern "C" fn application_did_finish_launching(object: &AnyObject, _: Sel, _: &AnyObject) {
    println!("[COCOA-CALLBACK]: applicationDidFinishLaunching:");

    let delegate_class = class!(ApplicationDelegate);
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
pub extern "C" fn gsp_init(launch_callback: CallbackFn) -> bool {
    println!("GSP INIT FROM RUST");

    unsafe {
        let _pool: Id<NSObject> =
            msg_send_id![msg_send_id![class!(NSAutoreleasePool), alloc], init];

        let app: Id<NSObject> = msg_send_id![class!(NSApplication), sharedApplication];

        let superclass = class!(NSObject);
        let mut decl = ClassBuilder::new("ApplicationDelegate", superclass).unwrap();

        decl.add_ivar::<Cell<CallbackFn>>("_launch_callback");
        decl.add_method(
            sel!(applicationDidFinishLaunching:),
            application_did_finish_launching as extern "C" fn(_, _, _),
        );

        let delegate_class = decl.register();
        let delegate_object: &mut AnyObject = msg_send![delegate_class, alloc];
        let delegate_object: &mut AnyObject = msg_send![delegate_object, init];
        let ivar = delegate_class
            .instance_variable("_launch_callback")
            .unwrap();
        let number: &mut Cell<CallbackFn> = ivar.load_mut::<Cell<CallbackFn>>(delegate_object);
        number.set(launch_callback);

        let () = msg_send![&app, setDelegate: delegate_object];
    }

    return true;
}

#[no_mangle]
pub extern "C" fn gsp_run() -> std::os::raw::c_int {
    println!("GSP RUN");

    unsafe {
        let app: Id<NSObject> = msg_send_id![class!(NSApplication), sharedApplication];

        // run the app
        let () = msg_send![&app, run];
    }

    return 0;
}
