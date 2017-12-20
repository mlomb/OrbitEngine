#ifndef ENGINE_TRANSFORM_HPP
#define ENGINE_TRANSFORM_HPP

#include "OE/Config.hpp"

#include "OE/Engine/Component.hpp"
#include "OE/Math/Vec3.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine {	namespace Engine {

	class REFLECT Transform : public Component {
		OEOBJECT(Transform)

	public:
		Transform();
		~Transform();

		Misc::Property<Math::Vec3f> Position;
		Misc::Property<Math::Vec3f> Rotation;
		Misc::Property<Math::Vec3f> Scale;

		Math::Mat4 getMatrix();

	private:
		Math::Mat4 m_Matrix;
	};
} }

#endif