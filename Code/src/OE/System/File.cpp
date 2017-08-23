#include <vector>
#include <fstream>

#include "OE/Config.hpp"

#if OE_ANDROID
	#include "OE/Platform/Android/SystemAndroid.hpp"
#endif

#include "OE/Misc/Log.hpp"
#include "OE/System/File.hpp"

namespace OrbitEngine { namespace System {
	std::string File::LoadFileAsString(const std::string& path)
	{
		std::vector<char> data = File::LoadFile(path);
		return std::string(data.begin(), data.end());
	}

	std::vector<char> File::LoadFile(const std::string& path) {
		std::vector<char> buffer;

		IOStream* stream = Open(path);

		size_t fileLength = stream->getFilesize();
		buffer.resize(fileLength);
		stream->read(buffer.data(), sizeof(char), fileLength);

		Close(stream);

		return buffer;
	}

	IOStream* File::Open(const std::string& path)
	{
		return new IOStream(path, AccessMode::READ);
	}

	void File::Close(IOStream* stream)
	{
		delete stream;
	}

	bool File::Exists(const std::string path)
	{
		IOStream* stream = Open(path);

		bool open = false;
		if (stream->isOpen())
			open = true;

		Close(stream);
		return open;
	}
} }