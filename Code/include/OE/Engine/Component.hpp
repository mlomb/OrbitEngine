#ifndef ENGINE_COMPONENT_HPP
#define ENGINE_COMPONENT_HPP

#include "OE/Config.hpp"
#include "OE/Misc/Property.hpp"
#include "OE/Misc/OEObject.hpp"

namespace OrbitEngine {	namespace Engine {

	class REFLECT Component : public Misc::OEObject {
		OEOBJECT(Component)

	public:
		Component();
		~Component();

		Misc::Property<std::string> strTest;
		Misc::Property<int> test_using_a_very_long_name_here;

	private:
	};
} }

#endif