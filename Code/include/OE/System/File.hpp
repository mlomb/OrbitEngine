#ifndef SYSTEM_FILE_HPP
#define SYSTEM_FILE_HPP

namespace OrbitEngine { namespace System {
	std::vector<char> LoadFile(const std::string& path, bool binary = false);
} }

#endif