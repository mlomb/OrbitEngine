$targetSystem = "Android"
$ScriptRoot = Split-Path $MyInvocation.MyCommand.Path
. "$ScriptRoot\common.ps1"

echo "********** BUILDING FOR ANDROID ON WINDOWS **********"

$androidNDK = "$env:ANDROID_SDK\ndk-bundle"

Push-Location $buildPath
cmake.exe -GNinja `
		  -DCMAKE_TOOLCHAIN_FILE="$androidNDK\build\cmake\android.toolchain.cmake" `
		  -DANDROID_ABI=armeabi-v7a `
		  -DANDROID_PLATFORM=android-21 `
		  -DANDROID_STL=c++_static `
		  -DANDROID_TOOLCHAIN=gcc `
		  "$basePath"
ninja
Pop-Location