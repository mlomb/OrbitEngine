#ifndef GRAPHICS_GLBUFFER_HPP
#define GRAPHICS_GLBUFFER_HPP

#include "OE/Graphics/API/Buffer.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"

namespace OrbitEngine {	namespace Graphics {
	class GLBuffer : public virtual Buffer {
	public:
		GLBuffer(unsigned int target, unsigned int usage, const unsigned int size = 0, const void* data = 0);
		~GLBuffer();

		GLuint getID() const;

		void bind() const override;
		void resize(const unsigned int size) override;
		void setData(const unsigned int size, const void* data) override;

		void* mapPointer(MappedPointerMode mode) override;
		void unmapPointer() override;

		static GLenum MappedPointerModeToGL(MappedPointerMode mode);
	protected:
		GLuint m_ID;
		GLuint m_Target, m_Usage;
	};
} }

#endif