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

	IOStream* File::Open(const std::string& path, AccessMode accessMode)
	{
		return new IOStream(path, accessMode);
	}

	void File::Close(IOStream* stream)
	{
		delete stream;
	}

	bool File::Exists(const std::string path)
	{
		IOStream* stream = Open(path);
		bool open = stream->isOpen();
		Close(stream);
		return open;
	}
} }