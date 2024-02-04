//  gOpenGL.h - System - GSP
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
//  gOpenGL.h defines OpenGL specific interfaces.
//  It should not be called directly by the user.
//  Instead the GspGraphics.h abstraction should
//  be used as this selects the available graphics
//  mode.

#ifndef GOPENGL_H
#define GOPENGL_H

#include "GspSystem.h"

gResult gOpenGLInitForWindow(gWindow window);

gResult gOpenGLCloseForWindow(gWindow window);

gResult gOpenGLStartRender(gWindow window);

gResult gOpenGLCommitRender(gWindow window);

gResult gOpenGLFillRect(gRect rect, gColor color);

#endif // GOPENGL_H