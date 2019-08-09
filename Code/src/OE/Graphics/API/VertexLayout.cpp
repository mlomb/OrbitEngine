#include "OE/Graphics/API/VertexLayout.hpp"

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec3.hpp"
#include "OE/Math/Vec4.hpp"
#include "OE/Math/Color.hpp"

#include "OE/Application/Context.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/OpenGL.hpp"
#endif
#if OE_D3D
	#include "OE/Platform/Direct3D/Direct3D.hpp"
#endif

namespace OrbitEngine {	namespace Graphics {
	void VertexLayout::push(VertexElement attrib) {
		attrib.offset = m_Stride;
		m_Stride += attrib.count * attrib.size;

		m_Childrens.push_back(attrib);
	}

	VertexLayout::VertexLayout() : m_Stride(0)
	{
	}

	VertexLayout::~VertexLayout()
	{
		m_Childrens.clear();
	}

	template<> void VertexLayout::push<unsigned char>(const std::string& name, const unsigned int count, const bool normalized)
	{
		switch (Application::Context::GetCurrentAPI())
		{
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			push({ name, GL_UNSIGNED_BYTE, sizeof(unsigned char), count, normalized });
			break;
#endif
#if OE_D3D
		case DIRECT3D:
			push({ name, DXGI_FORMAT_R8G8B8A8_UNORM, sizeof(unsigned char) * 4, 1, normalized });
			break;
#endif
		}
	}
	
	template<> void VertexLayout::push<unsigned int>(const std::string& name, const unsigned int count, const bool normalized)
	{
		switch (Application::Context::GetCurrentAPI())
		{
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			push({ name, GL_UNSIGNED_INT, sizeof(unsigned int), count, normalized });
			break;
#endif
#if OE_D3D
		case DIRECT3D:
			push({ name, DXGI_FORMAT_R32_UINT, sizeof(unsigned int), count, normalized });
			break;
#endif
		}
	}

	template<> void VertexLayout::push<float>(const std::string& name, const unsigned int count, const bool normalized)
	{
		switch (Application::Context::GetCurrentAPI())
		{
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			push({ name, GL_FLOAT, sizeof(float), count, normalized });
			break;
#endif
#if OE_D3D
		case DIRECT3D:
			push({ name, DXGI_FORMAT_R32_FLOAT, sizeof(float), count, normalized });
			break;
#endif
		}
	}

	template<> void VertexLayout::push<Math::Vec2f>(const std::string& name, const unsigned int count, const bool normalized)
	{
		switch (Application::Context::GetCurrentAPI())
		{
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			push({ name, GL_FLOAT, sizeof(float), 2 * count, normalized });
			break;
#endif
#if OE_D3D
		case DIRECT3D:
			push({ name, DXGI_FORMAT_R32G32_FLOAT, sizeof(float), 2 * count, normalized });
			break;
#endif
		}
	}

	template<> void VertexLayout::push<Math::Vec3f>(const std::string& name, const unsigned int count, const bool normalized)
	{
		switch (Application::Context::GetCurrentAPI())
		{
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			push({ name, GL_FLOAT, sizeof(float), 3 * count, normalized });
			break;
#endif
#if OE_D3D
		case DIRECT3D:
			push({ name, DXGI_FORMAT_R32G32B32_FLOAT, sizeof(float), 3 * count, normalized });
			break;
#endif
		}
	}

	template<> void VertexLayout::push<Math::Vec4f>(const std::string& name, const unsigned int count, const bool normalized)
	{
		switch (Application::Context::GetCurrentAPI())
		{
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			push({ name, GL_FLOAT, sizeof(float), 4 * count, normalized });
			break;
#endif
#if OE_D3D
		case DIRECT3D:
			push({ name, DXGI_FORMAT_R32G32B32A32_FLOAT, sizeof(float), 4 * count, normalized });
			break;
#endif
		}
	}

	template<> void VertexLayout::push<Math::Color3f>(const std::string& name, const unsigned int count, const bool normalized)
	{
		push<Math::Vec3f>(name, count, normalized);
	}

	template<> void VertexLayout::push<Math::Color4f>(const std::string& name, const unsigned int count, const bool normalized)
	{
		push<Math::Vec4f>(name, count, normalized);
	}
} }