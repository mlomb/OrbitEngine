#ifndef META_VARIANT_HPP
#define META_VARIANT_HPP

#include <string.h>

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec3.hpp"
#include "OE/Math/Vec4.hpp"
#include "OE/Meta/Type.hpp"

namespace OrbitEngine { namespace Meta {

	class Variant {
	public:
		~Variant();

		Variant();
		Variant(const Variant& v);
		Variant(const Kind kind);
		Variant(const void* data, const unsigned int size, const Kind kind);
		Variant(void* ptr);
		Variant(const bool b);
		Variant(const char c);
		Variant(const int i);
		Variant(const unsigned int ui);
		Variant(const long l);
		Variant(const long long ll);
		Variant(const unsigned long ul);
		Variant(const unsigned long long ull);
		Variant(const double d);
		Variant(const float f);
		Variant(const std::string& str);
		Variant(const Math::Vec2i& v2i);
		Variant(const Math::Vec3i& v3i);
		Variant(const Math::Vec4i& v4i);
		Variant(const Math::Vec2f& v2f);
		Variant(const Math::Vec3f& v3f);
		Variant(const Math::Vec4f& v4f);

		Variant& operator=(const Variant& v);

		friend bool operator==(const Variant& lhs, const Variant& rhs);
		friend bool operator!=(const Variant& lhs, const Variant& rhs);

		Kind GetKind() const;
		bool IsValid() const;
		bool IsNull() const;

		bool GetBool() const;
		char GetChar() const;
		int GetInt() const;
		unsigned int GetUInt() const;
		long GetLong() const;
		long long GetLongLong() const;
		unsigned long GetULong() const;
		unsigned long long GetULongLong() const;
		double GetDouble() const;
		float GetFloat() const;
		Math::Vec2i GetVec2i() const;
		Math::Vec3i GetVec3i() const;
		Math::Vec4i GetVec4i() const;
		Math::Vec2f GetVec2f() const;
		Math::Vec3f GetVec3f() const;
		Math::Vec4f GetVec4f() const;
		void* GetPointer() const;
		std::string GetString() const;

		void* GetDataPointer() const;

	private:
		Kind m_Kind;
		bool m_Null;

		union Data {
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
			Math::Vec2i v2i;
			Math::Vec3i v3i;
			Math::Vec4i v4i;
			Math::Vec2f v2f;
			Math::Vec3f v3f;
			Math::Vec4f v4f;
			std::string* str;
			void* ptr;

			Data() { memset(this, 0, sizeof(Data)); }
		} m_Data;

		void construct(const void* data, const unsigned int size, const Kind kind);
		void clear();
	};

} }

#endif