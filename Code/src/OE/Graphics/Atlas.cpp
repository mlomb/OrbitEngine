#include "OE/Graphics/Atlas.hpp"

namespace OrbitEngine { namespace Graphics {

	Atlas::Atlas()
	{
	}

	Atlas::~Atlas()
	{
	}
	
	void Atlas::writeMetadata(json_writer& writer)
	{
		writer.StartObject();

		// frames
		{
			writer.Key("frames");
			writer.StartArray();

			for (auto& p : m_Frames) {
				writer.StartObject();
				writer.Key("i");
				writer.Uint(p.first);
				writer.Key("x");
				writer.Int(p.second.x);
				writer.Key("y");
				writer.Int(p.second.y);
				writer.Key("w");
				writer.Int(p.second.w);
				writer.Key("h");
				writer.Int(p.second.h);
				writer.Key("f");
				writer.Bool(p.second.flipped);
				writer.EndObject();
			}

			writer.EndArray();
		}

		// animations
		{
			writer.Key("animations");
			writer.StartArray();

			// TODO...

			writer.EndArray();
		}

		writer.EndObject();
	}

	bool Atlas::addFrame(FrameIndex index, int x, int y, int w, int h, bool flipped) {
		if (m_Frames.find(index) == m_Frames.end()) {
			Frame frame;
			frame.x = x;
			frame.y = y;
			frame.w = w;
			frame.h = h;
			frame.flipped = flipped;

			const float texelSize = getTexelSize();

			if (flipped) {
				float u0 = texelSize * x;
				float v0 = texelSize * (y + h);
				float u1 = texelSize * (x + w);
				float v1 = texelSize * y;

				frame.uvs = Math::UV(Math::Vec2f(u0, v1), Math::Vec2f(u1, v1), Math::Vec2f(u1, v0), Math::Vec2f(u0, v0));
			}
			else {
				float u0 = texelSize * x;
				float v0 = texelSize * y;
				float u1 = texelSize * (x + w);
				float v1 = texelSize * (y + h);

				frame.uvs = Math::UV(u0, v0, u1, v1);
			}

			m_Frames.insert(std::make_pair(index, frame));
			return true;
		}
		return false;
	}
} }