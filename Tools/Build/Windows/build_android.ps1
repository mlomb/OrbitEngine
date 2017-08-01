$targetSystem = "Android"
$ScriptRoot = Split-Path $MyInvocation.MyCommand.Path
. "$ScriptRoot\common.ps1"

echo "********** BUILDING FOR ANDROID ON WINDOWS **********"

$androidNDK = "$env:ANDROID_NDK"

Push-Location $buildPath
cmake.exe -GNinja `
		  -DCMAKE_TOOLCHAIN_FILE="$androidNDK\build\cmake\android.toolchain.cmake" `
		  -DANDROID_TOOLCHAIN=gcc `
		  "$basePath"
ninja
Pop-Location