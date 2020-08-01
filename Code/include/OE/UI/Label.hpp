#ifndef UI_LABEL_HPP
#define UI_LABEL_HPP

#include <string>

#include "OE/UI/Element.hpp"

namespace OrbitEngine { namespace UI {
	/// Text UI element
	class Label : public Element {
	public:
		Label();
		virtual ~Label();

		void setText(std::string& text);

	private:
		std::string m_Text;

		Math::Vec2f measureContent(const Math::Vec2f& minSize, const Math::Vec2f& maxSize) override;
	};
} }

#endif