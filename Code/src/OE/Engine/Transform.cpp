#include "OE/Engine/Transform.hpp"

#include "OE/Engine/SceneObject.hpp"

namespace OrbitEngine {	namespace Engine {
	Transform::Transform()
	{
		float LO = -25;
		float HI = 25;

		float a = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
		float b = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
		float c = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
		Position = Math::Vec3f(a, b, c);


		Scale = Math::Vec3f(1, 1, 1);
	}

	Transform::~Transform()
	{

	}

	Math::Mat4 Transform::getMatrix()
	{
		//if (Position.isDirty() || Rotation.isDirty() || Scale.isDirty()) {
			//Position.setDirty(false);
			//Rotation.setDirty(false);
			//Scale.setDirty(false);

			m_Matrix = Math::Mat4::Identity();
			if (p_SceneObject) {
				//SceneObject* parent = p_SceneObject->getParent();
				//if (parent && parent->getParent() != 0 /* is not the root */)
				//	m_Matrix = parent->getComponent<Transform>()->getMatrix();
			}

			m_Matrix = m_Matrix * Math::Mat4::Translation(Position);

			Math::Vec3f rot = Rotation;
			m_Matrix = m_Matrix * Math::Mat4::Rotation(rot.x, Math::Vec3f(1, 0, 0));
			m_Matrix = m_Matrix * Math::Mat4::Rotation(rot.y, Math::Vec3f(0, 1, 0));
			m_Matrix = m_Matrix * Math::Mat4::Rotation(rot.z, Math::Vec3f(0, 0, 1));
			m_Matrix = m_Matrix * Math::Mat4::Scale(Scale);
		//}

		return m_Matrix;
	}
} }