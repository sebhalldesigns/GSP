#include "GSPCore/GSPCore.h"
#include "GSPCore/GWindow.h"
#include "GSPCore/GLog.h"

#include <stdio.h>


typedef struct {
    float a;
    int b;
    const char* c;
} exampleStruct;

void LaunchEvent() {
    printf("Launched!\n");
    GLog(INFO, "info");
    GLog(WARNING, "warning");
    GLog(ERROR, "warning");
    GLog(FAILURE, "warning");

    GWindowInfo info = {
        800,
        600,
        "My Window"
    };

    GWindow window = GWindow_Init(info);
}

int main() {

    GApplicationInfo info = {
        "example app", 
        "Seb Hall", 
        0, 0
    };

    GApplication app = GApplication_Init(info);
    
    GApplication_SetLaunchEvent(app, LaunchEvent);

    return GSPRun(app);
}