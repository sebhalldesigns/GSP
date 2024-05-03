#include "GSPCore/GSPCore.h"
#include "internal/def/GApplicationDef.h"

#include "GSPCore/GLog.h"
#include "internal/def/GWindowDef.h"

int GSPRun(GApplication app) {

    GLog(INFO, "Starting GSPCore...");


    if (app == 0 || ((GApplicationDef*)app)->launchEvent == 0) {
        GLog(FAILURE, "No app created, or app missing launch callback. Exiting application.");
        return -1;
    }

    (((GApplicationDef*)app)->launchEvent)();

    while (1) {
        GWindowDef_Poll();
    }

    return 0;
}