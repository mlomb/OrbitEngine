#include "OE/UI/Components/Scrollbar.hpp"

namespace OrbitEngine { namespace UI {
	Scrollbar::Scrollbar()
	{
		m_LowButton = new Button();
		m_Slider = new Slider();
		m_HighButton = new Button();

		addElement(m_LowButton);
		addElement(m_Slider);
		addElement(m_HighButton);
	}

	Scrollbar::~Scrollbar()
	{
		removeElement(m_LowButton);
		removeElement(m_Slider);
		removeElement(m_HighButton);

		delete m_LowButton;
		delete m_Slider;
		delete m_HighButton;
	}
} }
