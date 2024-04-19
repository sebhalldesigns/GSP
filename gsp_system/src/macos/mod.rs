use libc::c_int;
use objc2::rc::{Allocated, Id};
use objc2::{
    class, declare_class, extern_protocol, msg_send, msg_send_id, mutability,
    runtime::{AnyObject, ClassBuilder, NSObject, NSObjectProtocol, NSZone, Sel, BOOL},
    sel, ClassType, DeclaredClass, Encode, Encoding, ProtocolType,
};

use objc2::ffi::NSUInteger;

use objc2_foundation::{ns_string, MainThreadMarker, NSCopying, NSNotification, NSString};

#[link(name = "Cocoa", kind = "framework")]
extern "C" {}

//type NSApplicationDelegate = NSObject;

extern_protocol!(
    pub unsafe trait NSApplicationDelegate: NSObjectProtocol {
        #[optional]
        #[method(applicationDidFinishLaunching:)]
        unsafe fn applicationDidFinishLaunching(&self, notification: &NSObject);
    }

    unsafe impl ProtocolType for dyn NSApplicationDelegate {
        const NAME: &'static str = "NSApplicationDelegate";

        //fn protocol() -> Option<
    }
);

#[derive(Debug)]
#[allow(unused)]
struct Ivars {
    ivar: u8,
    another_ivar: bool,
    box_ivar: Box<i32>,
    maybe_box_ivar: Option<Box<i32>>,
    id_ivar: Id<NSString>,
    maybe_id_ivar: Option<Id<NSString>>,
}

declare_class!(
    struct AppDelegate;

    // SAFETY:
    // - The superclass NSObject does not have any subclassing requirements.
    // - Main thread only mutability is correct, since this is an application delegate.
    // - `AppDelegate` does not implement `Drop`.
    unsafe impl ClassType for AppDelegate {
        type Super = NSObject;
        type Mutability = mutability::MainThreadOnly;
        const NAME: &'static str = "MyAppDelegate";
    }

    impl DeclaredClass for AppDelegate {
        type Ivars = Ivars;
    }

    unsafe impl NSObjectProtocol for AppDelegate {}

    unsafe impl NSApplicationDelegate for AppDelegate {
        #[method(applicationDidFinishLaunching:)]
        fn did_finish_launching(&self, notification: &NSObject) {
            println!("Did finish launching!");
            // Do something with the notification
            dbg!(notification);
        }
    }
);

impl AppDelegate {
    fn new(ivar: u8, another_ivar: bool, mtm: MainThreadMarker) -> Id<Self> {
        let this = mtm.alloc();
        let this = this.set_ivars(Ivars {
            ivar,
            another_ivar,
            box_ivar: Box::new(2),
            maybe_box_ivar: None,
            id_ivar: NSString::from_str("abc"),
            maybe_id_ivar: Some(ns_string!("def").copy()),
        });
        unsafe { msg_send_id![super(this), init] }
    }
}

pub fn run() {
    let ns_autorelease_pool = class!(NSAutoreleasePool);
    let ns_application = class!(NSApplication);
    let ns_application_delegate = class!(NSApplicationDelegate);

    let pool: Id<NSObject> =
        unsafe { msg_send_id![msg_send_id![ns_autorelease_pool, alloc], init] };

    let app: Id<NSObject> = unsafe { msg_send_id![ns_application, sharedApplication] };

    let mut decl = ClassBuilder::new("MyApplicationDelegate", ns_application_delegate).unwrap();

    extern "C" fn application_launched(_: &mut AnyObject, _: Sel, _: &NSObject) {
        println!("Application Launched!");
    }

    unsafe {
        decl.add_method(
            sel!(applicationDidFinishLaunching:),
            application_launched as extern "C" fn(_, _, _),
        );
    }

    let delegate_class = decl.register();
    let delegate_object: Id<NSObject> =
        unsafe { msg_send_id![msg_send_id![delegate_class, alloc], init] };

    // Set the AppDelegate instance as the delegate of NSApplication
    unsafe {
        let () = msg_send![&app, setDelegate: Id::into_raw(delegate_object)];
    }

    // void msg_send
    let () = unsafe { msg_send![&app, run] };
}

pub fn print_stuff() {}
