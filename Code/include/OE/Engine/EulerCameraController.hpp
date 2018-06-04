#ifndef ENGINE_EULERCAMERACONTROLLER_HPP
#define ENGINE_EULERCAMERACONTROLLER_HPP

#include "OE/Config.hpp"

#include "OE/Math/Vec3.hpp"
#include "OE/Engine/Camera.hpp"
#include "OE/Engine/Component.hpp"
#include "OE/Engine/SceneObject.hpp"

namespace OrbitEngine {	namespace Engine {

	class EulerCameraController : public Component {
	public:
		EulerCameraController();
		~EulerCameraController();

		void awake() override;
		void update(float delta) override;

	private:
		Camera* m_Camera;
		Math::Vec3f m_DestPosition;

		float m_MovementSpeed;
		float m_MouseSpeed;
	};
} }

#endif