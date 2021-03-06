#include "OE/Engine/Component.hpp"

#include "OE/Engine/Transform.hpp"
#include "OE/Engine/SceneObject.hpp"

namespace OrbitEngine {	namespace Engine {
	Transform* Component::getTransform()
	{
		//return p_SceneObject->getComponent<Transform>();
		return 0;
	}

	bool Component::isAwaken()
	{
		return m_Awake;
	}

	void Component::setAwaken(bool awake)
	{
		m_Awake = awake;
	}

	Component::Component()
		: m_Awake(false)
	{

	}

	Component::~Component()
	{

	}

	WeakPtr<SceneObject> Component::GetSceneObject() const
	{
		return m_SceneObject;
	}

	NATIVE_REFLECTION_BEGIN(Component)

	NATIVE_REFLECTION_END()
} }