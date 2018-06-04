#ifndef GRAPHICS_GLSTATES_HPP
#define GRAPHICS_GLSTATES_HPP

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
		
		void GLEnableDisable(GLenum type, bool enabled);
		void GLEnableDisableColorMask(bool enabled);

		static GLenum CullModeToGL(CullMode cullMode);
		static GLenum FunctionModeToGL(FunctionMode functionMode);
	};
} }

#endif