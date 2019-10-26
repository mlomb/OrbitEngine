#ifndef MISC_JSON_HPP
#define MISC_JSON_HPP

#undef GetObject
#undef Bool

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

namespace OrbitEngine { namespace Misc {
	using namespace rapidjson;

	typedef Writer<StringBuffer> JSONWriter;

	bool WriteJSON(const std::string& path, StringBuffer& buffer);
	Document ReadJSON(const std::string& path);
} }

#endif