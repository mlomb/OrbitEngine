#ifndef ENGINE_SCENE_RUNNER_HPP
#define ENGINE_SCENE_RUNNER_HPP

#include "OE/Engine/Scene.hpp"
#include "OE/Engine/SceneObject.hpp"
#include "OE/Application/Loopeable.hpp"

#include "OE/Graphics/2D/Renderer2D.hpp"
#include "OE/Graphics/3D/Renderer3D.hpp"

#define MAX_3D_COMMANDS 1000

namespace OrbitEngine {
	namespace Engine {

#define ITERATE_ALL_COMPONENTS_BEGIN \
	for (auto& kv : m_Scene->getAllObjects()) { \
	if (storage->IsDerived(kv.first, metacpp::TypeInfo<Component>::ID)) { \
		Misc::TrackedMemoryPool* components = static_cast<Misc::TrackedMemoryPool*>(kv.second); \
		auto it_begin = components->begin(); \
		auto it_end = components->end(); \
		for (auto it = it_begin; it != it_end; it++) { \
			Component* component = static_cast<Component*>(*it); \

#define ITERATE_ALL_COMPONENTS_END } } }

	// TODO Find a better name
	class SceneRunner : public Application::Loopeable {
	public:
		SceneRunner(Scene* scene);
		~SceneRunner();

		void init(OrbitEngine::Application::Context* context) override;
		void render() override;
		void update(float delta) override;
		void deinitialize() override;

		void renderTraverse(SceneObject* obj);
		
	private:
		Application::Context* m_Context;

		Scene* m_Scene;

		Graphics::Renderer2D* m_Renderer2D;
		Graphics::Renderer3D* m_Renderer3D;
	};
} }

#endif