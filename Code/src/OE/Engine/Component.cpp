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
} }