#include "OE/Misc/JSON.hpp"

#include "OE/System/File.hpp"

namespace OrbitEngine { namespace Misc {

	bool WriteJSON(const std::string& path, StringBuffer& buffer)
	{
		System::IOStream* stream = new System::IOStream(path, System::AccessMode::WRITE);
		if (!stream->isOpen()) {
			delete stream;
			return false;
		}
		stream->write((unsigned char*)buffer.GetString(), sizeof(unsigned char), buffer.GetSize());
		delete stream;
		return true;
	}

	Document ReadJSON(const std::string& path)
	{
		Document d;
		std::string data = System::File::ReadAsString(path);
		if(data.size() > 0)
			d.Parse(data.c_str());
		return d;
	}
} }
