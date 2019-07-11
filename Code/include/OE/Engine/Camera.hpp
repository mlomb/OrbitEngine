#ifndef ENGINE_CAMERA_HPP
#define ENGINE_CAMERA_HPP

#include "OE/Config.hpp"

#include "OE/Engine/Component.hpp"
#include "OE/Engine/Texture.hpp"
#include "OE/Memory/Ptrs.hpp"
#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec3.hpp"
#include "OE/Math/Vec4.hpp"

#include "Transform.hpp"
#include "OE/Graphics/API/FrameBuffer.hpp"

namespace OrbitEngine {	namespace Engine {

	class Camera : public Component {
		NATIVE_REFLECTION

	public:
		enum Projection {
			ORTHOGRAPHIC,
			PERSPECTIVE
		};

		void update(float delta) override;

		Graphics::FrameBuffer* Render();


		Camera();
		~Camera();

		void SetResolution(const Math::Vec2i& res);

		Math::Mat4 getProjectionMatrix();
		Math::Mat4 getViewMatrix();
		Math::Mat4 getVPMatrix();

	private:
		Math::Vec2i m_Resolution;
		Math::Vec4f m_ClearColor;
		Graphics::FrameBuffer* m_FrameBuffer;

		Projection m_Projection;

		Math::Mat4 m_ProjectionMatrix;
		Math::Mat4 m_ViewMatrix;
		Math::Mat4 m_ViewProjectionMatrix;
	};
} }

#endif