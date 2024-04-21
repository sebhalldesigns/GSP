use gsp_system::*;
use windows_sys::Win32::System::LibraryLoader::GetModuleHandleA;
use std::{cell::Cell, ffi::c_char};
use raw_window_handle::{DisplayHandle, HandleError, HasDisplayHandle, HasWindowHandle, RawWindowHandle};
use futures::executor::block_on;

use lazy_static::lazy_static; // 1.4.0
use std::sync::Mutex;

lazy_static! {
    static ref SURFACE: Mutex<Cell<usize>> = Mutex::new(Cell::new(0));
    static ref QUEUE: Mutex<Cell<usize>> = Mutex::new(Cell::new(0));
    static ref DEVICE: Mutex<Cell<usize>> = Mutex::new(Cell::new(0));
    static ref CONFIG: Mutex<Cell<usize>> = Mutex::new(Cell::new(0));
}


pub struct WpguWindowHandle {
    pub raw: usize
}

impl HasWindowHandle for WpguWindowHandle {
    fn window_handle(&self) -> Result<wgpu::rwh::WindowHandle<'_>, wgpu::rwh::HandleError> {
        unsafe {
            let win32_handle = wgpu::rwh::Win32WindowHandle::new( core::num::NonZeroIsize::new(self.raw as isize).unwrap());
            let raw_window = wgpu::rwh::RawWindowHandle::Win32(win32_handle);

            
            return Ok(wgpu::rwh::WindowHandle::borrow_raw(raw_window));
        }
    }
}

impl HasDisplayHandle for WpguWindowHandle {
    fn display_handle(&self) -> Result< wgpu::rwh::DisplayHandle<'_>, wgpu::rwh::HandleError> {
        println!("DISPLAY CALLED");
        return Ok(DisplayHandle::windows());
    }
}

extern "C" fn launch_callback() {
    println!("app launched");

    let creation_info = GspWindowInfo {
        title: "GSP_GPI Test!\0".as_ptr() as *const c_char,
        width: 800,
        height: 600,
        x: 0,
        y: 0,
        hidden: false
    };

    let _window = gsp_create_window(creation_info);
    println!("WINDOW CRETATED WITH HANDLE {}", _window);


    let instance = wgpu::Instance::new(wgpu::InstanceDescriptor {
        backends: wgpu::Backends::all(),
        ..Default::default()
    });
    
    let wgpu_handle: WpguWindowHandle = WpguWindowHandle {
        raw: _window
    };

    println!("Made handle");

    let surface = match instance.create_surface(wgpu_handle) {
        Ok(s) => s,
        Err(error) => {
            println!("{}", error);
            return;
        }
    };

    println!("Created surface");

    let adapter = block_on(instance.request_adapter(
        &wgpu::RequestAdapterOptions {
            power_preference: wgpu::PowerPreference::default(),
            compatible_surface: Some(&surface),
            force_fallback_adapter: false,
        },
    )).expect("an eror occurred");

    let (device, queue) = block_on(adapter.request_device(
        &wgpu::DeviceDescriptor {
            ..Default::default()
        },
        None, // Trace path
    )).expect("");

    let surface_caps = surface.get_capabilities(&adapter);
    // Shader code in this tutorial assumes an sRGB surface texture. Using a different
    // one will result in all the colors coming out darker. If you want to support non
    // sRGB surfaces, you'll need to account for that when drawing to the frame.
    let surface_format = surface_caps.formats.iter()
        .copied()
        .filter(|f| f.is_srgb())
        .next()
        .unwrap_or(surface_caps.formats[0]);
    let config = wgpu::SurfaceConfiguration {
        usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
        format: surface_format,
        width: 2560,
        height: 1440,
        present_mode: surface_caps.present_modes[0],
        alpha_mode: surface_caps.alpha_modes[0],
        view_formats: vec![],
        desired_maximum_frame_latency: 2
    };

    surface.configure(&device, &config);

    gsp_set_window_resized_callback(_window, window_resized);

    let surf_raw = Box::into_raw(Box::new(surface)) as usize;

    SURFACE.lock().unwrap().set(surf_raw);

    let queue_raw = Box::into_raw(Box::new(queue)) as usize;
    QUEUE.lock().unwrap().set(queue_raw);

    let device_raw = Box::into_raw(Box::new(device)) as usize;
    DEVICE.lock().unwrap().set(device_raw);

    let config_raw = Box::into_raw(Box::new(config)) as usize;
    CONFIG.lock().unwrap().set(config_raw);

}

extern "C" fn window_resized(window: GspWindowHandle, size: GspWindowSize) {
    println!("RESIZED IN GPI");


    
    unsafe {





        let wgpu_handle: WpguWindowHandle = WpguWindowHandle {
            raw: window
        };
    
        let surf_raw = SURFACE.lock().unwrap().get() as *mut wgpu::Surface;
        let surface = &*surf_raw;

        let queue_raw = QUEUE.lock().unwrap().get() as *mut wgpu::Queue;
        let queue = &*queue_raw;

        let device_raw = DEVICE.lock().unwrap().get() as *mut wgpu::Device;
        let mut device = &*device_raw;
            
        let config_raw = CONFIG.lock().unwrap().get() as *mut wgpu::SurfaceConfiguration;
        let mut config = &mut *config_raw;


        //config.width = 2560;//size.width as u32;
        //config.height = 1440;//size.height as u32;

        //surface.configure(device, &config);

        let output = surface.get_current_texture().unwrap();

        let view = output.texture.create_view(&wgpu::TextureViewDescriptor::default());




        let mut encoder = device.create_command_encoder(&wgpu::CommandEncoderDescriptor {
            label: Some("Render Encoder"),
        });
    
        {
            let _render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
                label: Some("Render Pass"),
                color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                    view: &view,
                    resolve_target: None,
                    ops: wgpu::Operations {
                        load: wgpu::LoadOp::Clear(wgpu::Color {
                            r: 0.1,
                            g: 0.2,
                            b: 0.3,
                            a: 1.0,
                        }),
                        store: wgpu::StoreOp::Store,
                    },
                })],
                depth_stencil_attachment: None,
                occlusion_query_set: None,
                timestamp_writes: None,
            });
        }
        
 
            

        // submit will accept anything that implements IntoIter
        queue.submit(std::iter::once(encoder.finish()));
        output.present();
    
     

    }
    

}

fn main() {
    println!("hello gsp_gpi");

    let app_info: GspAppInfo = GspAppInfo {
        name: "GSP_GPI Test!\0".as_ptr() as *const c_char,
        developer: "Seb Hall\0".as_ptr() as *const c_char,
        version: GspVersion { major: 0, minor: 1, build: 1},
        launch_callback: launch_callback
    };

    let _ = gsp_run(app_info);
}