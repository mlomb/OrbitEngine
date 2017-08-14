#ifndef SYSTEM_FILE_HPP
#define SYSTEM_FILE_HPP

#include "OE/System/IOStream.hpp"

namespace OrbitEngine { namespace System {
	class File {
	public:
		static IOStream* Open(const std::string& path);
		static void Close(IOStream* stream);
		static bool Exists(std::string path);

		static std::string LoadFileAsString(const std::string& path);
		static std::vector<char> LoadFile(const std::string& path, bool binary = false);
	private:
	};
} }

#endif