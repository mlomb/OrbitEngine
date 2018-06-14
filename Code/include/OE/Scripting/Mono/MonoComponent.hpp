#ifndef SCRIPTING_MONO_COMPONENT_HPP
#define SCRIPTING_MONO_COMPONENT_HPP

#include "OE/Engine/Component.hpp"

namespace OrbitEngine {	namespace Engine {

	class MonoComponent : public Component {
	public:
		MonoComponent();
		virtual ~MonoComponent();
	};
} }

#endif