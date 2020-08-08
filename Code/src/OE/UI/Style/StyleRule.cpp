#include "OE/UI/Style/StyleRule.hpp"

namespace OrbitEngine { namespace UI {

	constexpr unsigned int str2int(const char* str, int h = 0)
	{
		return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
	}

	StyleProperty ResolveProperty(const std::string& name, const std::string& value)
	{
		// note: this function expects name to be lowercase

		using ID = StylePropertyID;

		StyleProperty prop = { ID::LAST_PROPERTY_INVALID };

		switch (str2int(name.c_str())) {
		case str2int("height"):
			prop.id = ID::HEIGHT;
			break;
		default:
			break;
		}

		return prop;
	}
} }