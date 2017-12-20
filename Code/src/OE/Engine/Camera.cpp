#include "OE/Engine/Camera.hpp"

#include "OE/Application/ContextImpl.hpp"

namespace OrbitEngine {	namespace Engine {
	Camera::Camera()
	{
		m_Projection = Projection::PERSPECTIVE;
	}

	Camera::~Camera()
	{

	}

	void Camera::update(float delta)
	{
		switch (m_Projection)
		{
		case Projection::ORTHOGRAPHIC:
			break;
		case Projection::PERSPECTIVE:
			Math::Vec2i size = Application::priv::ContextImpl::GetCurrent()->getSize();
			m_ProjectionMatrix = OrbitEngine::Math::Mat4::Perspective(45.0, (float)size.w / (float)size.h, 0.1f, 2000.0f);

			// TODO Quaternions
			Math::Vec3f position = getTransform()->Position;
			Math::Vec3f rotation = getTransform()->Rotation;

			Math::Vec3f direction = Math::Vec3f(cos(rotation.y) * sin(rotation.x), sin(rotation.y), cos(rotation.y) * cos(rotation.x));
			Math::Vec3f right = Math::Vec3f(sin(rotation.x - PI / 2.0f), 0, cos(rotation.x - PI / 2.0f));
			Math::Vec3f up = Math::Vec3f::Cross(right, direction);
			m_ViewMatrix = Math::Mat4::LookAt(position, position + direction, up);
			// -----------------

			m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
			break;
		}
	}

	Math::Mat4 Camera::getProjectionMatrix() {
		return m_ProjectionMatrix;
	}

	Math::Mat4 Camera::getViewMatrix()
	{
		return m_ViewMatrix;
	}

	Math::Mat4 Camera::getVPMatrix()
	{
		return m_ViewProjectionMatrix;
	}
} }