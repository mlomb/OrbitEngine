#include "OE/Platform/OpenGL/GLVertexArrays.hpp"

#include "OE/Platform/OpenGL/GLVertexBuffer.hpp"
#include "OE/Platform/OpenGL/GLContext.hpp"
#include "OE/Graphics/API/Mesh.hpp"
#include "OE/Platform/OpenGL/GLContext.hpp"
#include "OE/Platform/OpenGL/GLStates.hpp"

namespace OrbitEngine { namespace Graphics {
	GLVertexArrays::GLVertexArrays(Topology topology)
		: m_Topology(topology)
	{
		OE_CHECK_GL(glGenVertexArrays(1, &m_ID));

		m_Context = Application::priv::GLContext::GetCurrent();
	}

	GLVertexArrays::~GLVertexArrays()
	{
		OE_CHECK_GL(glDeleteVertexArrays(1, &m_ID));
	}

	void GLVertexArrays::bind() const
	{
		GLStates* states = static_cast<GLStates*>(Application::priv::GLContext::GetCurrent()->getGlobalStates());
		if (states->cache(0x85B5 /* GL_VERTEX_ARRAY_BINDING */, m_ID))
			return;

		OE_CHECK_GL(glBindVertexArray(m_ID));

		// invalidate buffer bindings
		states->cache(GL_ARRAY_BUFFER, 0);
		states->cache(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GLVertexArrays::drawElements(const unsigned int count, const unsigned int offset) const
	{
		OE_CHECK_GL(glDrawElements(TopologyToGL(m_Topology), count, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(offset * sizeof(unsigned short))));
	}

	void GLVertexArrays::drawArrays(const unsigned int count, const unsigned int offset) const
	{
		OE_CHECK_GL(glDrawArrays(TopologyToGL(m_Topology), offset, count));
	}

	void GLVertexArrays::setTopology(Topology topology)
	{
		m_Topology = topology;
	}

	void GLVertexArrays::push(VertexBuffer* buffer)
	{
		VertexLayout* bufferLayout = buffer->getLayout();

		bind();
		buffer->bind();
		const std::vector<VertexElement>& layout = bufferLayout->getElements();
		for (GLuint i = 0; i < layout.size(); i++) {
			OE_CHECK_GL(glEnableVertexAttribArray(i));
			OE_CHECK_GL(glVertexAttribPointer(i, layout[i].count, layout[i].type, layout[i].normalized, bufferLayout->getStride(), (const GLvoid*)(layout[i].offset)));
		}
		m_Buffers.push_back(buffer);
	}

	GLenum GLVertexArrays::TopologyToGL(Topology topology)
	{
		switch (topology)
		{
		case POINTS:
			return GL_POINTS;
		case TRIANGLES:
			return GL_TRIANGLES;
		case TRIANGLES_STRIP:
			return GL_TRIANGLE_STRIP;
		case TRIANGLES_FAN:
			return GL_TRIANGLE_FAN;
#if !OE_OPENGL_ES
		case QUADS:
			return GL_QUADS;
#endif
		}

		OE_ASSERT(false)

		return 0;
	}
} }