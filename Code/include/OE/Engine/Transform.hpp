#ifndef ENGINE_TRANSFORM_HPP
#define ENGINE_TRANSFORM_HPP

#include "OE/Config.hpp"

#include "OE/Engine/Component.hpp"
#include "OE/Math/Vec3.hpp"

namespace OrbitEngine {	namespace Engine {

	class REFLECT Transform : public Component {
		OEOBJECT(Transform)

	public:
		Transform();
		~Transform();

	private:
		Misc::Property<Math::Vec3f> m_Position;
		Misc::Property<Math::Vec3f> m_Rotation;
		Misc::Property<Math::Vec3f> m_Scale;

		Misc::Property<std::string> m_Test;
		Misc::Property<int> m_Counter;
	};
} }

#endif