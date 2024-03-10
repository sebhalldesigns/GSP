extern crate queues;

use std::{os::unix::thread, thread::*};

use queues::*;

use crate::interface::{path::VgPath, shapes::VgShape, VgPixelBuffer};



// state
pub struct VgEngineState {
    pub max_threads: usize,
    pub thread_pool: Vec<Thread>,

    pub layers: Vec<VgEngineLayer>
}

// iterate through enum with each pass until all layers are raster
pub enum VgEngineLayer {
    SHAPE (VgShape),
    PATH (VgPath),
    RASTER
}

pub fn engine_init() -> *mut VgEngineState {

    let thread_count: usize = std::thread::available_parallelism().unwrap().get();


    let new_state = VgEngineState {
        max_threads: thread_count,
        thread_pool: Vec::new(),
        layers: Vec::new()
    };

    return Box::into_raw(Box::new(new_state));
}

pub fn engine_reset(raw_state: *mut VgEngineState) {
    println!("VG ENGINE: RESET");

    let state: &mut VgEngineState = unsafe { &mut *raw_state };
    state.layers.clear();
}

pub fn engine_render(raw_state: *mut VgEngineState, buffer: VgPixelBuffer) {
    println!("VG ENGINE: RENDER");

    let state: &mut VgEngineState = unsafe { &mut *raw_state };
    


    // dont return until all calculations are finished.

}

fn engine_main() {
    let thread_count: usize = std::thread::available_parallelism().unwrap().get();

    println!("Welcome to GVG, the lightweight vector graphics library!\n Found {:?} CPUs on your system.", thread_count);
}



fn worker_thread_fn() {

}