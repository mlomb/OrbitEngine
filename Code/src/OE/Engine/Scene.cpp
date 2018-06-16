#include "OE/Engine/Scene.hpp"

#include "OE/Engine/MemoryDomain.hpp"
#include "OE/Engine/SceneObject.hpp"
#include "OE/Engine/TestComponent.hpp"
#include "OE/Engine/EulerCameraController.hpp"
#include "OE/Engine/Camera.hpp"

namespace OrbitEngine {	namespace Engine {

	Scene::Scene() {
		m_Root = MemoryDomain::Get()->Create<SceneObject>();
		m_Root->m_Self = m_Root;
		//m_Root->m_Scene = WeakPtr<Scene>(this);
		m_Root->SetName("Root");

		// mocking af
		{
			m_Root->AddChildren()->SetName("Child 1");

			WeakPtr<OrbitEngine::Engine::SceneObject> c2 = m_Root->AddChildren();
			c2->SetName("Child 2");
			{
				c2->AddChildren()->SetName("Child 2.1");
				c2->AddChildren()->SetName("Child 2.2");
			}
			m_Root->AddChildren()->SetName("Child 3");
			//c2->addComponent<TestComponent>();

			WeakPtr<OrbitEngine::Engine::SceneObject> cam = m_Root->AddChildren();
			c2->SetName("Camera");
			//cam->addComponent<Camera>();
			//cam->addComponent<EulerCameraController>();
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