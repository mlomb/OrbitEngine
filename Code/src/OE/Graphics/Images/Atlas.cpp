#include "OE/Graphics/Atlas.hpp"

namespace OrbitEngine { namespace Graphics {
	Atlas::Atlas()
	{
	}

	Atlas::~Atlas()
	{
	}

	int Atlas::getFrameIndex(const std::string& name) const
	{
		auto& it = m_FramesNames.find(name);
		if (it == m_FramesNames.end())
			return -1;
		return (*it).second;
	}

	int Atlas::addFrame(const std::string& name, Frame frame)
	{
		if (getFrameIndex(name) < 0) {
			int index = m_Frames.size();
			m_Frames.push_back(frame);
			m_FramesNames.insert(make_pair(name, index));
			return index;
		}
		return -1;
	}
} }