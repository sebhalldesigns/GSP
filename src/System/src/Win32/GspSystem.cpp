//  GspSystem.cpp - System - GSP
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
//  GspSystem.cpp contains a single implementation
//  for the entire System interface. Having all
//  the code in a single file reduces the complexity
//  somewhat and allows the 'System' component to be
//  seen as one.
//
//  This variant of the file is written specifically
//  for the Win32 environment.

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

#include "GspSystem.h"
#include "GspGraphics.h"

#include <stdio.h>
#include <vector>
#include <map>

struct gWindowInternalInfo {
    gWindowVisibility visibilityAttr;
    gWindowStyle styleAttr;
    gWindowFocus focuAttr;
    gWindowResizeCallback resizeCb = nullptr;
    gWindowMoveCallback moveCb = nullptr;
    gWindowVisbilityChangedCallback visbilityChangedCb = nullptr;
    gWindowFocusChangedCallback focusChangedCb = nullptr;
    gWindowDrawCallback drawCb = nullptr;
    gWindowCloseCallback closeCb = nullptr;
    gWindowResizeRequestCallback resizeRequestCb = nullptr;
    gWindowMoveRequestCallback moveRequestCb = nullptr;
    gWindowCloseRequestCallback closeRequestCb = nullptr;
};

const wchar_t CLASS_NAME[]  = L"GSP Window";

static gApplicationInfo appInfo;
static gApplicationLaunchedCallback launchedCb;
static gApplicationQuitCallback quitCb;
static gApplicationQuitRequestCallback quitRequestCb;

static std::vector<gWindow> windows = { };
static std::map<gWindow, gWindowInternalInfo> windowInfoMap = {};

static bool shouldQuitFlag = false;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//  gApplicationRun - main run function
gResult gApplicationRun(gApplicationInfo applicationInfo) {

    printf("STARTING GSP...\n");

    appInfo = applicationInfo;

    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProcedure;
    wc.hInstance     = GetModuleHandle(nullptr);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    if (GspGraphicsInit()) {
        return gResult::FAILURE;
    }

    if (launchedCb != nullptr) {
        std::vector<std::string> launchArgs;
        launchedCb(launchArgs);
    }

    MSG msg = { };

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (shouldQuitFlag) {
            if (quitCb != nullptr) {
                quitCb();
            }
            break;
        }
    }
    
    return gResult::SUCCESS;
}

//  gApplicationRun - main run function with launch args
gResult gApplicationRun(int argc, char* argv[], gApplicationInfo applicationInfo) {

    printf("STARTING GSP...\n");

    appInfo = applicationInfo;

    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProcedure;
    wc.hInstance     = GetModuleHandle(nullptr);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    if (launchedCb != nullptr) {
        std::vector<std::string> launchArgs;
        for (int i = 0; i < argc; i++) {
            launchArgs.push_back(argv[i]);
        }
        launchedCb(launchArgs);
    }

    MSG msg = { };

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (shouldQuitFlag) {
            if (quitCb != nullptr) {
                quitCb();
            }
            break;
        }
    }

    return gResult::SUCCESS;
}

//  gApplicationQuit - quit the application
gResult gApplicationQuit() {
    PostQuitMessage(0);
    return gResult::SUCCESS;
}

//  gApplicationSetLaunchedCallback - set launchedCallback
gResult gApplicationSetLaunchedCallback(gApplicationLaunchedCallback launchedCallback) {
    launchedCb = launchedCallback;
    return gResult::SUCCESS;
}

//  gApplicationSetQuitCallback - set quitCallback
gResult gApplicationSetQuitCallback(gApplicationQuitCallback quitCallback) {
    quitCb = quitCallback;
    return gResult::SUCCESS;
}

//  gApplicationSetQuitRequestCallback - set quitCallback
gResult gApplicationSetQuitRequestCallback(gApplicationQuitRequestCallback quitRequestCallback) {
    quitRequestCb = quitRequestCallback;
    return gResult::SUCCESS;
}

//  gCreateWindow - try to create a window with a title and size
gResult gCreateWindow(gWindow& window, std::string title, gSize size) {

    int titleLength = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, NULL, 0);
    std::wstring wideTitle;
    wideTitle.resize(titleLength);
    MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, &wideTitle[0], titleLength);

    window = CreateWindowEx(
        0,
        CLASS_NAME,
        wideTitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        (int)size.width, (int)size.height, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        GetModuleHandle(nullptr),
        NULL
    );

    if (window == nullptr) {
        return gResult::WARNING;
    } 

    GspGraphicsInitForWindow(window);

    windowInfoMap[window] = {};

    return gResult::SUCCESS;
}

//  gCreateWindow - try to create a window with a title, size and position
gResult gCreateWindow(gWindow& window, std::string title, gSize size, gPoint position) {

    int titleLength = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, NULL, 0);
    std::wstring wideTitle;
    wideTitle.resize(titleLength);
    MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, &wideTitle[0], titleLength);

    window = CreateWindowEx(
        0,
        CLASS_NAME,
        wideTitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        (int)size.width, (int)size.height, (int)position.x, (int)position.y,
        NULL,
        NULL,
        GetModuleHandle(nullptr),
        NULL
    );

    if (window == nullptr) {
        return gResult::WARNING;
    } 

    GspGraphicsInitForWindow(window);

    windowInfoMap[window] = {};

    return gResult::SUCCESS;
}

//  gDrawWindow - request a window redraw
gResult gDrawWindow(gWindow window) {

    if (!InvalidateRect((HWND)window, nullptr, true)) {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}

//  gDestroyWindow - try to destroy a window
gResult gDestroyWindow(gWindow window) {

    if (!DestroyWindow((HWND)window)) {
        return gResult::WARNING;
    }

    return gResult::FAILURE;
}

//  get/set window title
gResult gSetWindowTitle(gWindow window, std::string title) {

    int titleLength = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, NULL, 0);
    std::wstring wideTitle;
    wideTitle.resize(titleLength);
    MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, &wideTitle[0], titleLength);

    if (!SetWindowText((HWND)window, wideTitle.c_str())) {
        return gResult::WARNING;
    }

    return gResult::FAILURE;
}

gResult gGetWindowTitle(gWindow window, std::string& title) { 

    LPWSTR wideWinString;

    if (!GetWindowText((HWND)window, wideWinString, INT_MAX)) {
        return gResult::WARNING;
    }

    std::wstring wideString(wideWinString); 
    title = std::string(wideString.begin(), wideString.end());

    return gResult::SUCCESS;
}

//  get/set window position
gResult gSetWindowPosition(gWindow window, gPoint position) {

    RECT windowRect;

    if (!GetWindowRect((HWND)window, &windowRect)) {
        return gResult::WARNING;
    }

    if (!MoveWindow((HWND)window, (int)position.x, (int)position.y, 
    (windowRect.bottom - windowRect.top), (windowRect.right - windowRect.left), true)) {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}

gResult gGetWindowPosition(gWindow window, gPoint& position) {

    RECT windowRect;

    if (!GetWindowRect((HWND)window, &windowRect)) {
        return gResult::WARNING;
    }

    position = {(float)windowRect.left, (float)windowRect.top};

    return gResult::SUCCESS;
}

//  get/set window size
gResult gSetWindowSize(gWindow window, gSize size) {

    RECT windowRect;

    if (!GetWindowRect((HWND)window, &windowRect)) {
        return gResult::WARNING;
    }

    if (!MoveWindow((HWND)window, windowRect.left, windowRect.top, 
    (int)size.width, (int)size.height, true)) {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}

gResult gGetWindowSize(gWindow window, gSize& size) {

    RECT windowRect;

    if (!GetWindowRect((HWND)window, &windowRect)) {
        return gResult::WARNING;
    }

    size = {(float)(windowRect.right - windowRect.left), (float)(windowRect.bottom - windowRect.top)};

    return gResult::SUCCESS;
}

//  get/set window visbility
gResult gSetWindowVisibility(gWindow window, gWindowVisibility visibility) {

    int win32Command;

    switch (visibility) {
        case HIDDEN:
            win32Command = SW_HIDE;
            break;
        case MINIMISED:
            win32Command = SW_SHOWMINIMIZED;
            break;
        case VISIBLE:
            win32Command = SW_SHOW;
            break;
        case FULLSCREEN:
            win32Command = SW_SHOW; // NOTE MORE CODE IS NEEDED HERE LIKE HIDING DECORATIONS ETC
            break;
    }

    if (!ShowWindow((HWND)window, win32Command)) {
        return gResult::WARNING;
    }

    // check windo is in info map
    if (windowInfoMap.find(window) != windowInfoMap.end()) {
        windowInfoMap[window].visibilityAttr = visibility;
    }

    return gResult::SUCCESS;
}

gResult gGetWindowVisibility(gWindow window, gWindowVisibility& visibility) {

    // check windo is in info map
    if (windowInfoMap.find(window) != windowInfoMap.end()) {
        visibility = windowInfoMap[window].visibilityAttr;
    } else {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}

//  get/set window style
gResult gSetWindowStyle(gWindow window, gWindowStyle style) {

    // todo: change style here

    // check windo is in info map
    if (windowInfoMap.find(window) != windowInfoMap.end()) {
        windowInfoMap[window].styleAttr = style;
    }


    return gResult::SUCCESS;
}

gResult gGetWindowStyle(gWindow window, gWindowStyle& style) {

    // check windo is in info map
    if (windowInfoMap.find(window) != windowInfoMap.end()) {
        style = windowInfoMap[window].styleAttr;
    } else {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}



//  gSetWindowResizeCallback - set resizeCallback
gResult gSetWindowResizeCallback(gWindow window, gWindowResizeCallback resizeCallback) {

    // check windo is in info map
    if (windowInfoMap.find(window) != windowInfoMap.end()) {
        windowInfoMap[window].resizeCb = resizeCallback;
    } else {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}

//  gSetWindowMoveCallback - set moveCallback
gResult gSetWindowMoveCallback(gWindow window, gWindowMoveCallback moveCallback) {

    // check windo is in info map
    if (windowInfoMap.find(window) != windowInfoMap.end()) {
        windowInfoMap[window].moveCb = moveCallback;
    } else {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}

//  gSetWindowVisbilityChangedCallback - set visibilityChangedCallback
gResult gSetWindowVisbilityChangedCallback(gWindow window, gWindowVisbilityChangedCallback visibilityChangedCallback) {

    // check windo is in info map
    if (windowInfoMap.find(window) != windowInfoMap.end()) {
        windowInfoMap[window].visbilityChangedCb = visibilityChangedCallback;
    } else {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}

//  gSetWindowFocusChangedCallback - set focusChangedCallback
gResult gSetWindowFocusChangedCallback(gWindow window, gWindowFocusChangedCallback focusChangedCallback) {

    // check windo is in info map
    if (windowInfoMap.find(window) != windowInfoMap.end()) {
        windowInfoMap[window].focusChangedCb = focusChangedCallback;
    } else {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}

//  gSetWindowCloseCallback - set closeCallback
gResult gSetWindowDrawCallback(gWindow window, gWindowDrawCallback drawCallback) {

    // check windo is in info map
    if (windowInfoMap.find(window) != windowInfoMap.end()) {
        windowInfoMap[window].drawCb = drawCallback;
    } else {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}

//  gSetWindowCloseCallback - set closeCallback
gResult gSetWindowCloseCallback(gWindow window, gWindowCloseCallback closeCallback) {

    // check windo is in info map
    if (windowInfoMap.find(window) != windowInfoMap.end()) {
        windowInfoMap[window].closeCb = closeCallback;
    } else {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}

//  gSetWindowResizeRequestCallback - set resizeRequestCallback
gResult gSetWindowResizeRequestCallback(gWindow window, gWindowResizeRequestCallback resizeRequestCallback) {

    // check windo is in info map
    if (windowInfoMap.find(window) != windowInfoMap.end()) {
        windowInfoMap[window].resizeRequestCb = resizeRequestCallback;
    } else {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}

//  gSetWindowMoveRequestCallback - set moveRequestCallback
gResult gSetWindowMoveRequestCallback(gWindow window, gWindowMoveRequestCallback moveRequestCallback) {

    // check windo is in info map
    if (windowInfoMap.find(window) != windowInfoMap.end()) {
        windowInfoMap[window].moveRequestCb = moveRequestCallback;
    } else {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}

//  gSetWindowCloseRequestCallback - set closeRequestCallback
gResult gSetWindowCloseRequestCallback(gWindow window, gWindowCloseRequestCallback closeRequestCallback) {

    // check windo is in info map
    if (windowInfoMap.find(window) != windowInfoMap.end()) {
        windowInfoMap[window].closeRequestCb = closeRequestCallback;
    } else {
        return gResult::WARNING;
    }

    return gResult::SUCCESS;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    if (windowInfoMap.find((gWindow)hwnd) == windowInfoMap.end()) {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    gWindow window = (gWindow)hwnd; 

    switch (msg) {

        case WM_DESTROY: {
            if (windowInfoMap[window].closeCb != nullptr) {
                windowInfoMap[window].closeCb(window);
            }

            GspGraphicsCloseForWindow(window);

            windowInfoMap.erase(window);

            for (int i = 0; i++; i < windows.size()) {
                if (windows[i] == window) {
                    windows.erase(windows.begin() + i);
                    i--;
                }
            }

            if (appInfo.quitWhenNoWindows) {
                shouldQuitFlag = true;
            }

        }

        case WM_SIZE: {

            gSize newSize = {(float)LOWORD(lParam), (float)HIWORD(lParam)};

            if (windowInfoMap[window].resizeRequestCb != nullptr) {
                newSize = windowInfoMap[window].resizeRequestCb(window, newSize);
                gSetWindowSize(window, newSize);
            }

            if (windowInfoMap[window].resizeCb != nullptr) {
                windowInfoMap[window].resizeCb(window, newSize);
                gDrawWindow(window);
                return 0;
            } else {
                gDrawWindow(window);
                return DefWindowProc(hwnd, msg, wParam, lParam);
            }
        }

        case WM_MOVE: {

            gPoint newPos = {(float)(short)LOWORD(lParam), (float)(short)HIWORD(lParam)};

            if (windowInfoMap[window].moveRequestCb != nullptr) {
                newPos = windowInfoMap[window].moveRequestCb(window, newPos);
                gSetWindowPosition(window, newPos); 
            }

            if (windowInfoMap[window].moveCb != nullptr) {
                windowInfoMap[window].moveCb(window, newPos);
                gDrawWindow(window);
                return 0;
            } else {
                gDrawWindow(window);
                return DefWindowProc(hwnd, msg, wParam, lParam);
            }

            
        }

        case WM_PAINT: {

            GspGraphicsStartRender(window);

            if (windowInfoMap[window].drawCb != nullptr) {

                windowInfoMap[window].drawCb(window);
                /* PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
                //SwapBuffers(hdc);
                EndPaint(hwnd, &ps);
                */
                GspGraphicsCommitRender(window);
                return 0;
            } else {
                GspGraphicsCommitRender(window);
                return DefWindowProc(hwnd, msg, wParam, lParam);
            }
        }

        case WM_DISPLAYCHANGE: {
            gDrawWindow(window);
            return 0;
        }

        case WM_QUIT: {
            shouldQuitFlag = true;
            return 0;
        }

        default: {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }

}