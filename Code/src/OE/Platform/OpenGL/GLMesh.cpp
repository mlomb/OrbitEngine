#include "OE/Platform/OpenGL/GLMesh.hpp"

#include "OE/Platform/OpenGL/GLVertexArrays.hpp"

namespace OrbitEngine {	namespace Graphics {

	GLMesh::GLMesh(void* vertices, unsigned int vertexSize, VertexLayout* layout, const std::vector<unsigned short>& indices)
		: Mesh(vertices, vertexSize, layout, indices), m_VAO(NULL)
	{
	}

	GLMesh::~GLMesh()
	{
		// TODO: We should delete them as soon as the context is destroyed
		for (auto& p : m_VAOs)
			delete p.second;
	}

	void GLMesh::preDraw()
	{
		using Application::priv::GLContext;

		GLContext* current_context = GLContext::GetCurrent();
		auto it = m_VAOs.find(current_context);
		if (it != m_VAOs.end()) {
			m_VAO = (*it).second;
		}
		else {
			// We ensure that the VAO is generated in the same context
			m_VAO = new GLVertexArrays(p_Topology);
			m_VAO->push(p_VBO);
			m_VAOs.emplace(current_context, m_VAO);
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