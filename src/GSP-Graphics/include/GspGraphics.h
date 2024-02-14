//  GspGraphics.h - System - GSP
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
//  GspGraphics.h is an abstraction layer over
//  the raw platform APIs such as Vulkan, Metal
//  and OpenGL.

#ifndef GSPGRAPHICS_H
#define GSPGRAPHICS_H

#include "GspSystem.h"

enum gGraphicsApi {
    NO_API,
    OPENGL,
    VULKAN,
    METAL
};


// GENERAL

//  GspGraphicsInit - global init for graphics - should only be called once by system
gResult GspGraphicsInit();

//  GspGraphicsGetApi - identify which API is being used
gGraphicsApi GspGraphicsGetApi();


// API SPECIFIC

//  GspGraphicsInitForWindow - setup and allocate resources for rendering a window.
//  Should only be called once per window by the system.
gResult GspGraphicsInitForWindow(gWindow window);

//  GspGraphicsCloseForWindow - deallocate rendering resources for a window
//  Should only be called when a window is deallocated
gResult GspGraphicsCloseForWindow(gWindow window);

//  GspGraphicsStartRender - start rendering frame, called by system only
gResult GspGraphicsStartRender(gWindow window);

//  GspGraphicsCommitRender - stop rendering frame and swap buffers, called by system only
gResult GspGraphicsCommitRender(gWindow window);

//  GspGraphicsFillRect - fill rect on active window
gResult GspGraphicsFillRect(gRect rect, gColor color);


#endif // GSPGRAPHICS_H