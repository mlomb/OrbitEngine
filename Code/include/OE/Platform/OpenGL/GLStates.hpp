#ifndef GRAPHICS_GLSTATES_HPP
#define GRAPHICS_GLSTATES_HPP

#include <unordered_map>

#include "OE/Graphics/API/States.hpp"
#include "OE/Platform/OpenGL/OpenGL.hpp"

namespace OrbitEngine { namespace Graphics {
	class GLStates : public States {
	public:
		void setBlending(BlendState enabled) override;
		void setCullMode(CullMode cullMode) override;
		void setDepthTest(FunctionMode depthMode) override;
		void setStencil(FunctionMode stencilMode, StencilOperation operation) override;
		void setScissor(Math::Scissor* scissor) override;

		void bindTexture(unsigned int slot, GLenum target, GLuint id);
		
		void GLEnableDisable(GLenum type, GLboolean enabled);
		void GLEnableDisableColorMask(GLboolean enabled);

		static GLenum CullModeToGL(CullMode cullMode);
		static GLenum FunctionModeToGL(FunctionMode functionMode);

	private:
		std::unordered_map<GLenum, GLuint> m_GLCache;

		// returns true if the cache hits
		bool cache(GLenum key, GLuint value);
	};
} }

#endif