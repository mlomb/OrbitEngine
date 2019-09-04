#include "OE/Platform/OpenGL/GLBuffer.hpp"

#include "OE/Application/Context.hpp"
#include "OE/Misc/Log.hpp"
#include "OE/Platform/OpenGL/GLContext.hpp"
#include "OE/Platform/OpenGL/GLStates.hpp"

namespace OrbitEngine {	namespace Graphics {
	GLBuffer::GLBuffer(unsigned int target, unsigned int usage, const unsigned int size, const void* data)
		: m_Target(target), m_Usage(usage), m_ID(0)
	{
		OE_CHECK_GL(glGenBuffers(1, &m_ID));
		OE_ASSERT(m_ID);

		if(data){
			bind();
			setData(size, data);
		}
	}

	GLBuffer::~GLBuffer()
	{
		OE_CHECK_GL(glDeleteBuffers(1, &m_ID));
	}

	GLuint GLBuffer::getID() const
	{
		return m_ID;
	}

	void GLBuffer::bind() const {
		GLStates* states = static_cast<GLStates*>(Application::priv::GLContext::GetCurrent()->getGlobalStates());
		if (states->cache(m_Target, m_ID))
			return;

		OE_CHECK_GL(glBindBuffer(m_Target, m_ID));
	}

	void GLBuffer::setData(const unsigned int size, const void* data) {
		OE_ASSERT(size);
		bind();
		p_Size = size;
		OE_CHECK_GL(glBufferData(m_Target, size, data, m_Usage));
	}

	void* GLBuffer::mapPointer(MappedPointerMode mode) {
		OE_ASSERT(p_Size)

		void* ptr;
		// GL_MAP_UNSYNCHRONIZED_BIT
		bind();
		OE_CHECK_GL(ptr = glMapBufferRange(m_Target, 0, p_Size, MappedPointerModeToGL(mode)));

		return ptr;
	}

	void GLBuffer::unmapPointer() {
		bind();
#if OE_EMSCRIPTEN
		OE_CHECK_GL(glFlushMappedBufferRange(m_Target, 0, p_Size));
#endif
		OE_CHECK_GL(glUnmapBuffer(m_Target));
	}

	void GLBuffer::resize(unsigned int size)
	{
		setData(size, 0);
	}

	GLenum GLBuffer::MappedPointerModeToGL(MappedPointerMode mode)
	{
		switch (mode)
		{
#if !OE_EMSCRIPTEN
		case READ:
			return GL_MAP_READ_BIT;
#endif
		case WRITE:
#if OE_EMSCRIPTEN
			return GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT; // MAP_WRITE
#else
			return GL_MAP_WRITE_BIT;
#endif
		}
		OE_ASSERT(false);
		return (GLenum)0;
	}
} }