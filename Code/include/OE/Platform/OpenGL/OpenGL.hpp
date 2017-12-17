#ifndef OPENGL_HPP
#define OPENGL_HPP

#include "OE/Config.hpp"

#if OE_OPENGL_ANY

#define GL_STATIC_DRAW                    0x88E4
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_COMPILE_STATUS                 0x8B81
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_GEOMETRY_SHADER                0x8DD9
#define GL_TESS_EVALUATION_SHADER         0x8E87
#define GL_TESS_CONTROL_SHADER            0x8E88
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#define GL_TEXTURE0                       0x84C0
#define GL_STREAM_DRAW                    0x88E0
#define GL_RG                             0x8227
#define GL_RG_INTEGER                     0x8228
#define GL_R8                             0x8229
#define GL_R16                            0x822A
#define GL_R32F                           0x822E
#define GL_RGBA32F                        0x8814
#define GL_RGB32F                         0x8815
#define GL_RGB16F                         0x881B
#define GL_RG32F                          0x8230
#define GL_MIRRORED_REPEAT                0x8370
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_CLAMP_TO_BORDER                0x812D
#define GL_MAP_READ_BIT                   0x0001
#define GL_MAP_WRITE_BIT                  0x0002
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ACTIVE_UNIFORM_BLOCKS          0x8A36
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_2D_ARRAY               0x8C1A
#define GL_TEXTURE_3D                     0x806F
#define GL_MAX_UNIFORM_BUFFER_BINDINGS    0x8A2F
#define GL_LINK_STATUS                    0x8B82


	#if OE_OPENGL
		#if OE_WINDOWS
			#include "OE/Platform/Windows/Windows.hpp"
		#elif OE_LINUX
			#include <GL/glx.h>
		#endif
		#include <GL/gl.h>
	#endif

	#if OE_OPENGL_ES
		#include <EGL/egl.h>
		#if !OE_OPENGL
			// Only include GLES3 header if we didn't include the desktop OpenGL one
			#include <GLES3/gl3.h>
		#endif
	#endif

	#include "OE/Platform/OpenGL/OpenGLEXT.hpp"

	#include <string>
	#include "OE/Misc/Log.hpp"

	namespace {
		void glCheckError(const char* file = __FILE__, int line = __LINE__) {
			std::string errType;
			GLenum err = glGetError();
			bool error = false;
			while (err != GL_NO_ERROR) {
				error = true;
				switch (err) {
				case GL_INVALID_ENUM: errType = "GL_INVALID_ENUM"; break;
				case GL_INVALID_OPERATION: errType = "GL_INVALID_OPERATION"; break;
				case GL_INVALID_VALUE: errType = "GL_INVALID_VALUE"; break;
				case GL_OUT_OF_MEMORY: errType = "GL_OUT_OF_MEMORY"; break;
				//default: errType = "(" + std::to_string(err) + ")"; break;
				}
				OrbitEngine::Log::log(file, line, ("OpenGL error: " + errType).c_str(), OrbitEngine::LOG_ERROR);
				err = glGetError();
			}
			if(error){
				//OE_BREAK
			}
		}
	}

	#if OE_DEBUG
		#define OE_CHECK_GL(__statement__) \
			__statement__; \
			glCheckError(__FILE__, __LINE__);
	#else
		#define OE_CHECK_GL(__statement__) __statement__;
	#endif
#endif

#endif