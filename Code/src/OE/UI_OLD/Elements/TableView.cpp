#include "OE/UI/Elements/TableView.hpp"

namespace OrbitEngine { namespace UI {
	TableView::TableView()
	{
		m_Headers[Axis::HORIZONTAL] = std::vector<std::string>();
		m_Headers[Axis::VERTICAL] = std::vector<std::string>();
	}
} }