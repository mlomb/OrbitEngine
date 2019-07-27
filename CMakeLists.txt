cmake_minimum_required(VERSION 3.2)

project(OrbitEngine C CXX)

# Options
option(OE_EDITOR_INTERNAL OFF)

include(CMake/Config.cmake)
include(CMake/Tools.cmake)

# Workaround with clang
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND NOT("${CMAKE_SYSTEM_NAME}" STREQUAL "Android"))
	set(CMAKE_CXX_FLAGS "-stdlib=libc++")
endif()
# This fixes some Emscripten cross-compilation problem with zlibstatic of Assimp
include(CheckTypeSize)
check_type_size(off64_t OFF64_T)

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Code/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Code/src)
set(BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(BIN_INC_DIR ${BIN_DIR}/include)

# Configure macros file
configure_file(${INC_DIR}/OE/Config.hpp.in
			   ${BIN_INC_DIR}/OE/Config.hpp)

# - Dependencies -
add_subdirectory(Dependencies/FreeType)
add_subdirectory(Dependencies/FreeImage)
add_subdirectory(Dependencies/glsl-optimizer)

# msdfgen
file(GLOB_RECURSE msdfgen_HEADERS "Dependencies/msdfgen/core/*.h" "Dependencies/msdfgen/core/*.hpp")
file(GLOB_RECURSE msdfgen_SOURCES "Dependencies/msdfgen/core/*.cpp")
add_library(lib_msdfgen ${msdfgen_SOURCES} ${msdfgen_HEADERS})
target_include_directories(lib_msdfgen PUBLIC ${freetype_SOURCE_DIR}/include "Dependencies/msdfgen")
target_link_libraries(lib_msdfgen freetype)

option(XSC_BUILD_SHELL "" OFF) # Don't build XSC shell
add_subdirectory(Dependencies/XShaderCompiler)

option(ASSIMP_BUILD_ASSIMP_TOOLS "" OFF) # Don't build Assimp tools
option(BUILD_SHARED_LIBS "" OFF) # Build Assimp as a static lib
option(ASSIMP_BUILD_TESTS "" OFF) # Don't build Assimp tests
option(ASSIMP_NO_EXPORT "" ON) # Disable Assimp export feature
option(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT "" OFF) # Disable all import formats by default
option(ASSIMP_BUILD_OBJ_IMPORTER "" ON)
option(ASSIMP_BUILD_FBX_IMPORTER "" ON)
add_subdirectory(Dependencies/Assimp)

# Reset some flags that some libraries modify
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "" FORCE)

# - Code -
# General code
file(GLOB_RECURSE SRC ${INC_DIR}/OE/Misc/*.hpp
					  ${INC_DIR}/OE/Misc/*.inl
                      ${SRC_DIR}/OE/Misc/*.cpp
                      ${SRC_DIR}/OE/Misc/*.hpp
                      ${INC_DIR}/OE/Math/*.hpp
                      ${SRC_DIR}/OE/Math/*.cpp
                      ${INC_DIR}/OE/Graphics/*.hpp
                      ${SRC_DIR}/OE/Graphics/*.hpp
                      ${SRC_DIR}/OE/Graphics/*.cpp
                      ${INC_DIR}/OE/Memory/*.hpp
                      ${INC_DIR}/OE/Memory/*.inl
                      ${SRC_DIR}/OE/Memory/*.cpp
                      #${INC_DIR}/OE/Meta/*.hpp
                      #${SRC_DIR}/OE/Meta/*.cpp
                      ${INC_DIR}/OE/Application/*.hpp
                      ${SRC_DIR}/OE/Application/*.cpp
                      #${INC_DIR}/OE/Engine/*.hpp
                      #${INC_DIR}/OE/Engine/*.inl
                      #${SRC_DIR}/OE/Engine/*.cpp
                      #${INC_DIR}/OE/Scripting/*.hpp
                      #${SRC_DIR}/OE/Scripting/*.cpp
                      ${INC_DIR}/OE/System/*.hpp
                      ${SRC_DIR}/OE/System/*.hpp
                      ${SRC_DIR}/OE/System/*.cpp
                      ${INC_DIR}/OE/UI/*.hpp
                      ${SRC_DIR}/OE/UI/*.cpp
)

list(REMOVE_ITEM SRC
	${INC_DIR}/OE/Misc/FrameDebugger.hpp
	${SRC_DIR}/OE/Misc/FrameDebugger.cpp
)

set(SRC
	${SRC}
	${BIN_INC_DIR}/OE/Config.hpp
)

# Platform code
if(OE_WINDOWS)
	file(GLOB PLATFORM_SRC ${SRC_DIR}/OE/Platform/Windows/*.*
						   ${INC_DIR}/OE/Platform/Windows/*.*)
elseif(OE_EMSCRIPTEN)
	file(GLOB PLATFORM_SRC ${SRC_DIR}/OE/Platform/Emscripten/*.*
						   ${INC_DIR}/OE/Platform/Emscripten/*.*)
elseif(OE_ANDROID)
	# App Glue
	set(ANDROID_NATIVE_APP_GLUE ${ANDROID_NDK}/sources/android/native_app_glue)
	
	file(GLOB PLATFORM_SRC ${SRC_DIR}/OE/Platform/Android/*.*
						   ${INC_DIR}/OE/Platform/Android/*.*
						   ${ANDROID_NATIVE_APP_GLUE}/android_native_app_glue.c
	)
elseif(OE_UNIX)
	file(GLOB PLATFORM_SRC ${SRC_DIR}/OE/Platform/X11/*.*
						   ${INC_DIR}/OE/Platform/X11/*.*)
endif()

# Contexts code (GL, D3D, VK)
if(OE_D3D)
	# Assume Windows...
	file(GLOB D3D_SRC ${SRC_DIR}/OE/Platform/Direct3D/*.*
					  ${INC_DIR}/OE/Platform/Direct3D/*.*)
	set(PLATFORM_SRC ${PLATFORM_SRC} ${D3D_SRC})
endif()
if(OE_OPENGL OR OE_OPENGL_ES)
	file(GLOB OPENGL_SRC ${SRC_DIR}/OE/Platform/OpenGL/*.*
					  	 ${INC_DIR}/OE/Platform/OpenGL/*.*
					  	 ${INC_DIR}/OE/Platform/OpenGL/Shaders/*.glsl)

	# We don't want specific implementations
	file(GLOB SPECIFIC_OPENGL_SRC ${SRC_DIR}/OE/Platform/OpenGL/*Context.*)
	# Except this, which is not an specific implementation
	list(REMOVE_ITEM SPECIFIC_OPENGL_SRC ${SRC_DIR}/OE/Platform/OpenGL/GLContext.cpp)

	list(REMOVE_ITEM OPENGL_SRC ${SPECIFIC_OPENGL_SRC})
	set(PLATFORM_SRC ${PLATFORM_SRC} ${OPENGL_SRC})
endif()
if(OE_OPENGL)
	if(OE_WINDOWS)
		set(PLATFORM_SRC ${PLATFORM_SRC}
						 ${SRC_DIR}/OE/Platform/OpenGL/WGLContext.hpp
						 ${SRC_DIR}/OE/Platform/OpenGL/WGLContext.cpp)
	elseif(OE_LINUX)
		set(PLATFORM_SRC ${PLATFORM_SRC}
						 ${SRC_DIR}/OE/Platform/OpenGL/GLXContext.hpp
						 ${SRC_DIR}/OE/Platform/OpenGL/GLXContext.cpp)
	endif()
endif()
if(OE_OPENGL_ES)
	# If Emscripten, we use the WebGLContext located in Platform/Emscripten/
	if(NOT OE_EMSCRIPTEN)
		set(PLATFORM_SRC ${PLATFORM_SRC}
						 ${SRC_DIR}/OE/Platform/OpenGL/EGLContext.hpp
						 ${SRC_DIR}/OE/Platform/OpenGL/EGLContext.cpp)
	endif()
endif()
if(OE_VULKAN)
	file(GLOB VULKAN_SRC ${SRC_DIR}/OE/Platform/Vulkan/*.*
						 ${INC_DIR}/OE/Platform/Vulkan/*.*)
	set(PLATFORM_SRC ${PLATFORM_SRC} ${VULKAN_SRC})
endif()

# Add the main library
add_library(OrbitEngine ${SRC} ${PLATFORM_SRC})
#add_definitions("-E")

# Include
include_directories(${SRC_DIR})
target_include_directories(OrbitEngine PUBLIC
	${INC_DIR}
	${BIN_INC_DIR}
)

if(OE_ANDROID)
	# Include privatly? the Android native app glue
	target_include_directories(OrbitEngine PUBLIC ${ANDROID_NATIVE_APP_GLUE})
endif()

# - Libraries -
list(APPEND OE_LIBRARIES
	freetype
	FreeImage
	xsc_core
	glsl_optimizer
	assimp
	lib_msdfgen
	#${MONO_LIBRARIES}
)
list(APPEND OE_LIBRARIES_INC
	${freetype_SOURCE_DIR}/include
    ${FreeImage_SOURCE_DIR}/Source
    ${XShaderCompiler_SOURCE_DIR}/inc
    ${Assimp_SOURCE_DIR}/include
    ${Assimp_BINARY_DIR}/include
	Dependencies/glsl-optimizer/src/glsl
	${msdfgen_SOURCE_DIR}
	#${MONO_INCLUDE_DIR}
)

target_compile_definitions(FreeImage PUBLIC FREEIMAGE_LIB)
target_compile_definitions(OrbitEngine PUBLIC FREEIMAGE_LIB)

# Platform specific
if(OE_WINDOWS)

elseif(OE_EMSCRIPTEN)
	target_link_libraries(OrbitEngine PUBLIC "-s USE_WEBGL2=1 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=2 -s WASM=1 -s MODULARIZE=1")
elseif(OE_ANDROID)
    list(APPEND OE_LIBRARIES android log)
elseif(OE_LINUX)
    find_package(X11 REQUIRED)
    find_package(DL REQUIRED)
    list(APPEND OE_LIBRARIES ${X11_LIBRARIES} ${DL_LIBRARIES})
    list(APPEND OE_LIBRARIES_INC ${X11_INCLUDE_DIR} ${DL_INCLUDES})
endif()

# Contexts
if(OE_OPENGL)
    list(APPEND OE_LIBRARIES ${OPENGL_LIBRARIES})
    list(APPEND OE_LIBRARIES_INC ${OPENGL_INCLUDE_DIR})
endif()
if(OE_OPENGL_ES)
	# Emscripten provides it's own libraries
	if(NOT OE_EMSCRIPTEN)
		list(APPEND OE_LIBRARIES ${GLES_LIBRARIES} ${EGL_LIBRARIES})
		list(APPEND OE_LIBRARIES_INC ${GLES_INCLUDE_DIR} ${EGL_INCLUDE_DIRS})
	endif()
endif()
if(OE_D3D)
    list(APPEND OE_LIBRARIES ${DirectX_LIBRARIES})
    list(APPEND OE_LIBRARIES_INC ${DirectX_INCLUDE_DIR})
endif()
if(OE_VULKAN)
    list(APPEND OE_LIBRARIES ${Vulkan_LIBRARIES})
    list(APPEND OE_LIBRARIES_INC ${Vulkan_INCLUDE_DIRS})	
endif()

target_link_libraries(OrbitEngine PUBLIC ${OE_LIBRARIES})
target_include_directories(OrbitEngine PUBLIC ${OE_LIBRARIES_INC})

# Resources
OE_add_resources(Resources OrbitEngine)

if(EMSCRIPTEN)
    target_link_libraries(OrbitEngine PUBLIC "--preload-file vfs@/")

    # TODO Make this an option
    target_link_libraries(OrbitEngine PUBLIC "--shell-file \"${CMAKE_CURRENT_SOURCE_DIR}\"/CMake/Emscripten/template.html")
endif()

# - Binaries -
if(OE_WINDOWS)
	#list(APPEND OE_BINARIES
	#	"${MONO_PATH}/bin/${MONO_N}.dll"
	#)
	set(OE_BINARIES "${OE_BINARIES}" CACHE STRING "")
endif()

# C++11
useCXX11(OrbitEngine)