#include "OE/UI/Style/StyleParse.hpp"

namespace OrbitEngine { namespace UI {

	void ConsumeCSSWhiteSpace(const std::string& input, size_t& pos)
	{
		while (pos < input.size() && std::isspace(input[pos]))
			pos++;
	}

	std::string ParseCSSIdentifier(const std::string& input, size_t& pos)
	{
		std::string result;
		while (pos < input.size() && IsCSSIdent(input[pos])) {
			result.push_back(input[pos]);
			pos++;
		}
		return result;
	}

} }
