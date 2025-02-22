cmake_minimum_required(VERSION 3.10)
project(RaylibExample C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/Debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/Debug")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Default to static libs" FORCE)
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

add_executable(${PROJECT_NAME} src/main.c)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

add_library(game SHARED src/game.c)
target_include_directories(game PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)
set_target_properties(game PROPERTIES 
    OUTPUT_NAME "game"
)

target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries(game raylib)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} winmm.lib gdi32.lib)
    target_link_libraries(game winmm.lib gdi32.lib)
endif()

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()