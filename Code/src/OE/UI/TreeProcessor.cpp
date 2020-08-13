#include "OE/UI/TreeProcessor.hpp"

namespace OrbitEngine { namespace UI {

	TreeProcessor::TreeProcessor(Surface* surface)
		: m_Surface(surface)
	{
	}

	TreeProcessor::~TreeProcessor()
	{
	}

	void TreeProcessor::onVersionChange(Element* element, VersionChangeType changes)
	{
	}

} }
