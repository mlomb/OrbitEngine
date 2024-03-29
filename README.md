[![Build Status](https://img.shields.io/travis/mlomb/OrbitEngine.svg?label=Linux/OSX/Android/Emscripten%20builds)](https://travis-ci.org/mlomb/OrbitEngine)
[![Build Status](https://img.shields.io/appveyor/ci/mlomb/OrbitEngine.svg?label=Windows/Linux%20build)](https://ci.appveyor.com/project/mlomb/orbitengine)

# OrbitEngine
OrbitEngine is a cross-platform and multi-API graphics engine. It is a personal project.

Supported APIs:
* OpenGL & OpenGLES
* Direct3D11

Supported platforms:
* Android
* Emscripten
* Linux
* Windows

## Dependencies
Currently, OrbitEngine is making use of this libraries:
* [FreeType](https://www.freetype.org/), an open source library to render fonts
* [FreeImage](http://freeimage.sourceforge.net/), an open source library to manage popular graphics images formats
* [XShaderCompiler](https://github.com/LukasBanana/XShaderCompiler/), an open source library to cross compiler shaders
* [Assimp](https://github.com/assimp/assimp/), an open source library to import and export 3d model formats

All are included as submodules.

## Building
OrbitEngine relies on the [CMake build system](https://cmake.org/) and requires a CXX11 compiler.

Don't forget to update the submodules or clone the repositry with `--recursive`.

### Windows
You can just run `cmake` and then compile it with Visual Studio.

### Web (Emscripten)
On Windows, you must to have installed the [Emscripten SDK](https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html) and the [Ninja build system](https://ninja-build.org/).

Then, you have to run `cmake` with the Emscripten toolchain and the generator `Ninja`. For example:
```shell
cmake -GNinja -DCMAKE_TOOLCHAIN_FILE="%EMSCRIPTEN_SDK%\cmake\Modules\Platform\Emscripten.cmake"
```
After that, you can run `ninja` on the output directory.
### Android
To build for Android on Windows, you must have the Android NDK 25.2.5. The newer versions of the NDK deprecated the way we create the APK files. (This is something to refactor)

In you Application CMakeLists you should call the procedure `android_create_apk` to generate the APK file:

```cmake
if(ANDROID)	
	# Make sure that the Android entry point isnt getting stripped, find a better solution
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

	include(${OrbitEngine_SOURCE_DIR}/CMake/Android/APK.cmake)
	android_create_apk(Sandbox "${CMAKE_BINARY_DIR}/APK" "libApplication.so" "")
endif()
```
Don't forget to link it as a shared library!

Now you run CMake with the Android toolchain. For example:
```shell
cmake -GNinja -DCMAKE_TOOLCHAIN_FILE="%ANDROID_NDK%\build\cmake\android.toolchain.cmake" -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-21
```
The entry point is handled by the Engine, you should use the common `main`. This allows the code only be written once between platforms.

### Linux
You can use CMake with Make.

Don't forget to download these packages:
```shell
# X11
sudo apt-get install libx11-dev
# GL
sudo apt-get install freeglut3-dev
# GLES1 & GLES2 & EGL
sudo apt-get install libgles1-mesa libgles2-mesa-dev
```

## License
See [LICENSE](LICENSE).
