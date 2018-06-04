#ifndef ENGINE_SCENE_RENDERER_HPP
#define ENGINE_SCENE_RENDERER_HPP

#include "OE/Engine/Scene.hpp"
#include "OE/Engine/SceneObject.hpp"
#include "OE/Application/Loopeable.hpp"

#include "OE/Graphics/2D/Renderer2D.hpp"
#include "OE/Graphics/3D/Renderer3D.hpp"

#define MAX_3D_COMMANDS 1000

namespace OrbitEngine { namespace Engine {

	class SceneRenderer {
	public:
		SceneRenderer(Scene* scene);
		virtual ~SceneRenderer();

		void render();

	protected:

	private:

		Scene* m_Scene;

		Graphics::Renderer2D* m_Renderer2D;
		Graphics::Renderer3D* m_Renderer3D;
	};
} }

#endif