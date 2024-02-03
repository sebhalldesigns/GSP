//  gWindow.h - System - GSP
//  First created by Seb Hall on 03/Feb/2024
//
//  GSP is a cross-platform software framework
//  that provides abstraction layers, libraries,
//  packaging and build functionality. GSP is
//  designed around a modular architecture that
//  promotes simplicity and encapsulation, and
//  we hope that you will do amazing things with
//  it!
//
//  gWindow.h contains the core window api, implemented
//  in a functional, non object-oriented way.

#ifndef GWINDOW_H
#define GWINDOW_H

#include <string>

#include "gResult.h"
#include "gSize.h"
#include "gPoint.h"


//  gWindow - a window handle
typedef void* gWindow;

//  gWindowVisibility - window visbility enumeration
//  HIDDEN - completely invisible to user
//  MINIMISED - not shown on screen but existence known to user
//  VISIBLE - shown on screen
//  FULLSCREEN - occupies entire screen, no borders
typedef enum gWindowVisibility {
    HIDDEN,
    MINIMISED,
    VISIBLE,
    FULLSCREEN
};

//  gWindowStyle - window decoration style
//  NONE - window shown without borders
//  SYSTEM - window shown with default system title bar and decorations
//  CUSTOM - window borders to be drawn by user
typedef enum gWindowStyle {
    NONE,
    SYSTEM,
    CUSTOM
};

//  gWindowFocus - window focus status
//  BACKGROUND - window not primary
//  FOREGROUND - window primary and focused
typedef enum gWindowFocus {
    BACKGROUND,
    FOREGROUND
};


//  gWindowResizeCallback - called when window was resized by user
typedef void (*gWindowResizeCallback)(gWindow window, gSize newSize);

//  gWindowMoveCallback - called when window was moved by user
typedef void (*gWindowMoveCallback)(gWindow window, gPoint newPosition);

//  gWindowVisbilityChangedCallback - called when window changed visibility
typedef void (*gWindowVisbilityChangedCallback)(gWindow window, gWindowVisibility newVisibility);

//  gWindowFocusChangedCallback - called when window focus changed
typedef void (*gWindowFocusChangedCallback)(gWindow window, gWindowFocus newFocus);

//  gWindowDrawCallback - called when window needs a redraw
typedef void (*gWindowDrawCallback)(gWindow window);

//  gWindowCloseCallback - called when window closed
typedef void (*gWindowCloseCallback)(gWindow window);

//  gWindowResizeRequestCallback - called when the user has tried to resize the window
typedef gSize (*gWindowResizeRequestCallback)(gWindow window, gSize requestedSize);

//  gWindowMoveRequestCallback - called when the user has tried to move the window
typedef gPoint (*gWindowMoveRequestCallback)(gWindow window, gPoint requestedPosition);

//  gWindowCloseCallback - called when window close button has been pressed
typedef bool (*gWindowCloseRequestCallback)(gWindow window);



//  gCreateWindow - try to create a window with a title and size
gResult gCreateWindow(gWindow& window, std::string title, gSize size);

//  gCreateWindow - try to create a window with a title, size and position
gResult gCreateWindow(gWindow& window, std::string title, gSize size, gPoint position);

//  gDrawWindow - request a window redraw
gResult gDrawWindow(gWindow window);

//  gDestroyWindow - try to destroy a window
gResult gDestroyWindow(gWindow window);

//  get/set window title
gResult gSetWindowTitle(gWindow window, std::string title);
gResult gGetWindowTitle(gWindow window, std::string& title);

//  get/set window position
gResult gSetWindowPosition(gWindow window, gPoint position);
gResult gGetWindowPosition(gWindow window, gPoint& position);

//  get/set window size
gResult gSetWindowSize(gWindow window, gSize size);
gResult gGetWindowSize(gWindow window, gSize& size);

//  get/set window visbility
gResult gSetWindowVisibility(gWindow window, gWindowVisibility visibility);
gResult gGetWindowVisibility(gWindow window, gWindowVisibility& visibility);

//  get/set window style
gResult gSetWindowStyle(gWindow window, gWindowStyle style);
gResult gGetWindowStyle(gWindow window, gWindowStyle& style);



//  gSetWindowResizeCallback - set resizeCallback
gResult gSetWindowResizeCallback(gWindow window, gWindowResizeCallback resizeCallback);

//  gSetWindowMoveCallback - set moveCallback
gResult gSetWindowMoveCallback(gWindow window, gWindowMoveCallback moveCallback);

//  gSetWindowVisbilityChangedCallback - set visibilityChangedCallback
gResult gSetWindowVisbilityChangedCallback(gWindow window, gWindowVisbilityChangedCallback visibilityChangedCallback);

//  gSetWindowFocusChangedCallback - set focusChangedCallback
gResult gSetWindowFocusChangedCallback(gWindow window, gWindowFocusChangedCallback focusChangedCallback);

//  gSetWindowDrawCallback - set drawCallback
gResult gSetWindowDrawCallback(gWindow window, gWindowDrawCallback drawCallback);

//  gSetWindowCloseCallback - set closeCallback
gResult gSetWindowCloseCallback(gWindow window, gWindowCloseCallback closeCallback);

//  gSetWindowResizeRequestCallback - set resizeRequestCallback
gResult gSetWindowResizeRequestCallback(gWindow window, gWindowResizeRequestCallback resizeRequestCallback);

//  gSetWindowMoveRequestCallback - set moveRequestCallback
gResult gSetWindowMoveRequestCallback(gWindow window, gWindowMoveRequestCallback moveRequestCallback);

//  gSetWindowCloseRequestCallback - set closeRequestCallback
gResult gSetWindowCloseRequestCallback(gWindow window, gWindowCloseRequestCallback closeRequestCallback);


#endif // GWINDOW_H