#ifndef GRAPHICS_ENVIROMENT_MAP_HPP
#define GRAPHICS_ENVIROMENT_MAP_HPP

#include "OE/Graphics/API/FrameBuffer.hpp"
#include "OE/Graphics/API/Mesh.hpp"
#include "OE/Graphics/API/Texture.hpp"

#include "OE/Math/Vec4.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace Graphics {
	class Shader;

	class EnviromentMap {
	public:
		EnviromentMap(Graphics::Texture* texture);

		void precomputeEnvPBR();

		inline Texture* getEnviromentMap() const { return m_EnviromentMap; };
		inline Texture* getPrefilteredEnviromentMap() const { return m_PrefilteredEnvMap ? m_PrefilteredEnvMap->getColorTextures()[0] : 0; };
	protected:
		static Mesh* s_CaptureCube;
		static Shader* s_PrefilterCapture;
		static Shader* s_HDRToCubemap;
		static Math::Mat4 s_CubeFaces[6];

		static void setViewMatrices(Graphics::Shader* shader);

		Graphics::Texture* m_EnviromentMap;

		// IBL
		// Move this from here
		Graphics::FrameBuffer* m_PrefilteredEnvMap;
	};
} }

#endif