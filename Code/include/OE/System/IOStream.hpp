#ifndef SYSTEM_IOSTREAM_HPP
#define SYSTEM_IOSTREAM_HPP

#include "OE/Config.hpp"

#if OE_UNIX
	#include <sys/types.h>
	#include <sys/stat.h>
#endif

#if OE_ANDROID
	#include "OE/Platform/Android/Android.hpp"
#endif

#include <string>

namespace OrbitEngine {	namespace System {

#if OE_ANDROID
	typedef AAsset* IOHandle;
#else
	typedef FILE* IOHandle;
#endif

	enum class AccessMode {
		READ,
		WRITE,
		READ_AND_WRITE
	};

	class IOStream {
	public:
		IOStream(const std::string& file, AccessMode access = AccessMode::READ);
		~IOStream();

		bool isOpen();
		size_t getFilesize();
		size_t read(void* buffer, size_t size, size_t count);
		size_t write(const void* buffer, size_t size, size_t count);
		bool seek(size_t offset, int origin);
		size_t tell();
		void flush();

	private:
		IOHandle m_Data;
		std::string m_File;

		size_t m_CachedFilesize;
	};
} }

#endif