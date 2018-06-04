#include "OE/Engine/SceneRunner.hpp"

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
	SceneRunner::SceneRunner(Scene* scene)
		: m_Scene(scene)
	{
	}

	SceneRunner::~SceneRunner()
	{
	}

	static OrbitEngine::Graphics::Mesh* sphereMesh;
	static OrbitEngine::Graphics::Material* mat;

	void SceneRunner::init(OrbitEngine::Application::Context* context)
	{
		m_Context = context;

		m_Renderer2D = new Graphics::BatchRenderer();
		m_Renderer3D = new Graphics::ForwardRenderer();


		m_Renderer3D->addLight(new OrbitEngine::Graphics::DirectionalLight());

		sphereMesh = Graphics::MeshGenerator::SphereStandard();
		mat = new Graphics::Material(Graphics::SurfaceShader::Standard());
		Math::Color c1(0.0f, 0.0f, 1.0f, 1.0f);
		mat->setBaseColor(c1);
		mat->setMetallic(0.04f);
	}

	void SceneRunner::render()
	{
		m_Renderer3D->begin();

		////////////////////////
		// Render all components
		////////////////////////
		//auto& kv = m_Scene->getObjects<MeshRenderer>();
		//for (auto it = kv.first; it != kv.second; it++) {
		//	MeshRenderer* meshRenderer = static_cast<MeshRenderer*>(*it);
		//	m_Renderer3D->submitMesh(sphereMesh, sphereMesh->getIBO()->getSize(), mat, meshRenderer->getTransform()->getMatrix());
		//}

		// traverse the scene
		//renderTraverse(m_Scene->GetRoot());

		m_Renderer3D->end();
	}

	void SceneRunner::update(float delta)
	{
		//metacpp::Storage* storage = metacpp::Runtime::GetStorage();

		////////////////////////
		// Awake any new components
		///////////////////////
		//ITERATE_ALL_COMPONENTS_BEGIN
		//	if (!component->isAwaken()) {
		//		component->awake();
		//		component->setAwaken(true);
		//	}
		//ITERATE_ALL_COMPONENTS_END
		//
		//////////////////////////
		//// Update all components
		//////////////////////////
		//ITERATE_ALL_COMPONENTS_BEGIN
		//	component->update(delta);
		//ITERATE_ALL_COMPONENTS_END

		// Get the main camera
		//Camera* mainCamera = static_cast<Camera*>((*m_Scene->getObjects<Camera>().first));
		//m_Renderer3D->setCamera(mainCamera->getTransform()->Position, mainCamera->getVPMatrix());
	}

	void SceneRunner::deinitialize()
	{
		delete m_Renderer2D;
		delete m_Renderer3D;
	}

	void SceneRunner::renderTraverse(SceneObject* obj)
	{
		Transform* transform = obj->getComponent<Transform>();

		if (transform) {
			for (Component* component : obj->getComponents()) {
				//if (component->getTypeID() == metacpp::TypeInfo<MeshRenderer>::ID) {
				//	m_Renderer3D->submitMesh(sphereMesh, sphereMesh->getIBO()->getSize(), mat, transform->getMatrix());
				//}
			}
		}

		// continue traversing
		std::vector<OrbitEngine::Engine::SceneObject*> childs = obj->getChildrens();
		for (auto& child : childs)
			renderTraverse(child);
	}

} }
