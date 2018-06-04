#ifndef ENGINE_CAMERA_HPP
#define ENGINE_CAMERA_HPP

#include "OE/Config.hpp"

#include "OE/Engine/Component.hpp"
#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec3.hpp"
#include "OE/Math/Vec4.hpp"

#include "Transform.hpp"

namespace OrbitEngine {	namespace Engine {

	class Camera : public Component {
	public:
		enum Projection {
			ORTHOGRAPHIC,
			PERSPECTIVE
		};

		void update(float delta) override;

		Camera();
		~Camera();

		Math::Mat4 getProjectionMatrix();
		Math::Mat4 getViewMatrix();
		Math::Mat4 getVPMatrix();

	private:
		Projection m_Projection;

		Math::Mat4 m_ProjectionMatrix;
		Math::Mat4 m_ViewMatrix;
		Math::Mat4 m_ViewProjectionMatrix;
	};
} }

#endif