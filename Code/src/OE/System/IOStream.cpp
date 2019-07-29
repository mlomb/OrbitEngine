#include "OE/System/IOStream.hpp"

#if OE_ANDROID
	#include "OE/Platform/Android/SystemAndroid.hpp"
#endif

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace System {

	IOStream::IOStream(const std::string& file, AccessMode access)
		: m_File(file),
		m_CachedFilesize(SIZE_MAX),
		m_Data(0)
	{
		OE_LOG_DEBUG("Opening file: " << file);

#if OE_ANDROID
		if (access == AccessMode::READ) {
			AAssetManager* assetManager = System::priv::SystemAndroid::GetAndroidApp()->activity->assetManager;
			m_Data = AAssetManager_open(assetManager, file.c_str(), AASSET_MODE_UNKNOWN);
		}
		else
			OE_LOG_ERROR("Can't write a file in Android");
#else
		char* mode;
		switch (access) {
		case AccessMode::READ: mode = "rb"; break;
		case AccessMode::WRITE: mode = "w+b"; break;
		case AccessMode::READ_AND_WRITE: mode = "w+b"; break;
		}
		m_Data = ::fopen(file.c_str(), mode);
#endif

		if (!isOpen())
			OE_LOG_ERROR("Couldn't open file: " << file);
	}

	IOStream::~IOStream() {
		if (isOpen()) {
			OE_LOG_DEBUG("Closing file: " << m_File);
#if OE_ANDROID
			AAsset_close(m_Data);
#else
			::fclose(m_Data);
#endif
			m_Data = nullptr;
		}
	}

	size_t IOStream::getFilesize()
	{
		if (!isOpen())
			return 0;

		if (m_CachedFilesize == SIZE_MAX) {
#if OE_ANDROID
			m_CachedFilesize = AAsset_getLength(m_Data);
#else
			struct stat fileStat;
			int err = stat(m_File.c_str(), &fileStat);
			if (err != 0)
				return 0;
			m_CachedFilesize = (size_t)(fileStat.st_size);
#endif
		}

		return m_CachedFilesize;
	}

	size_t IOStream::read(void* buffer, size_t size, size_t count)
	{
		if (!isOpen())
			return 0;

#if OE_ANDROID
		int n = AAsset_read(m_Data, buffer, size * count);
		return n / size;
#else
		return ::fread(buffer, size, count, m_Data);
#endif
	}

	size_t IOStream::write(const void* buffer, size_t size, size_t count)
	{
		if (!isOpen())
			return 0;

#if OE_ANDROID
		return 0;
#else
		return ::fwrite(buffer, size, count, m_Data);
#endif
	}

	bool IOStream::seek(size_t offset, int origin)
	{
		if (!isOpen())
			return false;

#if OE_ANDROID
		return AAsset_seek(m_Data, offset, origin) != -1;
#else
		return ::fseek(m_Data, (long)offset, origin) == 0;
#endif
	}

	size_t IOStream::tell()
	{
		if (!isOpen())
			return 0;
		
#if OE_ANDROID
		return AAsset_getLength(m_Data) - AAsset_getRemainingLength(m_Data);
#else
		return ::ftell(m_Data);
#endif
	}

	void IOStream::flush()
	{
		if (!isOpen())
			return;

#if OE_ANDROID
		// -
#else
		::fflush(m_Data);
#endif
	}

	bool IOStream::isOpen()
	{
		return m_Data != 0;
	}
} }