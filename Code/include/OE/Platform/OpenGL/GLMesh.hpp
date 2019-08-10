#ifndef GRAPHICS_GLMESH_HPP
#define GRAPHICS_GLMESH_HPP

#include <map>

#include "OE/Graphics/API/Mesh.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"
#include "OE/Platform/OpenGL/GLContext.hpp"

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
		// Keep track of the VAOs per context
		// TODO: Eventually delete them
		std::map<Application::priv::GLContext*, GLVertexArrays*> m_VAOs;

		void preDraw();
	};
} }

#endif