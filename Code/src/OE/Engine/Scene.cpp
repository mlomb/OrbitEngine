#include "OE/Engine/Scene.hpp"

#include "OE/Engine/SceneObject.hpp"
#include "OE/Engine/TestComponent.hpp"
#include "OE/Engine/EulerCameraController.hpp"
#include "OE/Engine/Camera.hpp"

namespace OrbitEngine {	namespace Engine {

	Scene::Scene() {
		m_Root = new SceneObject();
		
		m_Root->m_Scene = this;
		m_Root->setName("Root");

		// mocking af
		{
			m_Root->addChildren("Child1");

			OrbitEngine::Engine::SceneObject* c2 = m_Root->addChildren("Child2");
			{
				c2->addChildren("Child2.1");
				c2->addChildren("Child2.2");
			}
			m_Root->addChildren("Child3");
			c2->addComponent<TestComponent>();

			OrbitEngine::Engine::SceneObject* cam = m_Root->addChildren("Camara");
			cam->addComponent<Camera>();
			cam->addComponent<EulerCameraController>();
		}
		
	}

	SceneObject* Scene::GetRoot()
	{
		return m_Root;
	}

	//std::unordered_map<metacpp::TypeID, Misc::TrackedMemoryPool*>& Scene::getAllObjects()
	//{
	//	return m_Objects;
	//}
} }