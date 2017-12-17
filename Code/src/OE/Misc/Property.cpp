#include "OE/Misc/Property.hpp"

namespace OrbitEngine {	namespace Misc {
	void PropertyBase::fireModification()
	{
		Application::Event e;
		onModification.FireEvent(e);
	}
} }