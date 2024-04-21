// lib.rs - gsp_system crate
// Created on 2024-04-20 by Seb Hall
//
// This file is a part of the OpenGSP project, which aims
// to provide a high-performance, intuitive and flexible framework
// for cross-platform application development.

// Platform-Specific Code:

#[cfg(target_os = "macos")]
pub use self::macos::*;
#[cfg(target_os = "macos")]
mod macos;

#[cfg(target_os = "windows")]
pub use self::windows::*;
#[cfg(target_os = "windows")]
mod windows;

mod internal;

// Top-Level Library Interface (replicates C header)

use std::os::raw::c_char;

#[repr(C)]
pub struct GspVersion {
    major: u32,
    minor: u32,
    build: u32,
<<<<<<< HEAD
    pub major: u32,
    pub minor: u32,
    pub build: u32
=======
>>>>>>> c45711ba5b31de0951a9cd950b43392694d06189
}

pub type GspAppLaunchCallback = extern "C" fn();

#[repr(C)]
pub struct GspAppInfo {
    pub name: *const c_char,
    pub developer: *const c_char,
    pub version: GspVersion,
    pub launch_callback: GspAppLaunchCallback,
}

#[derive(Clone, Copy)]
#[repr(C)]
pub struct GspWindowSize {
    pub width: i32,
    pub height: i32,
}

pub type GspWindowHandle = usize;
pub type GspWindowResizedCallback = extern "C" fn(window: GspWindowHandle, new_size: GspWindowSize);
pub type GspWindowWillResizeCallback =
    extern "C" fn(window: GspWindowHandle, requested_size: GspWindowSize) -> GspWindowSize;
pub type GspWindowClosedCallback = extern "C" fn(window: GspWindowHandle);
pub type GspWindowWillCloseCallback = extern "C" fn(window: GspWindowHandle) -> bool;
<<<<<<< HEAD

=======
>>>>>>> c45711ba5b31de0951a9cd950b43392694d06189

#[repr(C)]
pub struct GspWindowInfo {
    pub title: *const c_char,
    pub width: i32,
    pub height: i32,
    pub x: i32,
    pub y: i32,
    pub hidden: bool,
}
