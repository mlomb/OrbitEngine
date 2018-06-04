#ifndef META_NATIVE_TYPE_HPP
#define META_NATIVE_TYPE_HPP

#include "Type.hpp"

namespace OrbitEngine { namespace Meta {

	class NativeType : public Type {
	public:
		NativeType(const std::string& name, const Kind kind, const unsigned int size);
		NativeType(void(*init)(NativeType*));

		void SetSize(const unsigned int size);

	private:
		unsigned int m_Size;
	};

} }

#endif