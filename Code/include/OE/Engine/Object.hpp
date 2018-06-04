#pragma once

#include "OE/Meta/NativeReflection.hpp"

#include "OE/Scripting/Mono/MonoDomain.hpp"

namespace OrbitEngine { namespace Engine {
	class Object {
		NATIVE_REFLECTION()

	public:
		virtual void test() {};
		int b;
		int a;
	protected:
		friend class ::OrbitEngine::MonoDomain;

		::MonoObject* m_Object = 0;
	};
} }