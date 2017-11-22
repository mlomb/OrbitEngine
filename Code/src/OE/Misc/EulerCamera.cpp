#include "OE/Misc/EulerCamera.hpp"

#include "OE/Application/ContextImpl.hpp"
#include "OE/Application/InputManager.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Misc {
	void EulerCamera::update(float delta) {
		Application::InputManager* inputManager = Application::priv::ContextImpl::GetCurrent()->getWindowImpl()->getInputManager();

		//OE_LOG_DEBUG("Has focus: " << inputManager->hasFocus() << ", cursorMode: " << inputManager->getCursorMode());

		if (inputManager->hasFocus() && inputManager->getCursorMode() == Application::CursorMode::GRABBED) {
			float speed = 20;
			float mouseSpeed = 0.005f;
			
			/* Cursor */
			m_Rotation.x = m_Rotation.x + inputManager->getCursorDelta().x * mouseSpeed;
			m_Rotation.y = m_Rotation.y + inputManager->getCursorDelta().y * mouseSpeed;

			m_Rotation.y = Math::clamp(m_Rotation.y, (float)Math::deg2rad(-89), (float)Math::deg2rad(89));

			Math::Vec3f direction = Math::Vec3f(cos(m_Rotation.y) * sin(m_Rotation.x), sin(m_Rotation.y), cos(m_Rotation.y) * cos(m_Rotation.x));
			Math::Vec3f right = Math::Vec3f(sin(m_Rotation.x - PI / 2.0f), 0, cos(m_Rotation.x - PI / 2.0f));
			Math::Vec3f up = Math::Vec3f::Cross(right, direction);

			/* Keyboard */
			bool key_up = inputManager->isKeyDown(Application::W);
			bool key_down = inputManager->isKeyDown(Application::S);
			bool key_left = inputManager->isKeyDown(Application::A);
			bool key_right = inputManager->isKeyDown(Application::D);
			bool flyUp = inputManager->isKeyDown(Application::SPACEBAR);
			bool flyDown = inputManager->isKeyDown(Application::LSHIFT);

			if (inputManager->isKeyDown(Application::LCONTROL))
				speed = speed * 10;

			if (key_up)
				m_DestPosition += direction * delta * speed;
			if (key_down)
				m_DestPosition -= direction * delta * speed;
			if (key_right)
				m_DestPosition += right * delta * speed;
			if (key_left)
				m_DestPosition -= right * delta * speed;

			m_Position = Math::Vec3f::Lerp(m_Position, m_DestPosition, Math::clamp(8.0f * delta, 0.0f, 1.0f));

			// TODO Quaternions
			m_ViewMatrix = Math::Mat4::LookAt(m_Position, m_Position + direction, up);

			if (inputManager->isKeyDown(Application::ESCAPE)) {
				inputManager->requestCursorMode(Application::CursorMode::NORMAL);
				inputManager->setCursor(Application::Cursor::ARROW);
			}
		}
		else if (inputManager->isButtonDown(Application::LEFT)) {
			inputManager->requestCursorMode(Application::CursorMode::GRABBED);
			inputManager->setCursor(Application::Cursor::HIDDEN);
		}
	}
} }