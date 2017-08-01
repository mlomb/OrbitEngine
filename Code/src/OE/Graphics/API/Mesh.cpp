#include "OE/Graphics/API/Mesh.hpp"

#include "OE/Misc/Log.hpp"
#include "OE/Application/Context.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLMesh.hpp"
#endif
#if OE_D3D
	#include "OE/Platform/Direct3D/D3DMesh.hpp"
#endif

namespace OrbitEngine {	namespace Graphics {
	Mesh* Mesh::Create(void* vertices, unsigned int vertexSize, VertexLayout* layout, const std::vector<unsigned short>& indices)
	{
		switch (Application::Context::GetCurrentAPI()) {
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			return new GLMesh(vertices, vertexSize, layout, indices);
#endif
#if OE_D3D
		case DIRECT3D:
			return new D3DMesh(vertices, vertexSize, layout, indices);
#endif
		default:
			OE_ASSERT_MSG(false, No mesh implementation!)
				return 0;
		}
	}
	
	std::vector<unsigned short> Mesh::GenerateIndices(Topology topology, unsigned int verticesCount)
	{
		std::vector<unsigned short> indices = std::vector<unsigned short>();
		switch (topology)
		{
		case TRIANGLES:
		{
			int total = (verticesCount / 4) * 6;
			indices.resize(total);

			int offset = 0;
			int t = 0;
			for (int i = 0; i < total; i += 6) {
				indices[t++] = offset + 2;
				indices[t++] = offset + 1;
				indices[t++] = offset + 0;

				indices[t++] = offset + 0;
				indices[t++] = offset + 3;
				indices[t++] = offset + 2;

				offset += 4;
			}
		}
		break;
		case TRIANGLES_FAN:
		{
			int total = 3 * (verticesCount - 2);
			indices.resize(total);

			int t = 0;
			for (int n = 2; n < verticesCount; ++n) {
				indices[t++] = 0;
				indices[t++] = n - 1;
				indices[t++] = n;
			}
		}
			break;
		default:
			break;
		}
		return indices;
	}
	
	Mesh::Mesh(void* vertices, unsigned int vertexSize, VertexLayout* layout, const std::vector<unsigned short>& indices)
	{
		p_VBO = Buffer::CreateVertexBuffer(layout, vertexSize, vertices);
		if(indices.size() > 0)
			p_IBO = Buffer::CreateIndexBuffer(sizeof(unsigned short) * indices.size(), indices.data());
	}

	Mesh::~Mesh()
	{
	}

	VertexBuffer* Mesh::getVBO()
	{
		return p_VBO;
	}

	Buffer* Mesh::getIBO()
	{
		return p_IBO;
	}

	Topology Mesh::getTopology()
	{
		return p_Topology;
	}

	void Mesh::setTopology(Topology topology)
	{
		p_Topology = topology;
	}
} }