#include "OE/Meta/MonoMember.hpp"

namespace OrbitEngine { namespace Meta {
	MonoMember::MonoMember(const std::string& name, MonoType* type, ::MonoClassField* field)
		: Member(name, type), m_Field(field)
	{
	}

	Variant MonoMember::Get(void* object) const
	{
		if(!object)
			return Variant();

		void* ptr;
		mono_field_get_value((MonoObject*)object, m_Field, &ptr);

		switch (m_Type->GetKind())
		{
		case Kind::BOOL:
		case Kind::CHAR:
		case Kind::INT:
		case Kind::UINT:
		case Kind::LONG:
		case Kind::LONGLONG:
		case Kind::ULONG:
		case Kind::ULONGLONG:
		case Kind::DOUBLE:
		case Kind::FLOAT:
		case Kind::CLASS:
		{
			union {
				bool b;
				char c;
				int i;
				unsigned int ui;
				long l;
				long long ll;
				unsigned long ul;
				unsigned long long ull;
				double d;
				float f;
				MonoObject* obj;
			} val;
			mono_field_get_value((MonoObject*)object, m_Field, &val);
			return Variant(&val, sizeof(val), m_Type->GetKind());
		}
		case Kind::STRING:
		{
			MonoString* strval;
			mono_field_get_value((MonoObject*)object, m_Field, &strval);
			if (!strval)
				return Variant(Kind::STRING);
			char* p = mono_string_to_utf8(strval);
			Variant v = Variant(std::string(p));
			mono_free(p);
			return v;
		}
		case Kind::INVALID:
		default:
			return Variant();
		}

	}

	void MonoMember::Set(const Variant& value, void* object) const
	{
		if (!object || !value.IsValid())
			return;

		switch (m_Type->GetKind())
		{
		case Kind::BOOL:
		case Kind::CHAR:
		case Kind::INT:
		case Kind::UINT:
		case Kind::LONG:
		case Kind::LONGLONG:
		case Kind::ULONG:
		case Kind::ULONGLONG:
		case Kind::DOUBLE:
		case Kind::FLOAT:
		case Kind::CLASS:
		{
			mono_field_set_value((MonoObject*)object, m_Field, value.GetDataPointer());
			break;
		}
		case Kind::STRING:
		{
			const char* str = value.GetString().c_str();
			auto d = mono_domain_get();
			MonoString* strval = mono_string_new(d, str);
			mono_field_set_value((MonoObject*)object, m_Field, strval);
			break;
		}
		}
	}
} }