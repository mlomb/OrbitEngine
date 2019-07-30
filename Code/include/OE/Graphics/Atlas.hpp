#ifndef GRAPHICS_ATLAS_HPP
#define GRAPHICS_ATLAS_HPP

#include <string>
#include <unordered_map>

#include "OE/Misc/JSON.hpp"
#include "OE/Math/Vec2.hpp"
#include "OE/Math/UV.hpp"

namespace OrbitEngine { namespace Graphics {
	/// Frame identifier within an atlas. It must be unique and does not have to be sequential
	typedef unsigned int FrameIndex;

	/// Generic 2D atlas class
	class Atlas {
	public:
		struct Frame {
			int x, y, w, h;
			bool flipped;
			Math::UV uvs;
		};

		/// Returns texel sizes in both dimensions (1.0f / size)
		virtual Math::Vec2f getTexelSize() const = 0;

	protected:
		Atlas();
		virtual ~Atlas();

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
		void writeMetadata(Misc::JSONWriter& writer);
		/// Load JSON metadata with the format described in writeMetadata
		void loadMetadata(const rapidjson::Value& data);

		bool exportToFile(const std::string& metadata);
		bool addFrame(FrameIndex index, int x, int y, int w, int h, bool flipped);

		std::unordered_map<FrameIndex, Frame> m_Frames;
	};
} }

#endif