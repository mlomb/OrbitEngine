#include "OE/Graphics/API/Buffer.hpp"

#include "OE/Application/Context.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLIndexBuffer.hpp"
	#include "OE/Platform/OpenGL/GLVertexBuffer.hpp"
#endif
#if OE_D3D
	#include "OE/Platform/Direct3D/D3DVertexBuffer.hpp"
	#include "OE/Platform/Direct3D/D3DIndexBuffer.hpp"
#endif

namespace OrbitEngine {	namespace Graphics {

	Buffer::Buffer()
		: p_Size(0)
	{
	}

	VertexBuffer* Buffer::CreateVertexBuffer(VertexLayout* layout, const unsigned int size, const void* data)
	{
		VertexBuffer* vertexBuffer = 0;
		switch (Application::Context::GetCurrentAPI())
		{
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			vertexBuffer = new GLVertexBuffer(size, data);
			break;
#endif
#if OE_D3D
		case DIRECT3D:
			vertexBuffer = new D3DVertexBuffer(size, data);
			break;
#endif
		default:
			OE_ASSERT(false);
			break;
		}
		if (vertexBuffer != 0)
			vertexBuffer->setLayout(layout);

		return vertexBuffer;
	}
	
	unsigned int Buffer::getSize()
	{
		return p_Size;
	}

	Buffer* Buffer::CreateIndexBuffer(const unsigned int size, const void* data)
	{
		switch (Application::Context::GetCurrentAPI())
		{
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			return new GLIndexBuffer(size, data);
#endif
#if OE_D3D
		case DIRECT3D:
			return new D3DIndexBuffer(size, data);
#endif
		default:
			OE_ASSERT(false);
			return 0;
		}
	}
} }
