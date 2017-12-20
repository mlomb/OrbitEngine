#ifndef ENGINE_EULERCAMERACONTROLLER_HPP
#define ENGINE_EULERCAMERACONTROLLER_HPP

#include "OE/Config.hpp"

#include "OE/Math/Vec3.hpp"
#include "OE/Engine/Camera.hpp"
#include "OE/Engine/Component.hpp"
#include "OE/Engine/SceneObject.hpp"

namespace OrbitEngine {	namespace Engine {

	class REFLECT EulerCameraController : public Component {
		OEOBJECT(EulerCameraController)

	public:
		EulerCameraController();
		~EulerCameraController();

		void awake() override;
		void update(float delta) override;

	private:
		Camera* m_Camera;
		Math::Vec3f m_DestPosition;

		Misc::Property<float> m_MovementSpeed;
		Misc::Property<float> m_MouseSpeed;
	};
} }

#endif