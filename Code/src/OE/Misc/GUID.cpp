#include "OE/Misc/GUID.hpp"

#include <iomanip>
#include <sstream>
#include <algorithm>

#include "OE/Config.hpp"

#if OE_WINDOWS
#include <objbase.h>
#endif

#if OE_UNIX
#include <uuid/uuid.h>
#endif

namespace OrbitEngine {	namespace Misc {

	GUID::GUID()
	{
	}

	GUID::GUID(const std::array<unsigned char, 16>& bytes)
		: m_Bytes(bytes)
	{
	}

	GUID::GUID(const unsigned char* bytes)
	{
		std::copy(bytes, bytes + 16, std::begin(m_Bytes));
	}

	unsigned char hexDigitToChar(char ch)
	{
		// 0-9
		if (ch > 47 && ch < 58)
			return ch - 48;

		// a-f
		if (ch > 96 && ch < 103)
			return ch - 87;

		// A-F
		if (ch > 64 && ch < 71)
			return ch - 55;

		return 0;
	}

	bool isValidHexChar(char ch)
	{
		// 0-9
		if (ch > 47 && ch < 58)
			return true;

		// a-f
		if (ch > 96 && ch < 103)
			return true;

		// A-F
		if (ch > 64 && ch < 71)
			return true;

		return false;
	}

	unsigned char hexPairToChar(char a, char b)
	{
		return hexDigitToChar(a) * 16 + hexDigitToChar(b);
	}

	GUID::GUID(const std::string& guid)
	{
		char charOne = '\0';
		char charTwo = '\0';
		bool lookingForFirstChar = true;
		unsigned nextByte = 0;

		for (const char &ch : guid)
		{
			if (ch == '-')
				continue;

			if (nextByte >= 16 || !isValidHexChar(ch))
				goto fail;

			if (lookingForFirstChar)
			{
				charOne = ch;
				lookingForFirstChar = false;
			}
			else
			{
				charTwo = ch;
				auto byte = hexPairToChar(charOne, charTwo);
				m_Bytes[nextByte++] = byte;
				lookingForFirstChar = true;
			}
		}

		if (nextByte < 16)
			goto fail;
		return;

	fail:
		std::fill(m_Bytes.begin(), m_Bytes.end(), 0);
	}
	
	std::string GUID::str() const
	{
		std::stringstream ss;
		ss  << std::hex << std::setfill('0')
			<< std::setw(2) << (int)m_Bytes[0]
			<< std::setw(2) << (int)m_Bytes[1]
			<< std::setw(2) << (int)m_Bytes[2]
			<< std::setw(2) << (int)m_Bytes[3]
			<< "-"
			<< std::setw(2) << (int)m_Bytes[4]
			<< std::setw(2) << (int)m_Bytes[5]
			<< "-"
			<< std::setw(2) << (int)m_Bytes[6]
			<< std::setw(2) << (int)m_Bytes[7]
			<< "-"
			<< std::setw(2) << (int)m_Bytes[8]
			<< std::setw(2) << (int)m_Bytes[9]
			<< "-"
			<< std::setw(2) << (int)m_Bytes[10]
			<< std::setw(2) << (int)m_Bytes[11]
			<< std::setw(2) << (int)m_Bytes[12]
			<< std::setw(2) << (int)m_Bytes[13]
			<< std::setw(2) << (int)m_Bytes[14]
			<< std::setw(2) << (int)m_Bytes[15];
		return ss.str();
	}

	GUID::operator std::string() const
	{
		return str();
	}

	const unsigned char* GUID::data() const
	{
		return m_Bytes.data();
	}

	bool GUID::isValid() const
	{
		static GUID def;
		return (*this) != def;
	}

	std::ostream& operator<<(std::ostream& s, const GUID& uuid)
	{
		s << uuid.str();
		return s;
	}

	GUID GUID::New()
	{
#if OE_WINDOWS
		::GUID newId;
		CoCreateGuid(&newId);

		std::array<unsigned char, 16> bytes =
		{
			(unsigned char)((newId.Data1 >> 24) & 0xFF),
			(unsigned char)((newId.Data1 >> 16) & 0xFF),
			(unsigned char)((newId.Data1 >> 8) & 0xFF),
			(unsigned char)((newId.Data1) & 0xff),

			(unsigned char)((newId.Data2 >> 8) & 0xFF),
			(unsigned char)((newId.Data2) & 0xff),

			(unsigned char)((newId.Data3 >> 8) & 0xFF),
			(unsigned char)((newId.Data3) & 0xFF),

			(unsigned char)newId.Data4[0],
			(unsigned char)newId.Data4[1],
			(unsigned char)newId.Data4[2],
			(unsigned char)newId.Data4[3],
			(unsigned char)newId.Data4[4],
			(unsigned char)newId.Data4[5],
			(unsigned char)newId.Data4[6],
			(unsigned char)newId.Data4[7]
		};

		return bytes;
#endif
#if OE_UNIX
		uuid_t id;
		uuid_generate(id);
		return id;
#endif

		return GUID();
	}

	bool GUID::operator==(const GUID& other) const
	{
		return m_Bytes == other.m_Bytes;
	}

	bool GUID::operator!=(const GUID& other) const
	{
		return !((*this) == other);
	}
} }
