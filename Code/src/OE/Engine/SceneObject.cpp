#include "OE/Engine/SceneObject.hpp"

#include "OE/Engine/Transform.hpp"
#include "OE/Engine/TestComponent.hpp"

namespace OrbitEngine {	namespace Engine {
	SceneObject::SceneObject()
		: m_Childs(std::vector<SceneObject*>()), m_Parent(0)
	{
	}

	SceneObject::~SceneObject()
	{

	}

	SceneObject* SceneObject::addChildren(const std::string& name)
	{
#if OE_EDITOR
		// beginInsertRows
		//if(internal_EditorBeginInsert)
		//	internal_EditorBeginInsert(childCount(), this);
#endif

		SceneObject* sceneObject = m_Scene->allocateObject<SceneObject>();
		if (!sceneObject) {
			OE_LOG_WARNING("Can't allocate more SceneObjects!");
			return 0;
		}

		sceneObject->m_Parent = this;
		sceneObject->m_Scene = m_Scene;

		sceneObject->setName(name);
		sceneObject->addComponent<Transform>();

#if OE_EDITOR
		sceneObject->internal_EditorBeginInsert = internal_EditorBeginInsert;
		sceneObject->internal_EditorEndInsert = internal_EditorEndInsert;
#endif

		m_Childs.push_back(sceneObject);

#if OE_EDITOR
		// endInsertRows
		//if (internal_EditorEndInsert)
		//	internal_EditorEndInsert();
#endif

		return sceneObject;
	}

	Scene* SceneObject::getScene()
	{
		return m_Scene;
	}

	SceneObject* SceneObject::getParent()
	{
		return m_Parent;
	}

	SceneObject* SceneObject::childAt(int i)
	{
		if (i < 0 || i >= m_Childs.size())
			return 0;
		return m_Childs[i];
	}

	int SceneObject::childCount() const
	{
		return m_Childs.size();
	}

	std::vector<SceneObject*>& SceneObject::getChildrens()
	{
		return m_Childs;
	}

	std::vector<Component*>& SceneObject::getComponents()
	{
		return m_Components;
	}

	std::string SceneObject::getName()
	{
		return m_Name;
	}

	void SceneObject::setName(const std::string& name)
	{
		m_Name = name;
	}
} }