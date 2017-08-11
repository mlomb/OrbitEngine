#ifndef GRAPHICS_SKYBOX_HPP
#define GRAPHICS_SKYBOX_HPP

#include "OE/Misc/Camera.hpp"

#include "OE/Graphics/3D/EnviromentMap.hpp"

namespace OrbitEngine { namespace Graphics {
	class FrameBuffer;

	class Skybox : public EnviromentMap {
	public:
		Skybox(Texture* texture);

		void render(Misc::Camera* camera);
	private:
		Shader* m_Shader;
	};
} }

#endif