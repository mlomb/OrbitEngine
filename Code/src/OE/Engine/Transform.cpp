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
		m_Position = Math::Vec3f(a, b, c);


		m_Scale = Math::Vec3f(1, 1, 1);
	}

	Transform::~Transform()
	{

	}

	Math::Vec3f Transform::GetPosition() const
	{
		return m_Position;
	}

	Math::Vec3f Transform::GetRotation() const
	{
		return m_Rotation;
	}

	Math::Vec3f Transform::GetScale() const
	{
		return m_Scale;
	}

	Math::Mat4 Transform::getMatrix()
	{
		//if (Position.isDirty() || Rotation.isDirty() || Scale.isDirty()) {
			//Position.setDirty(false);
			//Rotation.setDirty(false);
			//Scale.setDirty(false);

			m_Matrix = Math::Mat4::Identity();
			if (m_SceneObject) {
				//SceneObject* parent = p_SceneObject->getParent();
				//if (parent && parent->getParent() != 0 /* is not the root */)
				//	m_Matrix = parent->getComponent<Transform>()->getMatrix();
			}

			m_Matrix = m_Matrix * Math::Mat4::Translation(m_Position);

			Math::Vec3f rot = m_Rotation;
			m_Matrix = m_Matrix * Math::Mat4::Rotation(rot.x, Math::Vec3f(1, 0, 0));
			m_Matrix = m_Matrix * Math::Mat4::Rotation(rot.y, Math::Vec3f(0, 1, 0));
			m_Matrix = m_Matrix * Math::Mat4::Rotation(rot.z, Math::Vec3f(0, 0, 1));
			m_Matrix = m_Matrix * Math::Mat4::Scale(m_Scale);
		//}

		return m_Matrix;
	}

	void Transform::SetPosition(const Math::Vec3f& pos)
	{
		m_Position = pos;
	}

	void Transform::SetRotation(const Math::Vec3f& rot)
	{
		m_Rotation = rot;
	}

	void Transform::SetScale(const Math::Vec3f& scale)
	{
		m_Scale = scale;
	}

	NATIVE_REFLECTION_BEGIN(Transform)
		NATIVE_REFLECTION_MEMBER(m_Position)
		NATIVE_REFLECTION_MEMBER(m_Rotation)
		NATIVE_REFLECTION_MEMBER(m_Scale)
	NATIVE_REFLECTION_END()
} }