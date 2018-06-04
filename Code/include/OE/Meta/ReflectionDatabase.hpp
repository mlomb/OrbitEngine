#ifndef META_REFLECTION_DATABASE_HPP
#define META_REFLECTION_DATABASE_HPP

#include <vector>

#include "OE/Meta/NativeType.hpp"
#include "OE/Meta/MonoType.hpp"

namespace OrbitEngine { namespace Meta {
	class ReflectionDatabase {
	public:
		ReflectionDatabase();
		~ReflectionDatabase();

		void AddMonoType(MonoType* type);
		static void AddNativeType(NativeType* type);

		MonoType* GetMonoType(const std::string& name);
		static NativeType* GetNativeType(const std::string& name);

	private:
		std::vector<MonoType*> m_MonoTypes;
		static std::vector<NativeType*> s_NativeTypes;
	};
} }

#endif