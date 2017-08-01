#ifndef GRAPHICS_GLINDEX_BUFFER_HPP
#define GRAPHICS_GLINDEX_BUFFER_HPP

#include "OE/Platform/OpenGL/GLBuffer.hpp"

namespace OrbitEngine {	namespace Graphics {
	class GLIndexBuffer : public GLBuffer {
	public:
		GLIndexBuffer(const size_t size, const void* data);
	};
} } 

#endif