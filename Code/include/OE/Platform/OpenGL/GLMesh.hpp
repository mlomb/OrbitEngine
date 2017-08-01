#ifndef GRAPHICS_GLMESH_HPP
#define GRAPHICS_GLMESH_HPP

#include "OE/Graphics/API/Mesh.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"

namespace OrbitEngine {	namespace Graphics {
	class GLVertexArrays;

	class GLMesh : public Mesh {
	public:
		GLMesh(void* vertices, unsigned int vertexSize, VertexLayout* layout, const std::vector<unsigned short>& indices);
		~GLMesh();

		void drawIndexed(unsigned int count, unsigned int offset) override;
		void draw(unsigned int count, unsigned int offset) override;
	private:
		GLVertexArrays* m_VAO;

		void preDraw();
	};
} }

#endif