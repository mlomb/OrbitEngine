#include "OE/Graphics/3D/Renderer3D.hpp"

namespace OrbitEngine { namespace Graphics {

	VertexLayout* Renderer3D::s_Vertex3DLayout = nullptr;

	void Renderer3D::begin()
	{
		m_Commands.clear();
	}

	void Renderer3D::submitMesh(Mesh* mesh, unsigned int count, Material* material, const Math::Mat4& transform)
	{
		Command command;
		command.mesh = mesh;
		command.count = count;
		command.material = material;
		command.transform = transform;

		m_Commands.push_back(command);
	}
	
	VertexLayout* Renderer3D::GetVertex3DLayout()
	{
		if (s_Vertex3DLayout == nullptr) {
			s_Vertex3DLayout = new VertexLayout();
			s_Vertex3DLayout->push<Math::Vec3f>("POSITION");
			s_Vertex3DLayout->push<Math::Vec3f>("NORMALS");
			s_Vertex3DLayout->push<Math::Vec2f>("TEXCOORDS");
			s_Vertex3DLayout->push<Math::Vec3f>("TANGENT");
		}

		return s_Vertex3DLayout;
	}
} }