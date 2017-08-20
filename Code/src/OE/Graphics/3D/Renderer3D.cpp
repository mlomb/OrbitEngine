#include "OE/Graphics/3D/Renderer3D.hpp"

#include "OE/Graphics/3D/BRDFLUT.hpp"

namespace OrbitEngine { namespace Graphics {

	VertexLayout* Renderer3D::s_Vertex3DLayout = nullptr;

	void Renderer3D::begin()
	{
		p_Commands.clear();
	}

	void Renderer3D::submitMesh(Mesh* mesh, unsigned int count, Material* material, const Math::Mat4& transform)
	{
		Command command;
		command.mesh = mesh;
		command.count = count;
		command.material = material;
		command.transform = transform;

		p_Commands.push_back(command);
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

	Renderer3D::Renderer3D()
	{
		m_CameraBufferPack = UniformsPack<CameraBuffer>::Create();
		m_ObjectBufferPack = UniformsPack<ObjectBuffer>::Create();
		p_BRDFLUT = new BRDFLUT();
	}

	Renderer3D::~Renderer3D()
	{
		delete m_CameraBufferPack;
		delete m_ObjectBufferPack;
		delete p_BRDFLUT;
	}

	void Renderer3D::fillCameraBuffer(Shader* shader)
	{
		CameraBuffer data;
		data.CameraPosition = p_Camera->getPosition();
		data.PVMatrix = p_Camera->getViewMatrix() * p_Camera->getProjectionMatrix();
		m_CameraBufferPack->setData(data);
		m_CameraBufferPack->bind("CameraBuffer", shader);
	}

	void Renderer3D::fillObjectBuffer(Shader* shader, const Command& command)
	{
		ObjectBuffer data;
		data.MMatrix = command.transform;
		m_ObjectBufferPack->setData(data);
		m_ObjectBufferPack->bind("ObjectBuffer", shader);
	}
} }