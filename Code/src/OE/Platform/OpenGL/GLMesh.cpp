#include "OE/Platform/OpenGL/GLMesh.hpp"

#include "OE/Platform/OpenGL/GLVertexArrays.hpp"

namespace OrbitEngine {	namespace Graphics {

	GLMesh::GLMesh(void* vertices, unsigned int vertexSize, VertexLayout* layout, const std::vector<unsigned short>& indices)
		: Mesh(vertices, vertexSize, layout, indices), m_VAO(0)
	{
	}

	GLMesh::~GLMesh()
	{
	}

	void GLMesh::preDraw()
	{
		if (m_VAO == nullptr) { // We ensure that the VAO is generated in the render thread
			m_VAO = new GLVertexArrays(p_Topology);
			m_VAO->push(p_VBO);
		}

		m_VAO->bind();
		m_VAO->setTopology(p_Topology);
	}

	void GLMesh::drawIndexed(unsigned int count, unsigned int offset)
	{
		preDraw();

		p_IBO->bind();
		m_VAO->drawElements(count, offset);
	}

	void GLMesh::draw(unsigned int count, unsigned int offset)
	{
		preDraw();
		
		m_VAO->drawArrays(count, offset);
	}
} }