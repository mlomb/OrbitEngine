#ifndef GRAPHICS_ATLAS_HPP
#define GRAPHICS_ATLAS_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "OE/Misc/JSON.hpp"

#include "OE/Math/UV.hpp"

namespace OrbitEngine { namespace Graphics {

	typedef unsigned int FrameIndex;

	/// Generic atlas class
	class Atlas {
	public:

		struct Frame {
			int x, y, w, h;
			bool flipped;
			Math::UV uvs;
		};

		/*
		// TODO: Animations
		struct Animation {
			std::vector<int> frames;
		};
		*/

		/**
			@brief Generates JSON metadata describing the atlas

			Format:
			@code{json}
			{
				"frames": [
					{
						"i": 1, // frame index
						"x": 10,
						"y": 10,
						"w": 15,
						"h": 25,
						"f": false // flipped?
					},
					...
				],
				"animations": [
					// TODO...
				]
			}
			@endcode
		*/
		void writeMetadata(json_writer& writer);

		virtual float getTexelSize() = 0;

	protected:
		Atlas();
		virtual ~Atlas();

		bool addFrame(FrameIndex index, int x, int y, int w, int h, bool flipped);

		std::unordered_map<FrameIndex, Frame> m_Frames;
	};
} }

#endif