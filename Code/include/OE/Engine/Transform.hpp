#ifndef ENGINE_TRANSFORM_HPP
#define ENGINE_TRANSFORM_HPP

#include "OE/Config.hpp"

#include "OE/Engine/Component.hpp"
#include "OE/Math/Vec3.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine {	namespace Engine {

	class Transform : public Component {
	public:
		Transform();
		~Transform();

		Math::Vec3f Position;
		Math::Vec3f Rotation;
		Math::Vec3f Scale;

		Math::Mat4 getMatrix();

	private:
		Math::Mat4 m_Matrix;
	};
} }

#endif