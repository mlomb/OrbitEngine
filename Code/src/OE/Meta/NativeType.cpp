#include "OE/Meta/NativeType.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Meta {

	NativeType::NativeType(const std::string& name, const Kind kind, const unsigned int size)
		: Type(name, kind), m_Size(size), m_Constructor(0), m_Destructor(0)
	{
	}

	NativeType::NativeType(void(*init)(NativeType*))
		: Type(), m_Constructor(0), m_Destructor(0)
	{
		init(this);
	}

	void NativeType::SetSize(const unsigned int size)
	{
		m_Size = size;
	}

	void NativeType::SetCtorAndDctor(Func ctor, Func dctor)
	{
		m_Constructor = ctor;
		m_Destructor = dctor;
	}

	unsigned int NativeType::GetSize() const
	{
		return m_Size;
	}

	void NativeType::Construct(void* object)
	{
		if (m_Constructor == 0) {
			OE_LOG_WARNING("Destructor not available for type " << GetName());
			return;
		}

		m_Constructor(object);
	}

	void NativeType::Destruct(void* object)
	{
		if (m_Destructor == 0) {
			OE_LOG_WARNING("Destructor not available for type " << GetName());
			return;
		}

		m_Destructor(object);
	}
} }
