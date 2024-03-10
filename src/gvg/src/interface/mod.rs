pub mod painting;
pub mod shapes;
pub mod path;

pub use std::os::raw::c_void;
pub use crate::engine::*;


#[repr(C)]
pub struct VgPixelBuffer {
    pub width: usize,
    pub height: usize,
    pub data: *mut u32 
}


#[no_mangle]
pub extern "C" fn vgpixelbuffer_alloc(width: usize, height: usize) -> VgPixelBuffer {

    let pixel_data = Box::into_raw(vec![0xFFFFFFFFu32; width * height].into_boxed_slice()) as *mut u32;

    return VgPixelBuffer {
        width: width,
        height: height,
        data: pixel_data
    };
}

// init & alloc engine
#[no_mangle]
pub extern "C" fn vg_init() -> *mut c_void {
    return engine_init() as *mut c_void;
}

// dealloc engine
#[no_mangle]
pub extern "C" fn vg_final(state: *mut c_void) {

}

// reset drawing commands
#[no_mangle]
pub extern "C" fn vg_reset(state: *mut c_void) {
    //engine_update(state as *mut VgEngineState);
}

// render commands to buffer
#[no_mangle]
pub extern "C" fn vg_render(state: *mut c_void, buffer: VgPixelBuffer) {
    //engine_update(state as *mut VgEngineState);
}


// add shape command
#[no_mangle] 
pub extern "C" fn vg_add_shape(state: *mut c_void, shape: shapes::VgShape) {

}

// add path command
#[no_mangle] 
pub extern "C" fn vg_add_path(state: *mut c_void, shape: path::VgPath) {
    
}

