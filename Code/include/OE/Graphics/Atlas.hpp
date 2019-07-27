#ifndef GRAPHICS_ATLAS_HPP
#define GRAPHICS_ATLAS_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "OE/Math/UV.hpp"

namespace OrbitEngine { namespace Graphics {
	/// Generic atlas class
	class Atlas {
	public:
		struct Frame {
			Math::UV uvs;
		};

		/*
		// TODO
		struct Animation {
			std::vector<int> frames;
		};
		*/

		int getFrameIndex(const std::string& name) const;

	protected:
		Atlas();
		virtual ~Atlas();

		int addFrame(const std::string& name, Frame frame);

		std::vector<Frame> m_Frames;
		std::unordered_map<std::string, int> m_FramesNames;
	};
} }

#endif