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

	/**
		@brief Generic 2D atlas class

		For examples see BitmapAtlas and TextureAtlas

		Metadata format:
		@code{json}
		{
			"frames": [
				{
					"i": 1, // frame index
					"x": 0,
					"y": 0,
					"w": 64,
					"h": 64,
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
	class Atlas {
	public:
		/// Holds the information to locate a frame
		struct Frame {
			int x, y, w, h;
			bool flipped;
			Math::UV uvs;
		};

		/// Returns whether the provided index already exists
		bool hasFrame(FrameIndex index) const;

		/// Returns texel sizes in both dimensions (1.0f / size)
		virtual Math::Vec2f getTexelSize() const = 0;

	protected:
		Atlas();
		virtual ~Atlas();

		/// Generates JSON metadata
		void writeMetadata(Misc::JSONWriter& writer);
		/// Load JSON metadata
		void loadMetadata(const rapidjson::Value& data);

		/// Exports the metadata to the disk
		bool exportToFile(const std::string& metadata);

		/**
			@brief Add a frame's metadata to the atlas
			@param[in] index identifier within the atlas, must be unique
			@param[in] x,y location of the frame
			@param[in] w,h sizes of the frame
			@param[in] flipped if the provided frame is rotated 90 degrees clockwise
			@return Whether the operation was successful
			@note The operation may fail if the index provided was already in use
		*/
		bool addFrame(FrameIndex index, int x, int y, int w, int h, bool flipped);

	protected:
		/**
			@brief Get a constant reference of a frame
			@param[in] index requested frame index
			@warning You must check if the frame exists (\ref hasFrame) before calling this function
		*/
		const Frame& getFrame(FrameIndex index) const;

	private:
		std::unordered_map<FrameIndex, Frame> m_Frames;
	};
} }

#endif