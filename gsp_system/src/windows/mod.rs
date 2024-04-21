use core::str;
use std::ffi::CStr;

use windows_sys::{
    core::*, Win32::Foundation::*,  
    Win32::UI::WindowsAndMessaging::*, Win32::System::LibraryLoader::GetModuleHandleA, 
    Win32::Graphics::Gdi::*
};

use crate::internal::GspInternalWindowState;
use crate::*;


fn loword(x: u32) -> u16 {
    return (x & 0xFFFF) as u16;
}


fn hiword(x: u32) -> u16 {
    return ((x >> 16) & 0xFFFF) as u16;
}

#[no_mangle]
pub extern "C" fn gsp_run(app_info: GspAppInfo) -> i32 {
    
    println!("OpenGSP v0.1.0 Alpha");

    // try to get app name from c string
    let app_name: &str = unsafe {
        match CStr::from_ptr(app_info.name).to_str() {
            Ok(name) => name,
            Err(_) => "ERROR"
        }
    };

    // try to get app developer from c string
    let app_developer: &str = unsafe {
        match CStr::from_ptr(app_info.developer).to_str() {
            Ok(dev) => dev,
            Err(_) => "ERROR"
        }
    };

    println!("Starting application '{}'", app_name);
    println!("v{}.{}.{}, developed by '{}'", app_info.version.major, app_info.version.minor, app_info.version.build, app_developer);

    // Register the window class

    unsafe {
        let wnd_class =  WNDCLASSA {
            style: CS_HREDRAW | CS_VREDRAW,
            lpfnWndProc: Some(window_proc),
            hInstance: GetModuleHandleA(0 as *const u8), // The instance handle of the current executable
            cbClsExtra: 0,
            cbWndExtra: 0,
            hIcon: HICON::default(),
            hCursor: HCURSOR::default(),
            hbrBackground: HBRUSH::default(),
            lpszClassName: s!("WindowClass"),
            lpszMenuName: s!("")
            
        };
    
        RegisterClassA(&wnd_class);
    }



    
    (app_info.launch_callback)();

    // Message loop
    unsafe {

        let mut msg = MSG {
            hwnd: 0,
            message: 0,
            wParam: 0,
            lParam: 0,
            time: 0,
            pt: POINT { x: 0, y: 0 }
        };

        while GetMessageA(&mut msg, 0, 0, 0) != 0 {

            TranslateMessage(&msg);
            DispatchMessageA(&msg);

        }
    }


    return 0;
}

#[no_mangle]
pub extern "C" fn gsp_create_window(creation_info: GspWindowInfo) -> GspWindowHandle {

    println!("create window");

    unsafe {
        let hwnd = CreateWindowExA(
            0,
            s!("WindowClass"),
            creation_info.title as _,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            creation_info.width,
            creation_info.height,
            creation_info.x as isize,
            creation_info.y as isize,
            GetModuleHandleA(0 as *const u8),
            std::ptr::null_mut(),
        );
        println!("Created window");
        println!("Created window with id {}", hwnd.clone());
        
        let window_id: usize = hwnd as usize; 

        println!("got to here");

        UpdateWindow(window_id as HWND);
        //Show and run the window
        ShowWindow(window_id as HWND,  SW_SHOWDEFAULT);
        //

        
        if window_id == 0 {
            println!("window was 0");
            return 0;
        }

        return window_id;  
    }
}

#[no_mangle]
pub extern "C" fn gsp_set_window_resized_callback(window: GspWindowHandle, resize_callback: GspWindowResizedCallback) {

    unsafe {
        if window == 0 || IsWindow(window as isize) == 0 {
            println!("[GSP-SYSTEM] [WARNING] window was invalid {}", window);
            return;
        }
        
        let window_info_ptr_raw = GetWindowLongPtrA(window as HWND, GWLP_USERDATA) as *mut GspInternalWindowState;

        // Ensure the raw pointer is not null before dereferencing
        if !window_info_ptr_raw.is_null() {
            let mut window_info_ptr = &mut *window_info_ptr_raw;
            window_info_ptr.resized_callback = resize_callback;
        } else {
            println!("[GSP-SYSTEM] [WARNING] window pointer was invalid {}", window);
        }

    }

}

#[no_mangle]
pub extern "C" fn gsp_set_window_will_resize_callback(window: GspWindowHandle, will_resize_callback: GspWindowWillResizeCallback) {
    
    unsafe {
        if window == 0 || IsWindow(window as isize) == 0 {
            println!("[GSP-SYSTEM] [WARNING] window was invalid {}", window);
            return;
        }
        
        let window_info_ptr_raw = GetWindowLongPtrA(window as HWND, GWLP_USERDATA) as *mut GspInternalWindowState;

        // Ensure the raw pointer is not null before dereferencing
        if !window_info_ptr_raw.is_null() {
            let mut window_info_ptr = &mut *window_info_ptr_raw ;
            window_info_ptr.will_resize_callback = will_resize_callback;
        } else {
            println!("[GSP-SYSTEM] [WARNING] window pointer was invalid {}", window);
        }

    }
    
}

#[no_mangle]
pub extern "C" fn gsp_set_window_closed_callback(window: GspWindowHandle, closed_callback: GspWindowClosedCallback) {
    unsafe {
        if window == 0 || IsWindow(window as isize) == 0 {
            println!("[GSP-SYSTEM] [WARNING] window was invalid {}", window);
            return;
        }
        
        let window_info_ptr_raw = GetWindowLongPtrA(window as HWND, GWLP_USERDATA) as *mut GspInternalWindowState;

        // Ensure the raw pointer is not null before dereferencing
        if !window_info_ptr_raw.is_null() {
            let mut window_info_ptr = &mut *window_info_ptr_raw ;
            window_info_ptr.closed_callback = closed_callback;
        } else {
            println!("[GSP-SYSTEM] [WARNING] window pointer was invalid {}", window);
        }

    }
}

#[no_mangle]
pub extern "C" fn gsp_set_window_will_close_callback(window: GspWindowHandle, will_close_callback: GspWindowWillCloseCallback) {
    
    unsafe {
        if window == 0 || IsWindow(window as isize) == 0 {
            println!("[GSP-SYSTEM] [WARNING] window was invalid {}", window);
            return;
        }
        
        let window_info_ptr_raw = GetWindowLongPtrA(window as HWND, GWLP_USERDATA) as *mut GspInternalWindowState;

        // Ensure the raw pointer is not null before dereferencing
        if !window_info_ptr_raw.is_null() {
            let mut window_info_ptr = &mut *window_info_ptr_raw;
            window_info_ptr.will_close_callback = will_close_callback;
        } else {
            println!("[GSP-SYSTEM] [WARNING] window pointer was invalid {}", window);
        }

    }
}

unsafe extern "system" fn window_proc(
    hwnd: HWND,
    msg: u32,
    w_param: WPARAM,
    l_param: LPARAM,
) -> LRESULT {

   match msg {
        WM_CREATE => {
            // allocate and attach window state
            let window_info_ptr: *mut GspInternalWindowState = Box::into_raw(Box::new(GspInternalWindowState::default()));
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, window_info_ptr as isize);
        },
        WM_SIZING => {
            if hwnd == 0 || IsWindow(hwnd as isize) == 0 {
                println!("[GSP-SYSTEM] [WARNING] window was invalid {}", hwnd);
                return DefWindowProcA(hwnd, msg, w_param, l_param);
            }
            
            let window_info_ptr_raw = unsafe { GetWindowLongPtrA(hwnd, GWLP_USERDATA) as *mut GspInternalWindowState };
    
            // Ensure the raw pointer is not null before dereferencing
            if !window_info_ptr_raw.is_null() {
                let window_info_ptr = unsafe { &mut *window_info_ptr_raw };

                let mut rect: RECT = std::mem::zeroed();

                let new_rect = l_param as *const RECT;
                if !new_rect.is_null() {
                    rect = *new_rect;

                    let new_size = (window_info_ptr.will_resize_callback)(hwnd as usize,  GspWindowSize { width: rect.right - rect.left as i32, height: rect.bottom - rect.top as i32});

                    // Adjust width and height to the nearest 10 pixels
                    rect.right = rect.left + new_size.width;
                    rect.bottom = rect.top + new_size.height;
                    *(l_param as *mut RECT) = rect;
                }

            } else {
                println!("[GSP-SYSTEM] [WARNING] window pointer was invalid {}", hwnd);
            }
        },
        WM_SIZE => {
            if hwnd == 0 || IsWindow(hwnd as isize) == 0 {
                println!("[GSP-SYSTEM] [WARNING] window was invalid {}", hwnd);
                return DefWindowProcA(hwnd, msg, w_param, l_param);
            }
            
            let window_info_ptr_raw = unsafe { GetWindowLongPtrA(hwnd, GWLP_USERDATA) as *mut GspInternalWindowState };
    
            // Ensure the raw pointer is not null before dereferencing
            if !window_info_ptr_raw.is_null() {
                let window_info_ptr = unsafe { &mut *window_info_ptr_raw };
                (window_info_ptr.resized_callback)(hwnd as usize, GspWindowSize { width: loword(l_param as u32) as i32, height: hiword(l_param as u32) as i32});
            } else {
                println!("[GSP-SYSTEM] [WARNING] window pointer was invalid {}", hwnd);
            }
        },
        WM_CLOSE => {
            if hwnd == 0 || IsWindow(hwnd as isize) == 0 {
                println!("[GSP-SYSTEM] [WARNING] window was invalid {}", hwnd);
                return DefWindowProcA(hwnd, msg, w_param, l_param);
            }
            
            let window_info_ptr_raw = unsafe { GetWindowLongPtrA(hwnd, GWLP_USERDATA) as *mut GspInternalWindowState };
    
            // Ensure the raw pointer is not null before dereferencing
            if !window_info_ptr_raw.is_null() {

                let window_info_ptr = unsafe { &mut *window_info_ptr_raw };
                
                // CALL: WILL CLOSE CALLBACK
                if !(window_info_ptr.will_close_callback)(hwnd as usize) {
                    // close has been intercepted, so prevent from closing
                    return 0;
                }
            } else {
                println!("[GSP-SYSTEM] [WARNING] window pointer was invalid {}", hwnd);
            }
        },
        WM_DESTROY => {

            if hwnd == 0 || IsWindow(hwnd as isize) == 0 {
                println!("[GSP-SYSTEM] [WARNING] window was invalid {}", hwnd);
                return DefWindowProcA(hwnd, msg, w_param, l_param);
            }
            
            let window_info_ptr_raw = unsafe { GetWindowLongPtrA(hwnd, GWLP_USERDATA) as *mut GspInternalWindowState };
    
            // Ensure the raw pointer is not null before dereferencing
            if !window_info_ptr_raw.is_null() {

                let window_info_ptr = &mut *window_info_ptr_raw;
                
                // CALL: CLOSED CALLBACK
                (window_info_ptr.closed_callback)(hwnd as usize);
                    
                // this will drop the window state memory when it goes out of scope
                let _ = Box::from_raw(window_info_ptr_raw);

            } else {
                println!("[GSP-SYSTEM] [WARNING] window pointer was invalid {}", hwnd);
            }
        },
        _ => {
        }
   } 
    return DefWindowProcA(hwnd, msg, w_param, l_param);
}