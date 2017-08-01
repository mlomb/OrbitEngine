#
#   Copyright 2013 Pixar
#
#   Licensed under the Apache License, Version 2.0 (the "Apache License")
#   with the following modification; you may not use this file except in
#   compliance with the Apache License and the following modification to it:
#   Section 6. Trademarks. is deleted and replaced with:
#
#   6. Trademarks. This License does not grant permission to use the trade
#      names, trademarks, service marks, or product names of the Licensor
#      and its affiliates, except as required to comply with Section 4(c) of
#      the License and to reproduce the content of the NOTICE file.
#
#   You may obtain a copy of the Apache License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the Apache License with the above modification is
#   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#   KIND, either express or implied. See the Apache License for the specific
#   language governing permissions and limitations under the Apache License.
#

# - Try to find OpenGLES
# Once done this will define
#  
#  GLES_FOUND        - system has OpenGLES
#  GLES_INCLUDE_DIR  - the GL include directory
#  GLES_LIBRARIES    - Link these to use OpenGLES

if(ANDROID)
    FIND_PATH( GLES_INCLUDE_DIR
        GLES3/gl3.h
        "${ANDROID_STANDALONE_TOOLCHAIN}/usr/include"
    )

    FIND_LIBRARY( GLES_LIBRARIES
        NAMES
            GLESv3
        PATHS
            "${ANDROID_STANDALONE_TOOLCHAIN}/usr/lib"
    )

elseif(IOS)
    FIND_PATH( GLES_INCLUDE_DIR
        OpenGLES/ES2/gl.h
    )

    FIND_LIBRARY( GLES_FRAMEWORKS OpenGLES )

    if(GLES_FRAMEWORKS)
        set( GLES_LIBRARIES "-framework OpenGLES" )
    endif()

else()
    find_path(GLES_INCLUDE_DIR GLES/gl.h)
    find_library(GLES_LIBRARIES NAMES GLESv1_CM)
    
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(GLES DEFAULT_MSG GLES_LIBRARIES GLES_INCLUDE_DIR)
endif()

SET( GLES_FOUND "NO" )
IF(GLES_LIBRARIES)
    SET( GLES_FOUND "YES" )
ENDIF(GLES_LIBRARIES)

MARK_AS_ADVANCED(
  GLES_INCLUDE_DIR
  GLES_LIBRARIES
)
