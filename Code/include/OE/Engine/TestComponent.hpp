#ifndef ENGINE_TESTCOMPONENT_HPP
#define ENGINE_TESTCOMPONENT_HPP

#include "OE/Config.hpp"

#include "OE/Engine/Component.hpp"
#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec3.hpp"
#include "OE/Math/Vec4.hpp"

#include "Transform.hpp"

namespace OrbitEngine {	namespace Engine {

	class RecObj2 : public Misc::OEObject {
		OEOBJECT(RecObj2)

	public:
		Misc::Property<int> m_Int;
	};

	class RecObj : public Misc::OEObject {
		OEOBJECT(RecObj)

	public:
		Misc::Property<int> m_Int;
		Misc::Property<RecObj2> m_RecursiveObjectTest;
		Misc::Property<int> m_Int2;
		Misc::Property<RecObj2> m_RecursiveObjectTest2;
	};

	class REFLECT TestComponent : public Component {
		OEOBJECT(TestComponent)

	public:
		TestComponent();
		~TestComponent();

	private:
		Misc::Property<std::string> m_String;
		Misc::Property<bool> m_Bool;
		Misc::Property<char> m_Char;
		Misc::Property<short> m_Short;
		Misc::Property<int> m_Int;
		Misc::Property<long> m_Long;
		Misc::Property<long long> m_LongLong;
		Misc::Property<RecObj> m_RecursiveOEObj;
		Misc::Property<unsigned char> m_UChar;
		Misc::Property<unsigned short> m_UShort;
		Misc::Property<unsigned int> m_UInt;
		Misc::Property<unsigned long> m_ULong;
		Misc::Property<unsigned long long> m_ULongLong;
		Misc::Property<float> m_Float;
		Misc::Property<double> m_Double;
		Misc::Property<Math::Vec2<int>> m_Vec2i;
		Misc::Property<Math::Vec3<int>> m_Vec3i;
		Misc::Property<Math::Vec4<int>> m_Vec4i;
		Misc::Property<Math::Vec2<float>> m_Vec2f;
		Misc::Property<Math::Vec3<float>> m_Vec3f;
		Misc::Property<Math::Vec4<float>> m_Vec4f;
		Misc::Property<Math::Vec2<double>> m_Vec2d;
		Misc::Property<Math::Vec3<double>> m_Vec3d;
		Misc::Property<Math::Vec4<double>> m_Vec4d;
	};
} }

#endif