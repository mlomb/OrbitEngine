#include "OE/UI/Components/Slider.hpp"


namespace OrbitEngine { namespace UI { 

	Slider::Slider()
	{
		setTag("Slider");

		m_Track = new Element();
		m_Track->addClass("tracker");
		m_Thumb = new Draggable();
		m_Thumb->addClass("thumb");

		addElement(m_Track);
		addElement(m_Thumb);
	}

	Slider::~Slider()
	{
		removeElement(m_Track);
		removeElement(m_Thumb);
		delete m_Track;
		delete m_Thumb;
	}

} }
