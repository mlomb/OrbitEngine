#include "OE/Engine/EulerCameraController.hpp"

#include "OE/Application/ContextImpl.hpp"
#include "OE/Application/InputManager.hpp"

namespace OrbitEngine {	namespace Engine {
	EulerCameraController::EulerCameraController()
	{
		m_MovementSpeed = 50;
		m_MouseSpeed = 0.005f;
	}

	EulerCameraController::~EulerCameraController()
	{

	}

	void EulerCameraController::awake()
	{
		m_Camera = static_cast<Camera*>(*(p_SceneObject->getScene()->getObjects<Camera>().first)); // TODO
	}

	void EulerCameraController::update(float delta)
	{
		Application::InputManager* inputManager = Application::priv::ContextImpl::GetCurrent()->getWindowImpl()->getInputManager();
		Transform* cameraTransform = m_Camera->getTransform();

		//OE_LOG_DEBUG("Has focus: " << inputManager->hasFocus() << ", cursorMode: " << inputManager->getCursorMode());


		if (inputManager->hasFocus() && inputManager->getCursorMode() == Application::CursorMode::GRABBED) {
			Math::Vec3f position = cameraTransform->Position;
			Math::Vec3f rotation = cameraTransform->Rotation;
			float movementSpeed = m_MovementSpeed;

			/* Cursor */
			rotation.x = rotation.x + inputManager->getCursorDelta().x * m_MouseSpeed;
			rotation.y = rotation.y + inputManager->getCursorDelta().y * m_MouseSpeed;

			rotation.y = Math::clamp(rotation.y, (float)Math::deg2rad(-89), (float)Math::deg2rad(89));

			Math::Vec3f direction = Math::Vec3f(cos(rotation.y) * sin(rotation.x), sin(rotation.y), cos(rotation.y) * cos(rotation.x));
			Math::Vec3f right = Math::Vec3f(sin(rotation.x - PI / 2.0f), 0, cos(rotation.x - PI / 2.0f));
			Math::Vec3f up = Math::Vec3f::Cross(right, direction);

			/* Keyboard */
			bool key_up = inputManager->isKeyDown(Application::W);
			bool key_down = inputManager->isKeyDown(Application::S);
			bool key_left = inputManager->isKeyDown(Application::A);
			bool key_right = inputManager->isKeyDown(Application::D);
			bool flyUp = inputManager->isKeyDown(Application::SPACEBAR);
			bool flyDown = inputManager->isKeyDown(Application::LSHIFT);

			if (inputManager->isKeyDown(Application::LCONTROL))
				movementSpeed = movementSpeed * 10;

			if (key_up)
				m_DestPosition += direction * delta * movementSpeed;
			if (key_down)
				m_DestPosition -= direction * delta * movementSpeed;
			if (key_right)
				m_DestPosition += right * delta * movementSpeed;
			if (key_left)
				m_DestPosition -= right * delta * movementSpeed;

			position = Math::Vec3f::Lerp(position, m_DestPosition, Math::clamp(8.0f * delta, 0.0f, 1.0f));
			
			cameraTransform->Position = position;
			cameraTransform->Rotation = rotation;

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