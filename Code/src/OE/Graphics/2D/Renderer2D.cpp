#include "OE/Graphics/2D/Renderer2D.hpp"

#include "OE/Graphics/API/Texture.hpp"
#include "OE/Graphics/API/FrameBuffer.hpp"

#include "OE/Math/UV.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Graphics {
	Mesh* Renderer2D::s_Quad = nullptr;

	Renderer2D::Renderer2D()
		: p_Transform(Math::Mat4::Identity())
	{
		m_UPVMatrix = Graphics::UniformsPack<Math::Mat4>::Create();
		setPVMatrices(Math::Mat4::Orthographic(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, 1.0f));
	}


	void Renderer2D::setPVMatrices(const Math::Mat4& projection_matrix, const Math::Mat4& view_matrix)
	{
		p_ProjectionMatrix = projection_matrix;
		p_ViewMatrix = view_matrix;
		m_UPVMatrix->setData(p_ViewMatrix * p_ProjectionMatrix);
	}

	void Renderer2D::setTransform(const Math::Mat4& matrix)
	{
		p_Transform = matrix;
	}

	void Renderer2D::resetTransform()
	{
		setTransform(Math::Mat4::Identity());
	}

	void Renderer2D::begin()
	{
		resetTransform();
	}

	void Renderer2D::end()
	{
		FrameBuffer::Prepare();
	}

	void Renderer2D::RenderQuadScreen()
	{
		struct Vertex2DSimple {
			Math::Vec2f position;
			Math::Vec2f uv;
		};

		if (s_Quad == nullptr) {
			float vertex[] = {
				-1.0f, 1.0f,
				-1.0f, -1.0f,
				1.0f, -1.0f,
				1.0f, 1.0f,
			};

			std::vector<Vertex2DSimple> vertices = std::vector<Vertex2DSimple>();
			std::vector<unsigned short> triangles = Mesh::GenerateIndices(Topology::TRIANGLES, 4);

			for (int i = 0; i < 4; i++) {
				vertices.push_back(Vertex2DSimple{
					Math::Vec2f(vertex[i * 2], vertex[i * 2 + 1]),
					Math::UV_Default.getUV(i)
				});
			}

			VertexLayout* layout = new VertexLayout();
			layout->push<Math::Vec2f>("POSITION");
			layout->push<Math::Vec2f>("TEXCOORD");

			s_Quad = Mesh::Create(vertices, layout, triangles);

			vertices.clear();
			triangles.clear();
		}
		s_Quad->drawIndexed(6);
	}
} }