#ifndef ENGINE_COMPONENT_HPP
#define ENGINE_COMPONENT_HPP

#include "OE/Config.hpp"

#include "OE/Meta/NativeReflection.hpp"

namespace OrbitEngine {	namespace Engine {
	class Transform;
	class SceneObject;

	class Component {
		NATIVE_REFLECTION

	public:
		virtual ~Component();

		virtual void awake() {};
		virtual void update(float delta) {};
		virtual void fixedUpdate() {};

		Transform* getTransform();
		bool isAwaken();
		void setAwaken(bool awake);

	protected:
		friend class SceneObject;

		Component();

		SceneObject* p_SceneObject;
		bool m_Awake;
	};
} }

#endif