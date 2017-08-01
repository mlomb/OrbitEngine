$targetSystem = "Emscripten"
$ScriptRoot = Split-Path $MyInvocation.MyCommand.Path
. "$ScriptRoot\common.ps1"

echo "********** BUILDING FOR EMSCRIPTEN ON WINDOWS **********"

$emscriptenPath = "$env:EMSCRIPTEN"

Push-Location $buildPath
cmake.exe -GNinja `
		  -DCMAKE_TOOLCHAIN_FILE="$emscriptenPath\cmake\Modules\Platform\Emscripten.cmake" `
		  -DCMAKE_BUILD_TYPE=Release `
		  -Wno-deprecated `
		  "$basePath"
ninja
Pop-Location