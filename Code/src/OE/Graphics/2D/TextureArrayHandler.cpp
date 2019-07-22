#include "OE/Graphics/2D/TextureArrayHandler.hpp"

#include "OE/Application/Context.hpp"
#include "OE/Graphics/API/Texture.hpp"
#include "OE/Misc/Log.hpp"
#if OE_OPENGL_ANY
#include "OE/Platform/OpenGL/GLShader.hpp"
#endif

namespace OrbitEngine { namespace Graphics {

	void TextureArrayHandler::init(Shader* shader)
	{
#if OE_OPENGL_ANY
		if (Application::Context::GetCurrentAPI() == OPENGL
#if OE_OPENGL_ES
			|| Application::Context::GetCurrentAPI() == OPENGL_ES
#endif
			) {
			Graphics::GLShader* glShader = (Graphics::GLShader*)shader;
			glShader->bind();

			GLint texIDs[OE_2D_MAX_TEXTURES];
			for (int i = 0; i < OE_2D_MAX_TEXTURES; i++) {
				texIDs[i] = i;
			}
			glShader->setUniform1iv("textures", texIDs, (sizeof(texIDs) / sizeof(*texIDs)));

			glShader->unbind();
		}
#endif
	}

	void TextureArrayHandler::begin()
	{
		p_Textures.clear();
	}

	void TextureArrayHandler::end()
	{
		for (unsigned int i = 0; i < p_Textures.size(); i++)
			p_Textures[i]->bind(i);
#if OE_OPENGL_ES
		if (Application::Context::GetCurrentAPI() == OPENGL_ES) {
			if (p_Textures.size() > 0) {
				for (unsigned int i = p_Textures.size(); i < OE_2D_MAX_TEXTURES; i++) {
					// TODO: We should bind a dummy texture
					p_Textures[0]->bind(i);
				}
			}
		}
#endif
	}

	int TextureArrayHandler::handleTexture(Texture* tex)
	{
		if (tex == NULL)
			return 0;

		for (size_t i = 0; i < p_Textures.size(); i++)
			if (p_Textures[i] == tex)
				return i + 1;

		if (p_Textures.size() >= OE_2D_MAX_TEXTURES) {
			OE_LOG_WARNING("Renderer2D: maximum of binded textured exceeded");
			return 0;
		}

		p_Textures.push_back(tex);

		return p_Textures.size();
	}
} }