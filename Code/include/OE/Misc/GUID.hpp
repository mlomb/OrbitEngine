#ifndef MISC_GUID_HPP
#define MISC_GUID_HPP

// Based on the implementation of
// https://github.com/graeme-hill/crossguid

#include <array>
#include <ostream>

namespace OrbitEngine {	namespace Misc {
	class GUID {
	public:
		GUID();
		GUID(const std::array<unsigned char, 16>& bytes);
		GUID(const unsigned char *bytes);
		GUID(const std::string& guid);

		static GUID New();

		bool operator==(const GUID &other) const;
		bool operator!=(const GUID &other) const;

		std::string str() const;
		operator std::string() const;
		const unsigned char* data() const;

		bool isValid() const;

	private:
		std::array<unsigned char, 16> m_Bytes;

	};

	std::ostream& operator<<(std::ostream &s, const GUID &uuid);
} }

namespace std {
	template<> struct hash<OrbitEngine::Misc::GUID> {
		size_t operator()(const OrbitEngine::Misc::GUID& guid) const noexcept
		{
			const std::uint64_t* p = reinterpret_cast<const std::uint64_t*>(guid.data());
			std::hash<std::uint64_t> hash;
			return hash(p[0]) ^ hash(p[1]);
		}
	};
}

#endif