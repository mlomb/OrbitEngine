#include "OE/Engine/Camera.hpp"

#include "OE/Application/ContextImpl.hpp"
#include "OE/Misc/FrameDebugger.hpp"

namespace OrbitEngine {	namespace Engine {
	Camera::Camera()
	{
		m_Projection = Projection::PERSPECTIVE;
	}

	Camera::~Camera()
	{

	}

	void Camera::SetResolution(const Math::Vec2i& res)
	{
		m_Resolution = res;
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
			Math::Vec3f position = getTransform()->GetPosition();
			Math::Vec3f rotation = getTransform()->GetRotation();

			Math::Vec3f direction = Math::Vec3f(cos(rotation.y) * sin(rotation.x), sin(rotation.y), cos(rotation.y) * cos(rotation.x));
			Math::Vec3f right = Math::Vec3f(sin(rotation.x - PI / 2.0f), 0, cos(rotation.x - PI / 2.0f));
			Math::Vec3f up = Math::Vec3f::Cross(right, direction);
			m_ViewMatrix = Math::Mat4::LookAt(position, position + direction, up);
			// -----------------

			m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
			break;
		}
	}

	Graphics::FrameBuffer* Camera::Render()
	{
		// Update contents of the framebuffer
		if (Misc::FrameDebugger::IsCapturing()) {
			Misc::FrameDebugger::Event e;
			e.name = "Camera.Render";
			Misc::FrameDebugger::RegisterEvent(e, true);

			{
				Misc::FrameDebugger::Event e;
				e.name = "RA";
				Misc::FrameDebugger::RegisterEvent(e);
			}
			{
				Misc::FrameDebugger::Event e;
				e.name = "RB";
				Misc::FrameDebugger::RegisterEvent(e);
			}
			{
				Misc::FrameDebugger::Event e;
				e.name = "RC";
				Misc::FrameDebugger::RegisterEvent(e);
			}
			{
				Misc::FrameDebugger::Event e;
				e.name = "Render Push";
				Misc::FrameDebugger::RegisterEvent(e, true);

				{
					Misc::FrameDebugger::Event e;
					e.name = "RA";
					Misc::FrameDebugger::RegisterEvent(e);
				}

				Misc::FrameDebugger::Pop();
			}
		}

		bool recreate = true;
		if (m_FrameBuffer) {
			if(m_FrameBuffer->getWidth() == m_Resolution.x &&
			   m_FrameBuffer->getHeight() == m_Resolution.y)
				recreate = false;
		}

		if (recreate) {
			if (m_FrameBuffer)
				delete m_FrameBuffer;

			m_FrameBuffer = Graphics::FrameBuffer::Create(m_Resolution.x, m_Resolution.y);

			Graphics::TextureFormatProperties tfp;
			tfp.format = Graphics::TextureFormat::RGBA;
			tfp.dataType = Graphics::TextureDataType::UNSIGNED_BYTE;
			tfp.mipmapping = false;

			m_FrameBuffer->attachColorTexture(tfp);
			m_FrameBuffer->finalize();
		}

		m_FrameBuffer->setClearColor(m_ClearColor);

		Graphics::FrameBuffer::Push(m_FrameBuffer);
		Graphics::FrameBuffer::Prepare();
		m_FrameBuffer->clear();

		/* Render */

		/* Render */

		Graphics::FrameBuffer::Pop();

		Misc::FrameDebugger::Pop();

		return m_FrameBuffer;
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

	NATIVE_REFLECTION_BEGIN(Camera)
		NATIVE_REFLECTION_MEMBER(m_Resolution)
		NATIVE_REFLECTION_MEMBER(m_ClearColor)
	NATIVE_REFLECTION_END()
} }