#include "OE/Meta/ReflectionDatabase.hpp"

namespace OrbitEngine { namespace Meta {

	std::vector<NativeType*> ReflectionDatabase::s_NativeTypes;

	ReflectionDatabase::ReflectionDatabase()
	{
	}

	ReflectionDatabase::~ReflectionDatabase()
	{
	}

	void ReflectionDatabase::AddMonoType(MonoType* type)
	{
		m_MonoTypes.push_back(type);
	}

	void ReflectionDatabase::AddNativeType(NativeType* type)
	{
		s_NativeTypes.push_back(type);
	}

	// TODO Optimize these getters

	MonoType* ReflectionDatabase::GetMonoType(const std::string& name)
	{
		for (MonoType* mt : m_MonoTypes)
			if (mt->GetName() == name)
				return mt;
		return nullptr;
	}

	NativeType* ReflectionDatabase::GetNativeType(const std::string& name)
	{
		for (NativeType* nt : s_NativeTypes)
			if (nt->GetName() == name)
				return nt;
		return nullptr;
	}
} }
