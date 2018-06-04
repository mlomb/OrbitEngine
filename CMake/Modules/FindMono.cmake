set(MONO_V "mono-2.0")
set(MONO_N "${MONO_V}-sgen")

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
set(MONO_LIBRARIES   "${MONO_N}.lib")

message("Mono Path: ${MONO_PATH}")
# message(${MONO_INCLUDE_DIR})
# message(${MONO_LIBRARY_DIR})
# message(${MONO_LIBRARIES})