#include "OE/Platform/OpenGL/GLVertexBuffer.hpp"

namespace OrbitEngine {	namespace Graphics {
	GLVertexBuffer::GLVertexBuffer(const size_t size, const void* data)
		: GLBuffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, (unsigned int)size, data)
	{
	}
} }