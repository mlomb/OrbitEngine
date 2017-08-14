#include "OE/System/AssimpIOWrapper.hpp"

#include "OE/System/File.hpp"

namespace OrbitEngine { namespace System { namespace priv {
	
	Assimp::IOStream* AssimpIOSystem::Open(const char* pFile, const char* pMode)
	{
		return new AssimpOIStream(pFile); // TODO? pMode
	}

	void AssimpIOSystem::Close(Assimp::IOStream* pFile)
	{
		delete pFile;
	}

	bool AssimpIOSystem::Exists(const char* pFile) const
	{
		return File::Exists(pFile);
	}

	char AssimpIOSystem::getOsSeparator() const
	{
		return '/';
	}


	AssimpOIStream::AssimpOIStream(const char* pFile)
	{
		m_Stream = File::Open(std::string(pFile));
	}

	AssimpOIStream::~AssimpOIStream()
	{
		delete m_Stream;
	}

	size_t AssimpOIStream::Read(void* pvBuffer, size_t pSize, size_t pCount)
	{
		return m_Stream->read(pvBuffer, pSize, pCount);
	}

	size_t AssimpOIStream::Write(const void * pvBuffer, size_t pSize, size_t pCount)
	{
		return m_Stream->write(pvBuffer, pSize, pCount);
	}

	aiReturn AssimpOIStream::Seek(size_t pOffset, aiOrigin pOrigin)
	{
		return m_Stream->seek(pOffset, pOrigin) ? AI_SUCCESS : AI_FAILURE;
	}

	size_t AssimpOIStream::Tell() const
	{
		return m_Stream->tell();
	}

	size_t AssimpOIStream::FileSize() const
	{
		return m_Stream->getFilesize();
	}

	void AssimpOIStream::Flush()
	{
		return m_Stream->flush();
	}
} } }
