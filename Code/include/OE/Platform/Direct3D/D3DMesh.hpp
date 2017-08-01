#ifndef GRAPHICS_D3DMESH_HPP
#define GRAPHICS_D3DMESH_HPP

#include "OE/Graphics/API/Mesh.hpp"

#include "OE/Platform/Direct3D/Direct3D.hpp"

namespace OrbitEngine { namespace Graphics {
	class D3DMesh : public Mesh {
	public:
		D3DMesh(void* vertices, unsigned int vertexSize, VertexLayout* layout, const std::vector<unsigned short>& indices);

		void drawIndexed(unsigned int count, unsigned int offset = 0) override;
		void draw(unsigned int count, unsigned int offset = 0) override;
	private:
		static D3D_PRIMITIVE_TOPOLOGY TopologyToD3D(Topology topology);

		void preDraw();
	};
} }

#endif