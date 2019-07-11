#include "OE/Engine/Scene.hpp"

#include "OE/Engine/MemoryDomain.hpp"
#include "OE/Engine/SceneObject.hpp"
#include "OE/Engine/TestComponent.hpp"
#include "OE/Engine/EulerCameraController.hpp"
#include "OE/Engine/Camera.hpp"
#include "OE/Engine/MeshRenderer.hpp"

namespace OrbitEngine {	namespace Engine {

	Scene::Scene() {
		m_Root = MemoryDomain::Get()->Create<SceneObject>();
		m_Root->m_Self = m_Root;
		//m_Root->m_Scene = WeakPtr<Scene>(this);
		m_Root->SetName("Root");





		// ---
		WeakPtr<OrbitEngine::Engine::SceneObject> camera = m_Root->AddChildren();
		camera->SetName("Camera");
		camera->AddComponent<Camera>();

		{
			WeakPtr<OrbitEngine::Engine::SceneObject> obj = m_Root->AddChildren();
			obj->SetName("Obj1");
			obj->AddComponent<MeshRenderer>();
		}
		{
			WeakPtr<OrbitEngine::Engine::SceneObject> obj = m_Root->AddChildren();
			obj->SetName("Obj2");
			obj->AddComponent<MeshRenderer>();
		}
	}

	Scene::~Scene() {

	}

	WeakPtr<SceneObject> Scene::GetRoot()
	{
		return m_Root;
	}

	//std::unordered_map<metacpp::TypeID, Misc::TrackedMemoryPool*>& Scene::getAllObjects()
	//{
	//	return m_Objects;
	//}

	NATIVE_REFLECTION_BEGIN(Scene)
	NATIVE_REFLECTION_END()
} }