#include "GSPCore/GWindow.h"
#include "internal/def/GWindowDef.h"
#include "internal/include/GVector.h"

#include "GSPCore/GLog.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>

// common statics
static GVector windowVector = NULL;

static Display* xDisplay = NULL;
static Window xRoot = None;
static XEvent xEvent;
static KeySym eventKey;
static char eventText[255];

static Atom deleteMessage;

XVisualInfo* glxVisualInfo = NULL;
GLXFBConfig bestGlxFbc;

static int visual_attribs[] = {
    GLX_X_RENDERABLE    , True,
    GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
    GLX_RENDER_TYPE     , GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
    GLX_RED_SIZE        , 8,
    GLX_GREEN_SIZE      , 8,
    GLX_BLUE_SIZE       , 8,
    GLX_ALPHA_SIZE      , 8,
    GLX_DEPTH_SIZE      , 24,
    GLX_STENCIL_SIZE    , 8,
    GLX_DOUBLEBUFFER    , True,
    None
};

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

void TryMakeGlVisualInfo();
void TryMakeGlxWindow(Window* xWindow, GLXContext* context, GWindowInfo info);
bool isExtensionSupported(const char *extList, const char *extension);

bool ctxErrorOccurred = false;
int ctxErrorHandler( Display *dpy, XErrorEvent *ev) {
    ctxErrorOccurred = true;
    return 0;
}



// functions defined in this file
GWindowDef* TryGetWindow(Window xWindow);

GWindow GWindow_Init(GWindowInfo info) {

    if (xDisplay == NULL) {
        // either first time opening a window, or x11 broken.
        xDisplay = XOpenDisplay(NULL);
        xRoot = DefaultRootWindow(xDisplay);
    }

    if (xDisplay == NULL) {
        DEBUG_LOG(ERROR, "Failed to connect to an X11 display server!");
        return NULL;
    }

    if (xRoot == 0) {
        XCloseDisplay(xDisplay);
        DEBUG_LOG(ERROR, "Failed to connect to an X11 root window!");
        return NULL;
    }

    deleteMessage = XInternAtom(xDisplay, "WM_DELETE_WINDOW", False);

    if (glxVisualInfo == NULL) {
        TryMakeGlVisualInfo();
    }

    GLXContext context = NULL;
    Window xWindow = None;

    if (glxVisualInfo != NULL) {
        TryMakeGlxWindow(&xWindow, &context, info);
    }

    if (xWindow == None) {
        xWindow = XCreateSimpleWindow(xDisplay, xRoot, 0, 0, info.width, info.height, 0, 0, 0xffffffff);
    }

    if (xWindow == None) {
        DEBUG_LOG(ERROR, "Failed to create an X11 window");
        return 0;
    }

    XStoreName(xDisplay, xWindow, info.title);

    XSetWMProtocols(xDisplay, xWindow, &deleteMessage, 1);
    XSelectInput(xDisplay, xWindow, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
    XMapWindow(xDisplay, xWindow);

    GWindowDef* window = malloc(sizeof(GWindowDef));

    if (window == NULL) {
        return NULL;
    }

    window->title = info.title;
    window->rawHandle = (void*)xWindow;
    window->width = info.width;
    window->height = info.height;
    window->glContext = context;

    if (windowVector == NULL) {
        windowVector = GVector_Init();
    }

    if (windowVector != NULL) {
        GVector_Add(windowVector, (GVectorItem)window);
        DEBUG_LOG(INFO, "Allocated GWindow at %lu", window);
        return window;
    } else {
        DEBUG_LOG(ERROR, "Failed to allocate a window vector.");
        return NULL;
    }

}

void GWindow_SetResizeDelegate(GWindow window, GWindowResizeDelegate resizeDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->resizeDelegate = resizeDelegate;
    }
}

void GWindow_SetPointerMoveDelegate(GWindow window, GWindowPointerMoveDelegate pointerMoveDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->pointerMoveDelegate = pointerMoveDelegate;
    }
}

void GWindow_SetButtonDownDelegate(GWindow window, GWindowButtonDownDelegate buttonDownDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->buttonDownDelegate = buttonDownDelegate;
    }
}

void GWindow_SetButtonUpDelegate(GWindow window, GWindowButtonUpDelegate buttonUpDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->buttonUpDelegate = buttonUpDelegate;
    }
}



void GWindowDef_Poll() {

    XNextEvent(xDisplay, &xEvent);

    GWindowDef* windowDef = TryGetWindow(xEvent.xany.window);

    if (windowDef == NULL) {
        DEBUG_LOG(ERROR, "Event recieved for unknown window!");
        return;
    }

    switch(xEvent.type) {
        case Expose:

            glXMakeCurrent(xDisplay, xEvent.xany.window,  windowDef->glContext);
            glClearColor(0, 0.5, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            glXSwapBuffers(xDisplay, xEvent.xany.window);

            break;
        case ConfigureNotify:
            
            int newWidth = xEvent.xconfigure.width;
            int newHeight = xEvent.xconfigure.height;

            if (windowDef->width != newWidth || windowDef->height != newHeight) {

                GWindowSize newSize = { newWidth, newHeight };

                windowDef->width = newWidth;
                windowDef->height = newHeight;
                
                if (windowDef->resizeDelegate != NULL) {
                    (windowDef->resizeDelegate)(windowDef, newSize);
                }
            }
            
            break;
        case MotionNotify:

            GWindowPoint motionLocation = {xEvent.xmotion.x, xEvent.xmotion.y};
            if (windowDef->pointerMoveDelegate != NULL) {
                (windowDef->pointerMoveDelegate)(windowDef, motionLocation);
            }

            break;
        case ButtonPress:
        
            GWindowPoint buttonDownLocation = {xEvent.xbutton.x, xEvent.xbutton.y};
            if (windowDef->buttonDownDelegate != NULL) {
                (windowDef->buttonDownDelegate)(windowDef, buttonDownLocation, (uint8_t)xEvent.xbutton.button);
            }

            break;
        case ButtonRelease:

            GWindowPoint buttonUpLocation = {xEvent.xbutton.x, xEvent.xbutton.y};
            if (windowDef->buttonUpDelegate != NULL) {
                (windowDef->buttonUpDelegate)(windowDef, buttonUpLocation, (uint8_t)xEvent.xbutton.button);
            }

            break;
        case KeyPress:
            // Handle keyboard events
            XLookupString(&xEvent.xkey, eventText, sizeof(eventText), &eventKey, 0);
            printf("Key pressed: %s\n", eventText);
            break;

        case ClientMessage:
            /*if (g_event.xclient.data.l[0] == g_delete_message) {
                if (g_window_close_request_callback != NULL) {

                    if (g_window_close_request_callback((uintptr_t) g_event.xany.window)) {

                        gwin_destroy_window((uintptr_t) g_event.xany.window);
                    }
                } else {
                    gwin_destroy_window((uintptr_t) g_event.xany.window);
                }
            }*/
            break;
    }
}

GWindowDef* TryGetWindow(Window xWindow) {

    for (int i = 0; i < GVector_Size(windowVector); i++) {
        
        GVectorItem item = GVector_Get(windowVector, i);
        if (item == NULL) {
            return NULL;
        }

        GWindowDef* window = (GWindowDef*)item;
        if (window->rawHandle == (GVectorItem)xWindow) {
            return (GWindowDef*)GVector_Get(windowVector, i);
        }
    }
        
    return NULL;
}

bool isExtensionSupported(const char *extList, const char *extension) {
  const char *start;
  const char *where, *terminator;
  
  /* Extension names should not have spaces. */
  where = strchr(extension, ' ');
  if (where || *extension == '\0')
    return false;

  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  for (start=extList;;) {
    where = strstr(start, extension);

    if (!where)
      break;

    terminator = where + strlen(extension);

    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;

    start = terminator;
  }

  return false;
}

void TryMakeGlVisualInfo() {
    int glx_major, glx_minor;

    // FBConfigs were added in GLX version 1.3.
    if (!glXQueryVersion(xDisplay, &glx_major, &glx_minor) || 
    ((glx_major == 1) && (glx_minor < 3)) || (glx_major < 1)) {
        DEBUG_LOG(ERROR, "Invalid GLX Version!");
        return;
    }

    int fbcount;
    GLXFBConfig* fbc = glXChooseFBConfig(xDisplay, DefaultScreen(xDisplay), visual_attribs, &fbcount);
    if (!fbc) {
        DEBUG_LOG(ERROR, "Failed to retrieve a GLX framebuffer config!");
        return;
    }

    int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

    int i;
    for (i=0; i<fbcount; ++i) {

        XVisualInfo* vi = glXGetVisualFromFBConfig( xDisplay, fbc[i] );

        if (vi) {
            int samp_buf, samples;
            glXGetFBConfigAttrib( xDisplay, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
            glXGetFBConfigAttrib( xDisplay, fbc[i], GLX_SAMPLES       , &samples  );

            if (best_fbc < 0 || samp_buf && samples > best_num_samp) {
                best_fbc = i, best_num_samp = samples;
            }
                
            if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp ) {
                worst_fbc = i, worst_num_samp = samples;
            }
        }

        // free visual info from glXGetVisualFromFBConfig()
        XFree(vi);
    }

    bestGlxFbc = fbc[ best_fbc ];

    // free FBConfig list allocated by glXChooseFBConfig()
    XFree(fbc);

    // Get a visual
    glxVisualInfo = glXGetVisualFromFBConfig(xDisplay, bestGlxFbc);
}

void TryMakeGlxWindow(Window* xWindow, GLXContext* context, GWindowInfo info) {
    XSetWindowAttributes swa;
    Colormap cmap;
    swa.colormap = cmap = XCreateColormap(xDisplay, xRoot, glxVisualInfo->visual, AllocNone);
    swa.background_pixmap = None ;
    swa.border_pixel      = 0;
    swa.event_mask        = StructureNotifyMask;

    *xWindow = XCreateWindow(xDisplay, xRoot, 0, 0, info.width, info.height, 0, 
    glxVisualInfo->depth, InputOutput, glxVisualInfo->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);

    XMapWindow(xDisplay, *xWindow);

    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString(xDisplay, DefaultScreen(xDisplay));

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

    
    ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);


    if (!isExtensionSupported( glxExts, "GLX_ARB_create_context") || !glXCreateContextAttribsARB) {
        DEBUG_LOG(ERROR, "GLX context creation error");
        return;
    } else {
        int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            None
        };

        *context = glXCreateContextAttribsARB(xDisplay, bestGlxFbc, 0, True, context_attribs);

        XSync( xDisplay, False );
        if ( !ctxErrorOccurred && *context ) {
            DEBUG_LOG(INFO, "Created GL 3.3 context");
        } else {
            return;
        }
    }

    // Sync to ensure any errors generated are processed.
    XSync( xDisplay, False );

    // Restore the original error handler
    XSetErrorHandler( oldHandler );
}