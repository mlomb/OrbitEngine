#ifndef GRAPHICS_GLVERTEX_ARRAYS_HPP
#define GRAPHICS_GLVERTEX_ARRAYS_HPP

#include <vector>
#include "OE/Graphics/API/Mesh.hpp"
#include "OE/Platform/OpenGL/OpenGL.hpp"

namespace OrbitEngine { namespace Graphics {
	class VertexBuffer;

	class GLVertexArrays {
	public:
		GLVertexArrays(Topology topology);
		~GLVertexArrays();

		void setTopology(Topology topology);
		void push(VertexBuffer* buffer);
		void bind() const;
		void drawElements(const unsigned int count, const unsigned int offset) const;
		void drawArrays(const unsigned int count, const unsigned int offset) const;
		
		static GLenum TopologyToGL(Topology topology);
	private:
		GLuint m_ID;

		Topology m_Topology;
		std::vector<VertexBuffer*> m_Buffers;
	};
} }

#endif