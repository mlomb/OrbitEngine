#ifndef MISC_SCISSOR_STACK_HPP
#define MISC_SCISSOR_STACK_HPP

#include "OE/Misc/BaseStack.hpp"
#include "OE/Math/Scissor.hpp"

namespace OrbitEngine { namespace Misc {
	class ScissorStack : public BaseStack<Math::Scissor> {
	protected:
		Math::Scissor nextStack(const Math::Scissor& old, const Math::Scissor& added) override {
			return Math::Scissor(
				std::max(old.rect.x, added.rect.x),
				std::max(old.rect.y, added.rect.y),
				std::min(old.rect.z, std::max(0.0f, added.rect.z)), // size >= 0
				std::min(old.rect.w, std::max(0.0f, added.rect.w))
			);
		};
	};
} }

#endif