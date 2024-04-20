use std::collections::HashMap;
use std::vec::Vec;
use std::{cell::Cell, sync::Mutex};

use crate::*;

pub struct GspInternalWindowState {
    pub resized_callback: GspWindowResizedCallback,
    pub will_resize_callback: GspWindowWillResizeCallback,
    pub closed_callback: GspWindowClosedCallback,
    pub will_close_callback: GspWindowWillCloseCallback,
}

// Define default callback functions
extern "C" fn default_resized_callback(window: GspWindowHandle, new_size: GspWindowSize) {
    println!("[GSP-SYSTEM]: Default resized_callback");
}

extern "C" fn default_will_resize_callback(
    window: GspWindowHandle,
    requested_size: GspWindowSize,
) -> GspWindowSize {
    println!("[GSP-SYSTEM]: Default will_resize_callback");
    requested_size
}

extern "C" fn default_closed_callback(window: GspWindowHandle) {
    println!("[GSP-SYSTEM]: Default closed_callback");
}

extern "C" fn default_will_close_callback(window: GspWindowHandle) -> bool {
    println!("[GSP-SYSTEM]: Default will_close_callback");
    true
}

impl Default for GspInternalWindowState {
    fn default() -> Self {
        GspInternalWindowState {
            resized_callback: default_resized_callback,
            will_resize_callback: default_will_resize_callback,
            closed_callback: default_closed_callback,
            will_close_callback: default_will_close_callback,
        }
    }
}
