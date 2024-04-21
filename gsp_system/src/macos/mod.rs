use core::str;
use std::cell::Cell;
use std::ffi::CStr;

use std::collections::HashMap;
use std::ops::{Deref, DerefMut};
use std::os::raw::c_void;

use objc2::ffi::{BOOL, YES};
use objc2::rc::Id;
use objc2::runtime::{AnyObject, AnyProtocol, Bool, ClassBuilder, NSObject, ProtocolObject, Sel};
use objc2::{class, msg_send, msg_send_id, sel};
use objc2_app_kit::{NSWindow, NSWindowStyleMask};
use objc2_foundation::{ns_string, CGFloat, CGPoint, CGSize, NSRect, NSSize, NSString, NSUInteger};

use crate::internal::GspInternalWindowState;
use crate::*;

type CallbackFn = extern "C" fn();

extern "C" fn application_did_finish_launching(object: &mut AnyObject, _: Sel, _: &AnyObject) {
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

extern "C" fn window_will_resize_to(
    object: &mut AnyObject,
    _: Sel,
    _: &AnyObject,
    size: NSSize,
) -> NSSize {
    unsafe {
        let window_state_ivar = class!(WindowDelegate)
            .instance_variable("_window_state")
            .unwrap();
        let window_state: &mut Cell<usize> = window_state_ivar.load_mut::<Cell<usize>>(object);

        let window_state_ptr = window_state.get() as *mut GspInternalWindowState;
        println!("STATE PTR is {}", window_state.get());
        if !window_state_ptr.is_null() {
            let state = &mut *window_state_ptr;
            let new_size = (state.will_resize_callback)(
                state.window_handle,
                GspWindowSize {
                    width: size.width as i32,
                    height: size.height as i32,
                },
            );

            (state.resized_callback)(state.window_handle, new_size.clone());

            return NSSize {
                width: new_size.width as f64,
                height: new_size.height as f64,
            };
        }

        return size;
    }
}

extern "C" fn window_should_close(object: &mut AnyObject, _: Sel, _: &AnyObject) -> Bool {
    println!("WINDOW SHOULD CLOSE");
    unsafe {
        let window_state_ivar = class!(WindowDelegate)
            .instance_variable("_window_state")
            .unwrap();
        let window_state: &mut Cell<usize> = window_state_ivar.load_mut::<Cell<usize>>(object);

        let window_state_ptr = window_state.get() as *mut GspInternalWindowState;
        if !window_state_ptr.is_null() {
            let state = &mut *window_state_ptr;
            let should_close = (state.will_close_callback)(state.window_handle);

            if !should_close {
                return Bool::NO;
            }

            (state.closed_callback)(state.window_handle);

            return Bool::YES;
        }

        return Bool::YES;
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
        // AUTORELEASE POOL
        let _pool: Id<NSObject> =
            msg_send_id![msg_send_id![class!(NSAutoreleasePool), alloc], init];

        // APPLICATION
        let app: Id<NSObject> = msg_send_id![class!(NSApplication), sharedApplication];

        // WINDOW DELEGATE
        let mut window_delegate_decl =
            ClassBuilder::new("WindowDelegate", class!(NSObject)).unwrap();
        window_delegate_decl.add_ivar::<Cell<usize>>("_window_state");
        window_delegate_decl.add_method(
            sel!(windowWillResize:toSize:),
            window_will_resize_to as extern "C" fn(_, _, _, _) -> NSSize,
        );
        window_delegate_decl.add_method(
            sel!(windowShouldClose:),
            window_should_close as extern "C" fn(_, _, _) -> Bool,
        );
        let _ = window_delegate_decl.register();

        // APP DELEGATE

        let mut app_delegate_decl =
            ClassBuilder::new("ApplicationDelegate", class!(NSObject)).unwrap();

        app_delegate_decl.add_ivar::<Cell<GspAppLaunchCallback>>("_launch_callback");
        app_delegate_decl.add_method(
            sel!(applicationDidFinishLaunching:),
            application_did_finish_launching as extern "C" fn(_, _, _),
        );

        let delegate_class = app_delegate_decl.register();
        let delegate_object: &mut AnyObject = msg_send![delegate_class, alloc];
        let delegate_object: &mut AnyObject = msg_send![delegate_object, init];

        let launch_callback_ivar = delegate_class
            .instance_variable("_launch_callback")
            .unwrap();
        let launch_callback: &mut Cell<CallbackFn> =
            launch_callback_ivar.load_mut::<Cell<CallbackFn>>(delegate_object);
        launch_callback.set(app_info.launch_callback);

        let () = msg_send![&app, setDelegate: delegate_object];

        let () = msg_send![&app, run];

        return 0;
    }
}

#[no_mangle]
pub extern "C" fn gsp_create_window(creation_info: GspWindowInfo) -> GspWindowHandle {
    unsafe {
        let delegate_object: &mut AnyObject = msg_send![class!(WindowDelegate), alloc];
        let delegate_object: &mut AnyObject = msg_send![delegate_object, init];

        let window_title: &str = unsafe {
            match CStr::from_ptr(creation_info.title).to_str() {
                Ok(dev) => dev,
                Err(_) => "ERROR",
            }
        };

        let frame: NSRect = NSRect {
            origin: CGPoint::new(creation_info.x as CGFloat, creation_info.y as CGFloat),
            size: CGSize::new(
                creation_info.width as CGFloat,
                creation_info.height as CGFloat,
            ),
        };

        let window: Id<NSWindow> = msg_send_id![msg_send_id![class!(NSWindow), alloc], init];

        let window_state_ivar = class!(WindowDelegate)
            .instance_variable("_window_state")
            .unwrap();
        let window_state: &mut Cell<usize> =
            window_state_ivar.load_mut::<Cell<usize>>(delegate_object);
        let new_state: GspInternalWindowState = GspInternalWindowState {
            window_handle: Id::into_raw(window.clone()) as usize,
            ..Default::default()
        };
        window_state.set(Box::into_raw(Box::new(new_state)) as usize);

        let () = msg_send![&window, setDelegate: delegate_object];

        let title = NSString::from_str(window_title);

        window.setTitle(title.deref());

        let style_mask: NSUInteger = NSWindowStyleMask::Titled.0
            | NSWindowStyleMask::Closable.0
            | NSWindowStyleMask::Resizable.0
            | NSWindowStyleMask::Miniaturizable.0;
        window.setStyleMask(NSWindowStyleMask(style_mask));

        if !creation_info.hidden {
            window.makeKeyWindow();
            window.orderFront(None);
        }

        return Id::into_raw(window) as usize;
    }
}

#[no_mangle]
pub extern "C" fn gsp_set_window_resized_callback(
    window: GspWindowHandle,
    resize_callback: GspWindowResizedCallback,
) {
    unsafe {
        let window_ptr = window as *mut NSWindow;

        if window_ptr.is_null() {
            return;
        }

        let window = &*window_ptr;

        let delegate: *mut NSObject = Id::into_raw(window.delegate().unwrap()) as *mut NSObject;
        let delegate: Id<NSObject> = Id::from_raw(delegate).unwrap();
        let window_state_ivar = class!(WindowDelegate)
            .instance_variable("_window_state")
            .unwrap();
        let window_state: &Cell<usize> = window_state_ivar.load::<Cell<usize>>(delegate.as_ref());

        let window_state_ptr = window_state.get() as *mut GspInternalWindowState;
        if !window_state_ptr.is_null() {
            println!("STATE PTR OK");
            let state = &mut *window_state_ptr;
            state.resized_callback = resize_callback;
        } else {
            println!("STATE PTR ERROR {}", window_state.get());
        }
    }
}

#[no_mangle]
pub extern "C" fn gsp_set_window_will_resize_callback(
    window: GspWindowHandle,
    will_resize_callback: GspWindowWillResizeCallback,
) {
    unsafe {
        let window_ptr = window as *mut NSWindow;

        if window_ptr.is_null() {
            return;
        }

        let window = &*window_ptr;

        let delegate: *mut NSObject = Id::into_raw(window.delegate().unwrap()) as *mut NSObject;
        let delegate: Id<NSObject> = Id::from_raw(delegate).unwrap();
        let window_state_ivar = class!(WindowDelegate)
            .instance_variable("_window_state")
            .unwrap();
        let window_state: &Cell<usize> = window_state_ivar.load::<Cell<usize>>(delegate.as_ref());

        let window_state_ptr = window_state.get() as *mut GspInternalWindowState;
        if !window_state_ptr.is_null() {
            println!("STATE PTR OK");
            let state = &mut *window_state_ptr;
            state.will_resize_callback = will_resize_callback;
        } else {
            println!("STATE PTR ERROR {}", window_state.get());
        }
    }
}

#[no_mangle]
pub extern "C" fn gsp_set_window_closed_callback(
    window: GspWindowHandle,
    closed_callback: GspWindowClosedCallback,
) {
    unsafe {
        let window_ptr = window as *mut NSWindow;

        if window_ptr.is_null() {
            return;
        }

        let window = &*window_ptr;

        let delegate: *mut NSObject = Id::into_raw(window.delegate().unwrap()) as *mut NSObject;
        let delegate: Id<NSObject> = Id::from_raw(delegate).unwrap();
        let window_state_ivar = class!(WindowDelegate)
            .instance_variable("_window_state")
            .unwrap();
        let window_state: &Cell<usize> = window_state_ivar.load::<Cell<usize>>(delegate.as_ref());

        let window_state_ptr = window_state.get() as *mut GspInternalWindowState;
        if !window_state_ptr.is_null() {
            println!("STATE PTR OK");
            let state = &mut *window_state_ptr;
            state.closed_callback = closed_callback;
        } else {
            println!("STATE PTR ERROR {}", window_state.get());
        }
    }
}

#[no_mangle]
pub extern "C" fn gsp_set_window_will_close_callback(
    window: GspWindowHandle,
    will_close_callback: GspWindowWillCloseCallback,
) {
    unsafe {
        let window_ptr = window as *mut NSWindow;

        if window_ptr.is_null() {
            return;
        }

        let window = &*window_ptr;

        let delegate: *mut NSObject = Id::into_raw(window.delegate().unwrap()) as *mut NSObject;
        let delegate: Id<NSObject> = Id::from_raw(delegate).unwrap();
        let window_state_ivar = class!(WindowDelegate)
            .instance_variable("_window_state")
            .unwrap();
        let window_state: &Cell<usize> = window_state_ivar.load::<Cell<usize>>(delegate.as_ref());

        let window_state_ptr = window_state.get() as *mut GspInternalWindowState;
        if !window_state_ptr.is_null() {
            println!("STATE PTR OK");
            let state = &mut *window_state_ptr;
            state.will_close_callback = will_close_callback;
        } else {
            println!("STATE PTR ERROR {}", window_state.get());
        }
    }
}
