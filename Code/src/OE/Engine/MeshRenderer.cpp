#include "OE/Engine/MeshRenderer.hpp"

#include "OE/Graphics/MeshGenerator.hpp"

namespace OrbitEngine {	namespace Engine {
	MeshRenderer::MeshRenderer()
		: m_Material(0), m_Mesh(0)
	{
		// for now
		m_Mesh = Graphics::MeshGenerator::Box();
	}

	MeshRenderer::~MeshRenderer()
	{
		// for now
		delete m_Mesh;
	}

	NATIVE_REFLECTION_BEGIN(MeshRenderer)
		NATIVE_REFLECTION_MEMBER(m_Mesh)
		NATIVE_REFLECTION_MEMBER(m_Material)
	NATIVE_REFLECTION_END()
} }