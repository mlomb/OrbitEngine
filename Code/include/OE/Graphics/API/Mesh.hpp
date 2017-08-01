#ifndef GRAPHICS_MESH_HPP
#define GRAPHICS_MESH_HPP

#include <vector>

#include "OE/Graphics/API/Buffer.hpp"

namespace OrbitEngine {	namespace Graphics {
	class Buffer;
	class VertexBuffer;
	class VertexLayout;

	enum Topology {
		POINTS,
		TRIANGLES,
		TRIANGLES_STRIP,
		TRIANGLES_FAN,
		QUADS,
		PATCHES
	};


	class Mesh {
	public:
		~Mesh();

		virtual void drawIndexed(unsigned int count, unsigned int offset = 0) = 0;
		virtual void draw(unsigned int count, unsigned int offset = 0) = 0;

		VertexBuffer* getVBO();
		Buffer* getIBO();
		Topology getTopology();

		void setTopology(Topology topology);

		template<typename Vertex>
		static Mesh* Create(const std::vector<Vertex>& vertices, VertexLayout* layout, const std::vector<unsigned short>& indices = std::vector<unsigned short>());
		static Mesh* Create(void* vertices, unsigned int vertexSize, VertexLayout* layout, const std::vector<unsigned short>& indices = std::vector<unsigned short>());

		static std::vector<unsigned short> GenerateIndices(Topology topology, unsigned int verticesCount);
	protected:
		Mesh(void* vertices, unsigned int vertexSize, VertexLayout* layout, const std::vector<unsigned short>& indices);

		VertexBuffer* p_VBO;
		Buffer* p_IBO;
		Topology p_Topology = TRIANGLES;
	};

	template<typename Vertex>
	Mesh* Mesh::Create(const std::vector<Vertex>& vertices, VertexLayout* layout, const std::vector<unsigned short>& indices)
	{
		void* verticesPtr = (void*)vertices.data();
		unsigned int vertexSize = sizeof(Vertex) * vertices.size();

		return Create(verticesPtr, vertexSize, layout, indices);
	}
} }

#endif