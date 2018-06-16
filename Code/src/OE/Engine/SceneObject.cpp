#include "OE/Engine/SceneObject.hpp"

#include "OE/Engine/MemoryDomain.hpp"
#include "OE/Engine/Transform.hpp"
#include "OE/Engine/TestComponent.hpp"

#include "OE/Math/Math.hpp"

namespace OrbitEngine {	namespace Engine {
	SceneObject::SceneObject()
		: m_Name("SceneObject")
	{
		AddComponent<Transform>();
		AddComponent<TestComponent>();
	}

	SceneObject::~SceneObject()
	{
		m_Childs.clear();
		m_Components.clear();
	}

	WeakPtr<Scene> SceneObject::GetScene() const
	{
		return m_Scene;
	}

	WeakPtr<SceneObject> SceneObject::GetParent() const
	{
		return m_Parent;
	}

	WeakPtr<SceneObject> SceneObject::GetChild(int i) const
	{
		if (i < 0 || i >= m_Childs.size())
			return WeakPtr<SceneObject>();
		return m_Childs[i];
	}

	int SceneObject::GetChildCount() const
	{
		return int(m_Childs.size());
	}

	std::string SceneObject::GetName() const
	{
		return m_Name;
	}

	WeakPtr<SceneObject> SceneObject::AddChildren()
	{
		StrongPtr<SceneObject> sceneObject = MemoryDomain::Get()->Create<SceneObject>();
		if (!sceneObject) {
			OE_LOG_WARNING("Can't allocate more SceneObjects!");
			return WeakPtr<SceneObject>();
		}

		sceneObject->m_Self = sceneObject;
		sceneObject->m_Parent = m_Self;
		sceneObject->m_Scene = m_Scene;

		m_Childs.push_back(sceneObject);

		return sceneObject;
	}

	void SceneObject::SetName(const std::string& name)
	{
		m_Name = name;
	}

	void SceneObject::SetParent(WeakPtr<SceneObject> parent, int position) {
		position = Math::clamp(position, 0, (int)(parent->m_Childs.size()) - 1);

		auto it = std::find_if(m_Parent->m_Childs.begin(), m_Parent->m_Childs.end(), [this](StrongPtr<SceneObject> i) {
			return i.Get() == this;
		});
		OE_ASSERT(it != m_Parent->m_Childs.end());
		StrongPtr<SceneObject> me = *it;

		if (parent == m_Parent) {
			if (position == it - m_Parent->m_Childs.begin()) {
				return;
			}
		}

		m_Parent->m_Childs.erase(it);

		parent->m_Childs.insert(parent->m_Childs.begin() + position, me);
		m_Parent = parent;
	}
	
	NATIVE_REFLECTION_BEGIN(SceneObject)
		NATIVE_REFLECTION_MEMBER(m_Name)
	NATIVE_REFLECTION_END()
} }