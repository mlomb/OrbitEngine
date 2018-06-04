#include "OE/Engine/SceneRenderer.hpp"

#include "OE/Misc/MemoryPool.hpp"
#include "OE/Graphics/MeshGenerator.hpp"
#include "OE/Graphics/3D/SurfaceShader.hpp"

#include <OE/Graphics/3D/ForwardRenderer.hpp>
#include <OE/Graphics/3D/DeferredRenderer.hpp>
#include <OE/Graphics/2D/BatchRenderer.hpp>

#include "OE/Engine/Transform.hpp"
#include "OE/Engine/Camera.hpp"
#include "OE/Engine/TestComponent.hpp"
#include "OE/Engine/MeshRenderer.hpp"

namespace OrbitEngine {	namespace Engine {
	SceneRenderer::SceneRenderer(Scene* scene)
		: m_Scene(scene)
	{
	}

	SceneRenderer::~SceneRenderer()
	{
	}

	void SceneRenderer::render()
	{
	}

} }
