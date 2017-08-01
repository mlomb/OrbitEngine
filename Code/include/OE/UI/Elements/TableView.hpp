#ifndef UI_TABLE_VIEW_HPP
#define UI_TABLE_VIEW_HPP

#include <vector>
#include <string>

#include "OE/UI/Elements/Element.hpp"

namespace OrbitEngine { namespace UI {
	class TableView : public Element {
	public:
		TableView();

	private:
		std::vector<std::string> m_Headers[2];
	};
} }

#endif