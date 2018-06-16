#ifndef ENGINE_TESTCOMPONENT_HPP
#define ENGINE_TESTCOMPONENT_HPP

#include "OE/Config.hpp"

#include "OE/Engine/Component.hpp"
#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec3.hpp"
#include "OE/Math/Vec4.hpp"

#include "OE/Meta/NativeReflection.hpp"

#include "Transform.hpp"

namespace OrbitEngine {	namespace Engine {

	class TestComponent : public Component {
		NATIVE_REFLECTION

	public:
		TestComponent();
		~TestComponent();

	private:
		std::string m_String;
		bool m_Bool;
		char m_Char;
		short m_Short;
		int m_Int;
		long m_Long;
		long long m_LongLong;
		unsigned char m_UChar;
		unsigned short m_UShort;
		unsigned int m_UInt;
		unsigned long m_ULong;
		unsigned long long m_ULongLong;
		float m_Float;
		double m_Double;
		Math::Vec2<int> m_Vec2i;
		Math::Vec3<int> m_Vec3i;
		Math::Vec4<int> m_Vec4i;
		Math::Vec2<float> m_Vec2f;
		Math::Vec3<float> m_Vec3f;
		Math::Vec4<float> m_Vec4f;
		Math::Vec2<double> m_Vec2d;
		Math::Vec3<double> m_Vec3d;
		Math::Vec4<double> m_Vec4d;
	};
} }

#endif