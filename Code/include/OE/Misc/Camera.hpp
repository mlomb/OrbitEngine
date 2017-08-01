#ifndef MISC_CAMERA_HPP
#define MISC_CAMERA_HPP

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec3.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace Misc {
	class Camera {
	public:
		Camera(const Math::Mat4 &projectionMatrix = Math::Mat4::Identity()) : m_ProjectionMatrix(projectionMatrix), m_ViewMatrix(Math::Mat4::Identity()), m_Position(Math::Vec3f()), m_Rotation(Math::Vec2f()) {};

		virtual void update(float direction) {};

		inline void setProjectionMatrix(const Math::Mat4 &projection) { m_ProjectionMatrix = projection; };
		inline void setViewMatrix(const Math::Mat4 &view) { m_ViewMatrix = view; };

		inline Math::Mat4& getProjectionMatrix() { return m_ProjectionMatrix; };
		inline Math::Mat4& getViewMatrix() { return m_ViewMatrix; };

		inline Math::Vec3f& getPosition() { return m_Position; }
		inline Math::Vec2f& getRotation() { return m_Rotation; }
	protected:
		Math::Mat4 m_ProjectionMatrix, m_ViewMatrix;
		Math::Vec3f m_Position;
		Math::Vec2f m_Rotation; // TODO Quaternions
	};
} }

#endif