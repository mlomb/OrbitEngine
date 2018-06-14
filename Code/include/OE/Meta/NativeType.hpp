#ifndef META_NATIVE_TYPE_HPP
#define META_NATIVE_TYPE_HPP

#include "Type.hpp"

namespace OrbitEngine { namespace Meta {

	class NativeType : public Type {
	public:
		typedef void(*Func)(void*);

		NativeType(const std::string& name, const Kind kind, const unsigned int size);
		NativeType(void(*init)(NativeType*));

		void SetSize(const unsigned int size);
		void SetCtorAndDctor(Func ctor, Func dctor);

		unsigned int GetSize() const;

		void Construct(void* object);
		void Destruct(void* object);

	private:
		unsigned int m_Size;
		Func m_Constructor, m_Destructor;
	};

} }

#endif