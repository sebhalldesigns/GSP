#include "GSPCore/GSPCore.h"
#include "def/GApplicationDef.h"




int GSPRun(GApplication app) {
    if (app == 0 || ((GApplicationDef*)app)->launchEvent == 0) {
        return -1;
    }

    (((GApplicationDef*)app)->launchEvent)();

    return 0;
}