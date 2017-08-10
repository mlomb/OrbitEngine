#ifndef SYSTEM_FILE_HPP
#define SYSTEM_FILE_HPP

namespace OrbitEngine { namespace System {
	std::string LoadFileAsString(const std::string& path);
	std::vector<char> LoadFile(const std::string& path, bool binary = false);
} }

#endif