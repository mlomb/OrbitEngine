#include "OE/Scripting/Mono/MonoAssembly.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine {
	MonoAssembly::MonoAssembly(const char* filepath)
	{
		m_Assembly = mono_domain_assembly_open(mono_domain_get(), filepath);
		if (!m_Assembly)
			OE_LOG_FATAL_SHOWBOX_AND_EXIT("Couldn't load Assembly '" << filepath << "'");
		m_Image = mono_assembly_get_image(m_Assembly);
		if (!m_Image)
			OE_LOG_FATAL_SHOWBOX_AND_EXIT("Couldn't get image from assembly '" << filepath << "'");
	}

	MonoImage* MonoAssembly::GetMonoImage()
	{
		return m_Image;
	}

	::MonoAssembly* MonoAssembly::GetAssembly()
	{
		return m_Assembly;
	}

	MonoClass* MonoAssembly::GetClass(const char* nameSpace, const char* className)
	{
		return mono_class_from_name(m_Image, nameSpace, className); ;
	}

	MonoClassField* MonoAssembly::GetField(MonoClass* klass, const char* fieldName)
	{
		if (klass == nullptr)
			return nullptr;
		return mono_class_get_field_from_name(klass, fieldName);
	}

	MonoMethod* MonoAssembly::GetMethod(MonoClass* klass, const char* methodName, int paramCount)
	{
		if (klass == nullptr)
			return nullptr;
		return mono_class_get_method_from_name(klass, methodName, paramCount);
	}
}