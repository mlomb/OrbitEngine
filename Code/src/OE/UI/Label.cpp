#include "OE/UI/Label.hpp"

#include <OE/Misc/Log.hpp>

namespace OrbitEngine { namespace UI {
	Label::Label()
		: Element()
	{
		enableMeasurement();
		setAsTextType();
	}

	Label::~Label()
	{
	}

	void Label::setText(std::string& text)
	{
		m_Text = text;
		markAsDirty(); // TODO: check
	}

	Math::Vec2f Label::measureContent(const Math::Vec2f& minSize, const Math::Vec2f& maxSize)
	{
		// TODO: do this right
		// See https://github.com/facebook/yoga/pull/576/files

		OE_LOG_DEBUG(minSize.x << ", " << minSize.y << " -- " << maxSize.x << ", " << maxSize.y);
		return Math::Vec2f(maxSize.x / 2.0, 60);
	}
} }
