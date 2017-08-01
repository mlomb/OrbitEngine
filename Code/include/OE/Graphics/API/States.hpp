#ifndef GRAPHICS_STATES_HPP
#define GRAPHICS_STATES_HPP

#include "OE/Math/Scissor.hpp"

namespace OrbitEngine { namespace Graphics {
	enum class BlendState {
		SRC_ALPHA = 0,
		ONE_ALPHA,
		NO_COLOR,
		DISABLED
	};

	enum class CullMode {
		FRONT = 0,
		BACK,
		WIREFRAME,
		NONE
	};

	enum class FunctionMode {
		NEVER = 0,
		LESS,
		EQUAL,
		LESS_EQUAL,
		GREATER,
		NOT_EQUAL,
		GREATER_EQUAL,
		ALWAYS,
		DISABLED
	};

	enum class StencilOperation {
		ZERO = 0,
		KEEP,
		SEPARATE_INCR_DECR
	};

	class States {
	public:
		virtual void setBlending(BlendState enabled) = 0;
		virtual void setCullMode(CullMode cullMode) = 0;
		virtual void setDepthTest(FunctionMode depthMode) = 0;
		virtual void setStencil(FunctionMode stencilMode, StencilOperation operation = (StencilOperation)0) = 0;
		virtual void setScissor(Math::Scissor* scissor) = 0;
	};
} }

#endif