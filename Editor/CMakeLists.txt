
set(CEF_USE_SANDBOX OFF CACHE BOOL "Force turning off of sandbox")
set(CEF_VERSION 84.2.1+g3f2c372+chromium-84.0.4147.89) # latest as of 19/07/2020
set(CEF_CMAKE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

include(../Dependencies/cef-cmake/cmake/cef_cmake.cmake)
add_subdirectory(../Dependencies/cef-cmake cef-cmake)

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Code/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Code/src)

file(GLOB_RECURSE SRC ${INC_DIR}/*.hpp
					  ${INC_DIR}/*.inl
                      ${SRC_DIR}/*.cpp
)

add_executable(OrbitEngine-Editor
    ${SRC}
    ${CEF_CMAKE_EXECUTABLE_RESOURCES}
)
target_include_directories(OrbitEngine-Editor PUBLIC
	${INC_DIR}
)

target_link_libraries(OrbitEngine-Editor OrbitEngine cefdll_wrapper)

# temporal fix: copy Resources folder
add_custom_command(TARGET OrbitEngine-Editor POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_directory
				   ${CMAKE_CURRENT_SOURCE_DIR}/../Resources ${CMAKE_CURRENT_BINARY_DIR}/Resources)
