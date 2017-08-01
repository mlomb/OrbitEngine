#ifndef MISC_MAXTRIX_STACK_HPP
#define MISC_MAXTRIX_STACK_HPP

#include "OE/Misc/BaseStack.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace Misc {
	class MatrixStack : public BaseStack<Math::Mat4> {
	protected:
		Math::Mat4 nextStack(const Math::Mat4& old, const Math::Mat4& added) override {
			return old * added;
		};
	};
} }

#endif