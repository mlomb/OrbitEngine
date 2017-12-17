#include "OE/Engine/SceneObject.hpp"

#include "OE/Engine/Transform.hpp"
#include "OE/Engine/TestComponent.hpp"

namespace OrbitEngine {	namespace Engine {
	SceneObject::SceneObject(const std::string& name)
		: m_Childs(std::vector<SceneObject*>()),
		m_Parent(0),
		m_Name(name)
	{
		addComponent(new Transform());
		addComponent(new TestComponent());
	}

	SceneObject::~SceneObject()
	{

	}

	void SceneObject::addChildren(SceneObject* sceneObject)
	{
#if OE_EDITOR
		// beginInsertRows
		//if(internal_EditorBeginInsert)
		//	internal_EditorBeginInsert(childCount(), this);
#endif

		// remove from childs list of the parent
		sceneObject->m_Parent = this;
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
	}

	void SceneObject::addComponent(Component* component)
	{
		m_Components.push_back(component);
	}

	SceneObject* SceneObject::parent()
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
} }