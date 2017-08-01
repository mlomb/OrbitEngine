#ifndef GRAPHICS_GLVERTEX_BUFFER_HPP
#define GRAPHICS_GLVERTEX_BUFFER_HPP

#include "OE/Platform/OpenGL/GLBuffer.hpp"

namespace OrbitEngine {	namespace Graphics {
	class GLVertexBuffer : public GLBuffer, public VertexBuffer {
	public:
		GLVertexBuffer(const size_t size, const void* data);
	};
} } 

#endif