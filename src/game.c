#define BUILD_DLL
#include "game.h"

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#include <windows.h>
#include <stdbool.h>

#include "raylib.h"

void InitWindowDLL(int width, int height, const char* title) {
    InitWindow(width, height, title);
}

bool WindowShouldCloseDLL(void) {
    return WindowShouldClose();
}

void BeginDrawingDLL(void) {
    BeginDrawing();
}

void EndDrawingDLL(void) {
    EndDrawing();
}

void CloseWindowDLL(void) {
    CloseWindow();
}

void SetTargetFPSDLL(int fps) {
    SetTargetFPS(fps);
}

void UpdateAndDraw() {
    ClearBackground(DARKGRAY);  // Basic drawing to keep window visible
    DrawText("Basic Raylib Window", 10, 10, 20, WHITE);
    //DrawText("Basic Raylib Window", 10, 30, 20, WHITE); //test game.dll rebuild
}