use std::ffi::CString;
use std::os::raw::c_char;


#[no_mangle]
pub extern "C" fn send_msg() -> *mut c_char {

    let rust_string = "Hello from Rust!";

    let c_string = CString::new(rust_string).expect("Failed to create C string");

    return c_string.into_raw();
}

