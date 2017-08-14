#ifndef SYSTEM_ASSIMP_IO_WRAPPER_H
#define SYSTEM_ASSIMP_IO_WRAPPER_H

#define ASSIMP_BUILD_BOOST_WORKAROUND
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

#include "OE/System/IOStream.hpp"

namespace OrbitEngine { namespace System { namespace priv {
	class AssimpOIStream : public Assimp::IOStream {
	public:
		AssimpOIStream(const char* pFile);
		~AssimpOIStream();

		size_t Read(void* pvBuffer, size_t pSize, size_t pCount) override;
		size_t Write(const void* pvBuffer, size_t pSize, size_t pCount) override;
		aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override;
		size_t Tell() const override;
		size_t FileSize() const override;
		void Flush() override;

	private:
		OrbitEngine::System::IOStream* m_Stream;
	};

	class AssimpIOSystem : public Assimp::IOSystem {
	public:
		Assimp::IOStream* Open(const char* pFile, const char* pMode = "rb") override;
		void Close(Assimp::IOStream* pFile) override;

		bool Exists(const char* pFile) const override;
		char getOsSeparator() const override;
	};
} } }

#endif