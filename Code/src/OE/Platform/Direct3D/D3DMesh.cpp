#include "OE/Platform/Direct3D/D3DMesh.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"

namespace OrbitEngine { namespace Graphics {
	D3DMesh::D3DMesh(void* vertices, unsigned int vertexSize, VertexLayout* layout, const std::vector<unsigned short>& indices)
		: Mesh(vertices, vertexSize, layout, indices) {

	}

	void D3DMesh::preDraw()
	{
		p_VBO->bind();
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->IASetPrimitiveTopology(TopologyToD3D(p_Topology));
	}

	void D3DMesh::draw(unsigned int count, unsigned int offset)
	{
		preDraw();
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->Draw(count, offset);
	}

	void D3DMesh::drawIndexed(unsigned int count, unsigned int offset)
	{
		preDraw();
		p_IBO->bind();
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->DrawIndexed(count, 0, offset);
	}

	D3D_PRIMITIVE_TOPOLOGY D3DMesh::TopologyToD3D(Topology topology)
	{
		switch (topology)
		{
		case TRIANGLES:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		case TRIANGLES_STRIP:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		case POINTS:
		default:
			return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		}
	}
} }