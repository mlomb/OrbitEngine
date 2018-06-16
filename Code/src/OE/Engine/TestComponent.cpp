#include "OE/Engine/TestComponent.hpp"

namespace OrbitEngine {	namespace Engine {
	TestComponent::TestComponent()
	{
		m_Float = 42.4f;
		m_Double = 24.42;
	}

	TestComponent::~TestComponent()
	{

	}

	NATIVE_REFLECTION_BEGIN(TestComponent)
		NATIVE_REFLECTION_MEMBER(m_Int)
	NATIVE_REFLECTION_END()
} }