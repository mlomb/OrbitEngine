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

	std::vector<char> File::LoadFile(const std::string& path, bool binary) {
		std::vector<char> buffer;

		// TODO Replace with IOStream
#if OE_ANDROID
		AAssetManager* assetManager = System::priv::SystemAndroid::GetAndroidApp()->activity->assetManager;

		AAsset* file = AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_BUFFER);
		if (file) {
			size_t fileLength = AAsset_getLength(file);
			buffer = std::vector<char>(fileLength + 1);
			AAsset_read(file, buffer.data(), fileLength);
			buffer[fileLength] = '\0';
			AAsset_close(file);
		}
#else
		std::ifstream input(path, binary ? std::ios::binary : std::ios::app);
		if (input.is_open()) {
			buffer = std::vector<char>((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
			input.close();
		}
#endif
		
		return buffer;
		/*
		std::vector<char> buffer;

		IOStream* stream = Open(path);

		size_t fileLength = stream->getFilesize();
		buffer.resize(fileLength + 1);
		stream->read(buffer.data(), sizeof(char), fileLength);
		buffer[fileLength] = '\0';

		Close(stream);

		return buffer;
		*/
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