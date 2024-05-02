#include "GSPCore/GApplication.h"
#include "def/GApplicationDef.h"

#include <stdlib.h>
#include <stddef.h>

GApplication GApplication_Init(GApplicationInfo info) {

    // check if there is already an app
    if (app != NULL) {
        return NULL;
    }

    GApplication app = calloc(1, sizeof(GApplicationDef));
    ((GApplicationDef*)app)->info = info;

    return app;
}

void GApplication_SetLaunchEvent(GApplication app, GApplicationLaunchEvent launchEvent) {
    if (app == NULL) {
        return;
    }

    ((GApplicationDef*)app)->launchEvent = launchEvent;
}