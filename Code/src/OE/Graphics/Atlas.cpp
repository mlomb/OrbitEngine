#include "OE/Graphics/Atlas.hpp"

#include "OE/System/IOStream.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Graphics {
	Atlas::Atlas()
	{
	}

	Atlas::~Atlas()
	{
	}
	
	void Atlas::writeMetadata(Misc::JSONWriter& writer) const {
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
				if (p.second.meta.anchor.x != 0.5f) { writer.Key("ax"); writer.Double(p.second.meta.anchor.x); }
				if (p.second.meta.anchor.y != 0.5f) { writer.Key("ay"); writer.Double(p.second.meta.anchor.y); }
				if (p.second.meta.pivot.x  != 0.5f) { writer.Key("px"); writer.Double(p.second.meta.pivot.x);  }
				if (p.second.meta.pivot.y  != 0.5f) { writer.Key("py"); writer.Double(p.second.meta.pivot.y);  }
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

	void Atlas::loadMetadata(const rapidjson::Value& data) {
		if (data.HasMember("frames")) {
			const rapidjson::Value& frames = data["frames"];
			if (frames.IsArray()) {
				for(rapidjson::SizeType i = 0; i < frames.Size(); ++i) {
					const rapidjson::Value& frame = frames[i];
					if (frame.IsObject()) {
						if (frame.HasMember("i") &&
							frame.HasMember("x") &&
							frame.HasMember("y") &&
							frame.HasMember("w") &&
							frame.HasMember("h") &&
							frame.HasMember("f") &&

							frame["i"].IsUint() &&
							frame["x"].IsInt() &&
							frame["y"].IsInt() &&
							frame["w"].IsInt() &&
							frame["h"].IsInt() &&
							frame["f"].IsBool()) {

							// meta
							FrameMetadata meta = FrameMetadata();
							if (frame.HasMember("ax") && frame["ax"].IsDouble()) meta.anchor.x = frame["ax"].GetDouble();
							if (frame.HasMember("ay") && frame["ay"].IsDouble()) meta.anchor.y = frame["ay"].GetDouble();
							if (frame.HasMember("px") && frame["px"].IsDouble()) meta.pivot.x = frame["px"].GetDouble();
							if (frame.HasMember("py") && frame["py"].IsDouble()) meta.pivot.y = frame["py"].GetDouble();

							if (!this->addFrame(
								(FrameIndex)frame["i"].GetUint(),
								frame["x"].GetInt(),
								frame["y"].GetInt(),
								frame["w"].GetInt(),
								frame["h"].GetInt(),
								frame["f"].GetBool(),
								meta)) {
								OE_LOG_WARNING("frame couldn't be added");
							}
						} else OE_LOG_WARNING("a frame is malformed");
					} else OE_LOG_WARNING("a frame is not an object");
				} // for
			} else OE_LOG_WARNING("frames is not an array");
		}

		if (data.HasMember("animations")) {
			// TODO...
		}
	}

	bool Atlas::exportToFile(const std::string& metadata) const {
		rapidjson::StringBuffer buffer;
		Misc::JSONWriter writer(buffer);
		writeMetadata(writer);
		return Misc::WriteJSON(metadata, buffer);
	}

	bool Atlas::addFrame(FrameIndex index, int x, int y, int w, int h, bool flipped, FrameMetadata meta) {
		if (hasFrame(index))
			return false;

		Frame frame;
		frame.x = x;
		frame.y = y;
		frame.w = w;
		frame.h = h;
		frame.flipped = flipped;
		frame.meta = meta;

		const Math::Vec2f texelSize = getTexelSize();

		float u0 = texelSize.x * x;
		float v0 = texelSize.y * y;
		float u1 = texelSize.x * (x + w);
		float v1 = texelSize.y * (y + h);

		v0 = 1.0f - v0;
		v1 = 1.0f - v1;

		if (flipped)
			frame.uvs = Math::UV(Math::Vec2f(u1, v0), Math::Vec2f(u0, v0), Math::Vec2f(u0, v1), Math::Vec2f(u1, v1));
		else
			frame.uvs = Math::UV(u0, v0, u1, v1);

		m_Frames.insert(std::make_pair(index, frame));
		return true;
	}

	bool Atlas::hasFrame(FrameIndex index) const {
		return m_Frames.find(index) != m_Frames.end();
	}

	const Atlas::Frame& Atlas::getFrame(FrameIndex index) const {
		const auto& it = m_Frames.find(index);
		if(it == m_Frames.end())
			OE_ASSERT_MSG(false, "index not found in atlas. First use hasFrame to check for existance")
		return (*it).second;
	}
} }