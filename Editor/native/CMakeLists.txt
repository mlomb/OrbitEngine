cmake_minimum_required(VERSION 3.12.4)
project(Editor)

# custom for now
add_subdirectory("../../OrbitEngine" OrbitEngine)

file(GLOB_RECURSE SRC
    "src/*.h"
    "src/*.hpp"
    "src/*.c"
    "src/*.cc"
    "src/*.cpp"
)

add_executable(EditorNative ${SRC})

target_link_libraries(EditorNative OrbitEngine)
target_include_directories(EditorNative PUBLIC "src")
