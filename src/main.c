#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

typedef void (*InitWindowFunc)(int, int, const char*);
typedef bool (*WindowShouldCloseFunc)(void);
typedef void (*BeginDrawingFunc)(void);
typedef void (*EndDrawingFunc)(void);
typedef void (*CloseWindowFunc)(void);
typedef void (*SetTargetFPSFunc)(int);
typedef void (*UpdateAndDrawFunc)(void);

static InitWindowFunc InitWindowPtr = NULL;
static WindowShouldCloseFunc WindowShouldClosePtr = NULL;
static BeginDrawingFunc BeginDrawingPtr = NULL;
static EndDrawingFunc EndDrawingPtr = NULL;
static CloseWindowFunc CloseWindowPtr = NULL;
static SetTargetFPSFunc SetTargetFPSPtr = NULL;
static UpdateAndDrawFunc UpdateAndDrawPtr = NULL;

static HMODULE dllHandle = NULL;

bool LoadDLL(const char* dllPath) {
    if (dllHandle) {
        FreeLibrary(dllHandle);
    }

    char tempPath[MAX_PATH];
    snprintf(tempPath, MAX_PATH, "game_initial_%lu.dll", GetTickCount());
    CopyFile(dllPath, tempPath, FALSE);
    dllHandle = LoadLibrary(tempPath);
    if (!dllHandle) {
        printf("Failed to load DLL: %lu\n", GetLastError());
        return false;
    }

    InitWindowPtr = (InitWindowFunc)GetProcAddress(dllHandle, "InitWindowDLL");
    WindowShouldClosePtr = (WindowShouldCloseFunc)GetProcAddress(dllHandle, "WindowShouldCloseDLL");
    BeginDrawingPtr = (BeginDrawingFunc)GetProcAddress(dllHandle, "BeginDrawingDLL");
    EndDrawingPtr = (EndDrawingFunc)GetProcAddress(dllHandle, "EndDrawingDLL");
    CloseWindowPtr = (CloseWindowFunc)GetProcAddress(dllHandle, "CloseWindowDLL");
    SetTargetFPSPtr = (SetTargetFPSFunc)GetProcAddress(dllHandle, "SetTargetFPSDLL");
    UpdateAndDrawPtr = (UpdateAndDrawFunc)GetProcAddress(dllHandle, "UpdateAndDraw");

    if (!InitWindowPtr || !WindowShouldClosePtr || !BeginDrawingPtr || 
        !EndDrawingPtr || !CloseWindowPtr || !SetTargetFPSPtr || !UpdateAndDrawPtr) {
        printf("Failed to load DLL functions: %lu\n", GetLastError());
        FreeLibrary(dllHandle);
        dllHandle = NULL;
        return false;
    }

    printf("DLL loaded successfully!\n");
    return true;
}

int main(void) {
    const char* dllPath = "game.dll";
    if (!LoadDLL(dllPath)) {
        printf("Initial DLL load failed\n");
        return 1;
    }

    InitWindowPtr(800, 450, "Raylib Hot Reload Test");
    SetTargetFPSPtr(60);

    printf("Window initialized, entering main loop...\n");
    printf("Press R to reload game.dll, or close the window to exit.\n");
    int frameCount = 0;
    while (!WindowShouldClosePtr()) {
        frameCount++;
        printf("Frame %d: Window still open\n", frameCount);

        // Manual reload on 'R' key press
        if (GetKeyState('R') & 0x8000) {  // High bit set means key is pressed
            printf("Reload requested...\n");
            CloseWindowPtr();  // Close current window
            if (!LoadDLL(dllPath)) {
                printf("Reload failed, exiting\n");
                break;
            }
            InitWindowPtr(800, 450, "Raylib Hot Reload Test");
            SetTargetFPSPtr(60);
            printf("DLL reloaded and window re-initialized\n");
            Sleep(200);  // Brief delay to stabilize
        }

        BeginDrawingPtr();
        UpdateAndDrawPtr();
        EndDrawingPtr();
    }

    printf("Window closing...\n");
    if (dllHandle) {
        CloseWindowPtr();
        FreeLibrary(dllHandle);
    }
    printf("Program exiting\n");
    return 0;
}