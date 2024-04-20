use std::cell::Cell;

use objc2::rc::Id;
use objc2::runtime::{AnyObject, ClassBuilder, NSObject, Sel};
use objc2::{class, msg_send, msg_send_id, sel};

#[link(name = "Cocoa", kind = "framework")]
extern "C" {}

extern "C" fn application_did_finish_launching(object: &AnyObject, _: Sel, _: &AnyObject) {
    let delegate_class = class!(MyApplicationDelegate);
    let ivar = delegate_class.instance_variable("_number").unwrap();
    unsafe {
        let number: &Cell<usize> = ivar.load::<Cell<usize>>(object);
        println!("Finished launching! {}", number.get());
    }
}

pub fn run() {
    unsafe {
        let _pool: Id<NSObject> =
            msg_send_id![msg_send_id![class!(NSAutoreleasePool), alloc], init];

        let app: Id<NSObject> = msg_send_id![class!(NSApplication), sharedApplication];

        let superclass = class!(NSObject);
        let mut decl = ClassBuilder::new("MyApplicationDelegate", superclass).unwrap();

        decl.add_ivar::<Cell<usize>>("_number");
        decl.add_method(
            sel!(applicationDidFinishLaunching:),
            application_did_finish_launching as extern "C" fn(_, _, _),
        );

        let delegate_class = decl.register();
        let delegate_object: &mut AnyObject = msg_send![delegate_class, alloc];
        let delegate_object: &mut AnyObject = msg_send![delegate_object, init];
        let ivar = delegate_class.instance_variable("_number").unwrap();
        let number: &mut Cell<usize> = ivar.load_mut::<Cell<usize>>(delegate_object);
        number.set(100);

        let () = msg_send![&app, setDelegate: delegate_object];

        // run the app
        let () = msg_send![&app, run];
    }
}

pub fn print_stuff() {}
