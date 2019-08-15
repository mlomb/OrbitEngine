#include "OE/Platform/OpenGL/GLStates.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"
#include "OE/Graphics/API/FrameBuffer.hpp"

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

			if (!cache(0x0BE1 /* GL_BLEND_SRC */, (int)blendState)) {
				switch (blendState)
				{
				case BlendState::SRC_ALPHA:
					OE_CHECK_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
					break;
				case BlendState::ONE_ALPHA:
					OE_CHECK_GL(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
					break;
				case BlendState::ONE_ONE:
					OE_CHECK_GL(glBlendFunc(GL_ONE, GL_ONE));
					break;
				case BlendState::ONE_ZERO:
					OE_CHECK_GL(glBlendFunc(GL_ONE, GL_ZERO));
					break;
				}
				break;
			}
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
				OE_CHECK_GL(glCullFace(CullModeToGL(cullMode)));
		}
	}

	void GLStates::setDepthTest(FunctionMode depthMode)
	{
		bool enabled = depthMode != FunctionMode::DISABLED;
		GLEnableDisable(GL_DEPTH_TEST, enabled);
		if (!enabled)
			return;
		OE_CHECK_GL(glDepthFunc(FunctionModeToGL(depthMode)));
	}

	void GLStates::setStencil(FunctionMode stencilMode, StencilOperation operation)
	{
		bool enabled = stencilMode != FunctionMode::DISABLED;
		GLEnableDisable(GL_STENCIL_TEST, enabled);
		if (!enabled)
			return;

		OE_CHECK_GL(glStencilMask(0xff));
		OE_CHECK_GL(glStencilFunc(FunctionModeToGL(stencilMode), 0x0, 0xff));

		switch (operation)
		{
		case StencilOperation::ZERO:
			OE_CHECK_GL(glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO));
			break;
		case StencilOperation::KEEP:
			OE_CHECK_GL(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
			break;
		case StencilOperation::SEPARATE_INCR_DECR:
			OE_CHECK_GL(glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP));
			OE_CHECK_GL(glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP));
			break;
		}
	}

	void GLStates::setScissor(Math::Scissor* scissor)
	{
		bool enabled = scissor != nullptr;

		GLEnableDisable(GL_SCISSOR_TEST, enabled);

		if (enabled) {
			OE_CHECK_GL(glScissor(
				scissor->rect.x,
				FrameBuffer::GetCurrentSize().y - (scissor->rect.w + scissor->rect.y),
				scissor->rect.z,
				scissor->rect.w
			));
		}
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

	bool GLStates::cache(GLenum key, GLuint value)
	{
		auto it = m_GLCache.find(key);
		if (it != m_GLCache.end()) {
			if ((*it).second == value)
				return true; // hit
		}
		m_GLCache[key] = value;
		return false;
	}

	void GLStates::GLEnableDisable(GLenum type, GLboolean enabled)
	{
		if (cache(type, enabled))
			return;

		if (enabled) {
			OE_CHECK_GL(glEnable(type));
		}
		else {
			OE_CHECK_GL(glDisable(type));
		}
	}

	void GLStates::GLEnableDisableColorMask(GLboolean enabled)
	{
		if (cache(0x8E52 /* GL_SAMPLE_MASK_VALUE */, enabled))
			return;

		OE_CHECK_GL(glColorMask(enabled, enabled, enabled, enabled));
	}
} }