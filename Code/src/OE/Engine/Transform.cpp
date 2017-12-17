#include "OE/Engine/Transform.hpp"

namespace OrbitEngine {	namespace Engine {
	Transform::Transform()
	{
		m_Position = Math::Vec3f(1, 2, 3);
	}

	Transform::~Transform()
	{

	}
} }