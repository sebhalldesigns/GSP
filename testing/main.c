#include "GSPCore/GSPCore.h"

#include <stdio.h>

void LaunchEvent() {
    printf("Launched!\n");
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