cmake_minimum_required(VERSION 3.2)

project(OrbitEngine CXX)

include(CMake/Config.cmake)
include(CMake/Tools.cmake)

# Workaround with clang
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "-stdlib=libc++")
endif()

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Code/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Code/src)
set(BIN_DIR ${CMAKE_BINARY_DIR})
set(BIN_INC_DIR ${BIN_DIR}/include)

# Configure macros file
configure_file(${INC_DIR}/OE/Config.hpp.in
			   ${BIN_INC_DIR}/OE/Config.hpp)

# - Dependencies -
add_subdirectory(Dependencies/FreeType)
add_subdirectory(Dependencies/FreeImage)
option(XSC_BUILD_SHELL "" OFF)
add_subdirectory(Dependencies/XShaderCompiler)

# - Code -
# General code
file(GLOB_RECURSE SRC ${INC_DIR}/OE/Misc/*.hpp
                      ${SRC_DIR}/OE/Misc/*.cpp
                      ${INC_DIR}/OE/Math/*.hpp
                      ${SRC_DIR}/OE/Math/*.cpp
                      ${INC_DIR}/OE/Graphics/*.hpp
                      ${SRC_DIR}/OE/Graphics/*.cpp
                      ${INC_DIR}/OE/Application/*.hpp
                      ${SRC_DIR}/OE/Application/*.cpp
                      ${INC_DIR}/OE/System/*.hpp
                      ${SRC_DIR}/OE/System/*.cpp
                      ${INC_DIR}/OE/UI/*.hpp
                      ${SRC_DIR}/OE/UI/*.cpp
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
						   ${ANDROID_NATIVE_APP_GLUE}/android_native_app_glue.c)
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
	set(PLATFORM_SRC ${PLATFORM_SRC}
					 ${SRC_DIR}/OE/Platform/OpenGL/EGLContext.hpp
					 ${SRC_DIR}/OE/Platform/OpenGL/EGLContext.cpp)
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
	freeimage
	xsc_core
)
list(APPEND OE_LIBRARIES_INC
	${freetype_SOURCE_DIR}/include
    ${FREEIMAGE_SOURCE_DIR}
    ${XShaderCompiler_SOURCE_DIR}/inc
)

target_compile_definitions(freeimage PUBLIC FREEIMAGE_LIB)
target_compile_definitions(OrbitEngine PUBLIC FREEIMAGE_LIB)

# Platform specific
if(OE_WINDOWS)

elseif(OE_EMSCRIPTEN)
	target_link_libraries(OrbitEngine PUBLIC "-s USE_WEBGL2=1 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=2")
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

# Better way of doing this?
list(APPEND OE_LIBRARIES ${XMAKE_FREEIMAGE_LINK_TARGETS})

target_link_libraries(OrbitEngine PUBLIC ${OE_LIBRARIES})
target_include_directories(OrbitEngine PUBLIC ${OE_LIBRARIES_INC})

# Resources
OE_add_resources(Resources OrbitEngine)

if(EMSCRIPTEN)
    target_link_libraries(OrbitEngine PUBLIC "--preload-file vfs@/")

    # TODO Make this an option
    target_link_libraries(OrbitEngine PUBLIC "--shell-file \"${CMAKE_CURRENT_SOURCE_DIR}\"/CMake/Emscripten/template.html")
endif()

# C++11
useCXX11(OrbitEngine)