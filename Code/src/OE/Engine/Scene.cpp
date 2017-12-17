#include "OE/Engine/Scene.hpp"

#include "OE/Engine/TestComponent.hpp"

namespace OrbitEngine {	namespace Engine {

	Scene::Scene()
		: m_Root(new OrbitEngine::Engine::SceneObject("Root"))
	{
		// mocking af
		{
			m_Root->addChildren(new OrbitEngine::Engine::SceneObject("Child1"));

			OrbitEngine::Engine::SceneObject* c2 = new OrbitEngine::Engine::SceneObject("Child2");
			m_Root->addChildren(c2);
			{
				c2->addChildren(new OrbitEngine::Engine::SceneObject("Child2.1"));
				c2->addChildren(new OrbitEngine::Engine::SceneObject("Child2.2"));
			}
			m_Root->addChildren(new OrbitEngine::Engine::SceneObject("Child3"));
			c2->addComponent(new OrbitEngine::Engine::TestComponent());
		}
	}

	SceneObject* Scene::getRoot()
	{
		return m_Root;
	}
} }