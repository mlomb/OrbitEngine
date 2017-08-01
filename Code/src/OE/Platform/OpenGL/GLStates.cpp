#include "OE/Platform/OpenGL/GLStates.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"

namespace OrbitEngine { namespace Graphics {

	void GLStates::setBlending(BlendState blendState)
	{
		GLEnableDisableColorMask(blendState != BlendState::NO_COLOR);

		switch (blendState)
		{
		case BlendState::NO_COLOR:
		case BlendState::DISABLED:
			GLEnableDisable(GL_BLEND, false);
			break;
		default:
			GLEnableDisable(GL_BLEND, true);
			switch (blendState)
			{
			case BlendState::SRC_ALPHA:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case BlendState::ONE_ALPHA:
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				break;
			}
			break;
		}
	}

	void GLStates::setCullMode(CullMode cullMode)
	{
#if !OE_OPENGL_ES
		// Not supported in OpenGLES
		if (cullMode == CullMode::WIREFRAME)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#else
		cullMode = CullMode::NONE;
#endif

		if (cullMode == CullMode::NONE)
			GLEnableDisable(GL_CULL_FACE, false);
		else {
			GLEnableDisable(GL_CULL_FACE, true);

			if (cullMode != CullMode::WIREFRAME)
				glCullFace(CullModeToGL(cullMode));
		}
	}


	void GLStates::setDepthTest(FunctionMode depthMode)
	{
		bool enabled = depthMode != FunctionMode::DISABLED;
		GLEnableDisable(GL_DEPTH_TEST, enabled);
		if (!enabled)
			return;
		glDepthFunc(FunctionModeToGL(depthMode));
	}

	void GLStates::setStencil(FunctionMode stencilMode, StencilOperation operation)
	{
		bool enabled = stencilMode != FunctionMode::DISABLED;
		GLEnableDisable(GL_STENCIL_TEST, enabled);
		if (!enabled)
			return;

		glStencilMask(0xff);
		glStencilFunc(FunctionModeToGL(stencilMode), 0x0, 0xff);

		switch (operation)
		{
		case StencilOperation::ZERO:
			glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
			break;
		case StencilOperation::KEEP:
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			break;
		case StencilOperation::SEPARATE_INCR_DECR:
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);
			break;
		}
	}

	void GLStates::setScissor(Math::Scissor* scissor)
	{
		bool enabled = scissor != nullptr;

		GLEnableDisable(GL_SCISSOR_TEST, enabled);

		// TODO
		//if (enabled)
		//	glScissor((GLint)scissor->position.x, (GLint)(Application::View::GetCurrent()->getHeight() - scissor->size.y - scissor->position.y), (GLsizei)scissor->size.x, (GLsizei)scissor->size.y);
	}

	GLenum GLStates::CullModeToGL(CullMode cullMode)
	{
		switch (cullMode)
		{
		case CullMode::WIREFRAME:
		case CullMode::NONE:
			return GL_FRONT_AND_BACK;
		case CullMode::FRONT:
			return GL_FRONT;
		case CullMode::BACK:
			return GL_BACK;
		}
		// TODO Assert
		return 0;
	}

	GLenum GLStates::FunctionModeToGL(FunctionMode functionMode)
	{
		switch (functionMode)
		{
		case FunctionMode::NEVER:
			return GL_NEVER;
		case FunctionMode::LESS:
			return GL_LESS;
		case FunctionMode::EQUAL:
			return GL_EQUAL;
		case FunctionMode::LESS_EQUAL:
			return GL_LEQUAL;
		case FunctionMode::GREATER:
			return GL_GREATER;
		case FunctionMode::NOT_EQUAL:
			return GL_NOTEQUAL;
		case FunctionMode::GREATER_EQUAL:
			return GL_GEQUAL;
		case FunctionMode::ALWAYS:
			return GL_ALWAYS;
		}
		// TODO Assert
		return 0;
	}

	void GLStates::GLEnableDisable(GLenum type, bool enabled)
	{
		// TODO Cache states

		if (enabled)
			glEnable(type);
		else
			glDisable(type);
	}

	void GLStates::GLEnableDisableColorMask(bool enabled)
	{
		glColorMask(enabled, enabled, enabled, enabled);
	}
} }