#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#ifdef _WIN32
    #ifdef BUILD_DLL
        #define DLL_API __declspec(dllexport)
    #else
        #define DLL_API __declspec(dllimport)
    #endif
#else
    #define DLL_API
#endif

DLL_API void UpdateAndDraw(void);
DLL_API void InitWindowDLL(int width, int height, const char* title);
DLL_API bool WindowShouldCloseDLL(void);
DLL_API void BeginDrawingDLL(void);
DLL_API void EndDrawingDLL(void);
DLL_API void CloseWindowDLL(void);
DLL_API void SetTargetFPSDLL(int fps);

#endif