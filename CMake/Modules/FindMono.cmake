set(MONO_V "mono-2.0")

find_path(
	MONO_PATH
	NAMES
	  "include/${MONO_V}/mono/jit/jit.h"
	PATHS
	  "$ENV{ProgramFiles}/Mono"
	  "$ENV{ProgramFiles} (x86)/Mono"
)

set(MONO_INCLUDE_DIR "${MONO_PATH}/include/${MONO_V}")
set(MONO_LIBRARY_DIR "${MONO_PATH}/lib")

if(EXISTS "${MONO_LIBRARY_DIR}/${MONO_V}-sgen.lib")
	set(MONO_N "${MONO_V}-sgen")
else()
	set(MONO_N "monosgen-2.0")
endif()

find_library(MONO_LIBRARIES ${MONO_N} PATHS ${MONO_LIBRARY_DIR})

message("Mono Path: ${MONO_PATH}")
#message(${MONO_INCLUDE_DIR})
#message(${MONO_LIBRARY_DIR})
#message(${MONO_LIBRARIES})