#pragma once

#include "OE/Scripting/Mono/Mono.hpp"

namespace OrbitEngine {
	class MonoAssembly {
	public:
		MonoAssembly(const char* filepath);

		::MonoImage* GetMonoImage();
		::MonoAssembly* GetAssembly();

		::MonoClass* GetClass(const char* nameSpace, const char* className);
		::MonoClassField* GetField(MonoClass* klass, const char* fieldName);
		::MonoMethod* GetMethod(MonoClass* klass, const char* methodName, int paramCount);
	private:
		::MonoAssembly* m_Assembly;
		::MonoImage* m_Image;
	};
}
