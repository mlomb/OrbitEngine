#include "OE/Engine/SceneObject.hpp"

namespace OrbitEngine {	namespace Engine {
	SceneObject::SceneObject(const std::string& name)
		: m_Childs(std::vector<SceneObject*>()),
		m_Parent(0),
		m_Name(name)
	{

	}

	SceneObject::~SceneObject()
	{

	}

	void SceneObject::addChildren(SceneObject* sceneObject)
	{
#if OE_EDITOR
		// beginInsertRows
		if(internal_EditorBeginInsert)
			internal_EditorBeginInsert(childCount(), this);
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
		if (internal_EditorEndInsert)
			internal_EditorEndInsert();
#endif
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

	std::string SceneObject::getName() const
	{
		return m_Name;
	}
} }