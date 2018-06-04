#include "OE/Meta/NativeType.hpp"

namespace OrbitEngine { namespace Meta {

	NativeType::NativeType(const std::string& name, const Kind kind, const unsigned int size)
		: Type(name, kind), m_Size(size)
	{
	}

	NativeType::NativeType(void(*init)(NativeType*)) : Type()
	{
		init(this);
	}

	void NativeType::SetSize(const unsigned int size)
	{
		m_Size = size;
	}
} }
