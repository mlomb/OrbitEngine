#include <vector>
#include <fstream>

#include "OE/Config.hpp"

#if OE_ANDROID
	#include "OE/Platform/Android/SystemAndroid.hpp"
#endif

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace System {

	// TODO Optimize this, its slow as fuck
	std::vector<char> LoadFile(const std::string& path, bool binary = false) {
		OE_LOG_DEBUG("Loading file: " << path);

		std::vector<char> buffer;
		bool success = false;

#if OE_ANDROID
		AAssetManager* assetManager = System::priv::SystemAndroid::GetAndroidApp()->activity->assetManager;

		AAsset* file = AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_BUFFER);
		if (file) {
			success = true;

			size_t fileLength = AAsset_getLength(file);
			buffer = std::vector<char>(fileLength + 1);
			AAsset_read(file, buffer.data(), fileLength);
			buffer[fileLength] = '\0';
			AAsset_close(file);
		}
#else
		std::ifstream input(path, binary ? std::ios::binary : std::ios::app);
		if (input.is_open()) {
			success = true;

			buffer = std::vector<char>((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
			input.close();
		}
#endif

		if(!success){
			OE_LOG_ERROR("Couldn't load file: (binary=" << binary << ") " << path);
		}

		return buffer;
	}
} }