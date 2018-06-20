#ifndef ENGINE_TRANSFORM_HPP
#define ENGINE_TRANSFORM_HPP

#include "OE/Config.hpp"

#include "OE/Engine/Component.hpp"
#include "OE/Math/Vec3.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine {	namespace Engine {

	class Transform : public Component {
		NATIVE_REFLECTION

	public:
		Transform();
		~Transform();

		Math::Vec3f GetPosition() const;
		Math::Vec3f GetRotation() const;
		Math::Vec3f GetScale() const;
		Math::Mat4 getMatrix();

		void SetPosition(const Math::Vec3f& pos);
		void SetRotation(const Math::Vec3f& rot);
		void SetScale(const Math::Vec3f& scale);

	private:
		Math::Mat4 m_Matrix;

		Math::Vec3f m_Position;
		Math::Vec3f m_Rotation;
		Math::Vec3f m_Scale;
	};
} }

#endif