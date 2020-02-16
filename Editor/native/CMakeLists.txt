cmake_minimum_required(VERSION 3.12.4)
project(EditorService)

add_subdirectory("../.." OrbitEngine)

file(GLOB_RECURSE SRC
    "src/*.h"
    "src/*.hpp"
    "src/*.c"
    "src/*.cc"
    "src/*.cpp"
)

add_executable(EditorService ${SRC})

target_link_libraries(EditorService OrbitEngine)
target_include_directories(EditorService PUBLIC "src")

add_custom_command(
	TARGET EditorService
	POST_BUILD 
	COMMAND
		"${CMAKE_COMMAND}" -E copy 
		"$<TARGET_FILE:EditorService>"
		"../../public/native/$<TARGET_FILE_NAME:EditorService>" 
	COMMENT "Copying to native directory"
)
