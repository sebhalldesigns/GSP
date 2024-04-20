import ctypes
from ctypes import c_void_p
import os
from ctypes import CFUNCTYPE

# Get the directory of the current script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Construct the full path to the C library
library_path = os.path.join(script_dir, "./../../gsp_system/target/debug/libgsp_system.dylib")  # Adjust the filename accordingly

dep2 = ctypes.CDLL("/System/Library/Frameworks/Cocoa.framework/Versions/A/Cocoa")
lib = ctypes.CDLL(library_path)  # Adjust the filename accordingly

# Define the necessary types
gsp_application_launch_callback_t = ctypes.CFUNCTYPE(None)

# Define the functions
gsp_init = lib.gsp_init
gsp_init.argtypes = [gsp_application_launch_callback_t]
gsp_init.restype = ctypes.c_bool

gsp_run = lib.gsp_run
gsp_run.restype = ctypes.c_int

# Define a callback function if needed
def launch_callback():
    print("Launch callback invoked")


CMPFUNC = CFUNCTYPE(None)
# Call gsp_init with the launch_callback function

cmp_func = CMPFUNC(launch_callback)

gsp_init(cmp_func)


gsp_run()