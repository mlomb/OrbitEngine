#pragma once

#include "OE/Meta/NativeReflection.hpp"

#include "OE/Scripting/Mono/MonoDomain.hpp"
#include "OE/Memory/Ptrs.hpp"

namespace OrbitEngine { namespace Engine {
	class Object {
		NATIVE_REFLECTION

	public:
	protected:
		friend class ::OrbitEngine::MonoDomain;

		::MonoObject* m_Object = 0;
	};
} }