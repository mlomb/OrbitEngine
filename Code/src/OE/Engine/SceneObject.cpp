#include "OE/Engine/SceneObject.hpp"

#include "OE/Engine/Transform.hpp"
#include "OE/Engine/TestComponent.hpp"

#include "OE/Math/Math.hpp"

namespace OrbitEngine {	namespace Engine {
	SceneObject::SceneObject()
		: m_Childs(std::vector<SceneObject*>()), m_Parent(0)
	{
		memset(&m_Test, 0, sizeof(Test));
	}

	SceneObject::~SceneObject()
	{

	}

	SceneObject* SceneObject::addChildren(const std::string& name)
	{
		SceneObject* sceneObject = new SceneObject();
		if (!sceneObject) {
			OE_LOG_WARNING("Can't allocate more SceneObjects!");
			return 0;
		}

		sceneObject->m_Parent = this;
		sceneObject->m_Scene = m_Scene;

		sceneObject->setName(name);
		sceneObject->addComponent<Transform>();

		m_Childs.push_back(sceneObject);

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
		return int(m_Childs.size());
	}

	std::vector<SceneObject*>& SceneObject::getChildrens()
	{
		return m_Childs;
	}

	std::vector<Component*>& SceneObject::getComponents()
	{
		return m_Components;
	}

	std::string SceneObject::getName() const
	{
		return m_Name;
	}

	void SceneObject::setName(const std::string& name)
	{
		m_Name = name;
	}

	void SceneObject::parent(SceneObject* parent, int position) {
		position = Math::clamp(position, 0, (int)(parent->m_Childs.size()) - 1);

		auto it = std::find(m_Parent->m_Childs.begin(), m_Parent->m_Childs.end(), this);

		if (parent == m_Parent) {
			if (position == it - m_Parent->m_Childs.begin()) {
				return;
			}
		}

		m_Parent->m_Childs.erase(it);


		parent->m_Childs.insert(parent->m_Childs.begin() + position, this);
		m_Parent = parent;
	}

	NATIVE_REFLECTION_BEGIN(Test)
		NATIVE_REFLECTION_MEMBER(b)
		NATIVE_REFLECTION_MEMBER(c)
		NATIVE_REFLECTION_MEMBER(i)
		NATIVE_REFLECTION_MEMBER(ui)
		NATIVE_REFLECTION_MEMBER(l)
		NATIVE_REFLECTION_MEMBER(ll)
		NATIVE_REFLECTION_MEMBER(ul)
		NATIVE_REFLECTION_MEMBER(ull)
		NATIVE_REFLECTION_MEMBER(d)
		NATIVE_REFLECTION_MEMBER(f)
		NATIVE_REFLECTION_MEMBER(v2i)
		NATIVE_REFLECTION_MEMBER(v3i)
		NATIVE_REFLECTION_MEMBER(v4i)
		NATIVE_REFLECTION_MEMBER(v2f)
		NATIVE_REFLECTION_MEMBER(v3f)
		NATIVE_REFLECTION_MEMBER(v4f)
		NATIVE_REFLECTION_MEMBER(str)
	NATIVE_REFLECTION_END()

	NATIVE_REFLECTION_BEGIN(SceneObject)
		NATIVE_REFLECTION_MEMBER(m_Name)
		NATIVE_REFLECTION_MEMBER(m_Test)
	NATIVE_REFLECTION_END()
} }