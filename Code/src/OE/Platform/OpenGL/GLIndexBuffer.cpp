#include "OE/Platform/OpenGL/GLIndexBuffer.hpp"

namespace OrbitEngine {	namespace Graphics {
	GLIndexBuffer::GLIndexBuffer(const size_t size, const void* data)
		: GLBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, size, data)
	{
	}
} }