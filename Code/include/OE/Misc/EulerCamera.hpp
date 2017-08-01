#ifndef MISC_EULER_CAMERA_HPP
#define MISC_EULER_CAMERA_HPP

#include "OE/Misc/Camera.hpp"

#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace Misc {
	class EulerCamera : public Camera {
	public:
		EulerCamera(const Math::Mat4 &projectionMatrix) : Camera(projectionMatrix) {};
		EulerCamera() : Camera() {}

		void update(float direction) override;
	private:
		Math::Vec3f m_DestPosition;
	};
} }

#endif