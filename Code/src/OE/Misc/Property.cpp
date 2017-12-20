#include "OE/Misc/Property.hpp"

namespace OrbitEngine {	namespace Misc {
	bool PropertyBase::isDirty()
	{
		return m_Dirty;
	}

	void PropertyBase::setDirty(bool dirty)
	{
		m_Dirty = dirty;
	}

	void PropertyBase::fireModification()
	{
		setDirty(true);
		Application::Event e;
		onModification.FireEvent(e);
	}
} }