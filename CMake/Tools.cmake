include(${CMAKE_CURRENT_LIST_DIR}/Config.cmake)

macro(OE_add_resources folder target)
	if(EMSCRIPTEN)
		set(virtual_folder ${virtual_folder}/vfs)
	elseif(ANDROID)
		set(virtual_folder ${virtual_folder}/APK/assets)
	endif()

	get_filename_component(folder_name ${folder} NAME)
	set(virtual_folder ${virtual_folder}/${folder_name})

	# If we are in Windows or Linux, we can copy the resources after the build has finished
	set(build_order POST_BUILD)
	# But if we are in Android or Emscripten where the resources must be packed we need them be copied before
	if(ANDROID OR EMSCRIPTEN)
		set(build_order PRE_BUILD)
	endif()

	add_custom_command(TARGET ${target} ${build_order}
					   COMMAND ${CMAKE_COMMAND} -E copy_directory
					   ${CMAKE_CURRENT_LIST_DIR}/${folder} ${CMAKE_BINARY_DIR}/${virtual_folder})
endmacro(OE_add_resources folder target)

macro(useCXX11 target)
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if(COMPILER_SUPPORTS_CXX11)
	    target_compile_options(${target} PUBLIC -std=c++11)
	elseif(COMPILER_SUPPORTS_CXX0X)
	    target_compile_options(${target} PUBLIC -std=c++0x)
	else()
	    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif()
endmacro(useCXX11 target)