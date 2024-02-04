//  gVulkan.h - System - GSP
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
//  gVulkan.h defines Vulkan specific interfaces.
//  It should not be called directly by the user.
//  Instead the GspGraphics.h abstraction should
//  be used as this selects the available graphics
//  mode.

#ifndef GVULKAN_H
#define GVULKAN_H

#include "GspSystem.h"

gResult gVulkanInit();

gResult gVulkanInitForWindow(gWindow window);

gResult gVulkanCloseForWindow(gWindow window);

gResult gVulkanStartRender(gWindow window);

gResult gVulkanCommitRender(gWindow window);

gResult gVulkanFillRect(gRect rect, gColor color);

#endif // GVULKAN_H