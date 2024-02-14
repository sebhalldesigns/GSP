//  gApplication.h - System - GSP
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
//  gApplication.h contains the gApplication type

#ifndef GAPPLICATION_H
#define GAPPLICATION_H

#include "gResult.h"

#include <vector>
#include <string>

struct gApplicationInfo {
    std::string title;
    std::string developer;
    std::string version;
    std::string category;
    std::string helpWebsite;

    bool quitWhenNoWindows;
};

// gApplicationLaunchedCallback - called when app has finished launching
typedef void (*gApplicationLaunchedCallback)(std::vector<std::string> args);

// gApplicationQuitCallback - called when an app is shutting down
typedef void (*gApplicationQuitCallback)();

// gApplicationQuitRequestCallback - called when an app quit is requested
typedef bool (*gApplicationQuitRequestCallback)();

//  gApplicationRun - main run function
gResult gApplicationRun(gApplicationInfo applicationInfo);

//  gApplicationRun - main run function with launch args
gResult gApplicationRun(int argc, char* argv[], gApplicationInfo applicationInfo);

//  gApplicationQuit - quit the application
gResult gApplicationQuit();

//  gApplicationSetLaunchedCallback - set launchedCallback
gResult gApplicationSetLaunchedCallback(gApplicationLaunchedCallback launchedCallback);

//  gApplicationSetQuitCallback - set quitCallback
gResult gApplicationSetQuitCallback(gApplicationQuitCallback quitCallback);

//  gApplicationSetQuitRequestCallback - set quitCallback
gResult gApplicationSetQuitRequestCallback(gApplicationQuitRequestCallback quitRequestCallback);

#endif // GAPPLICATION_H