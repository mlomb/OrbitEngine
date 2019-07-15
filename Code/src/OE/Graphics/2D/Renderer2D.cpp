#include "OE/Graphics/2D/Renderer2D.hpp"

#include "OE/Graphics/API/Texture.hpp"
#include "OE/Graphics/API/FrameBuffer.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Graphics {
	Mesh* Renderer2D::s_Quad = nullptr;

	Renderer2D::Renderer2D()
		: p_Transform(Math::Mat4::Identity()), p_BindTexture(0)
	{
		p_ScissorStack = new Misc::ScissorStack();

		m_UPVMatrix = Graphics::UniformsPack<Math::Mat4>::Create();
		m_UPVMatrix->setData(Math::Mat4::Orthographic(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, 1.0f));
	}

	int Renderer2D::handleTexture(Texture* tex)
	{
		if (tex == 0) {
			p_BindTexture = 0;
			return 0;
		}

		for (unsigned short i = 0; i < p_Textures.size(); i++)
			if (p_Textures[i] == tex) {
				p_BindTexture = (i + 1);
				return p_BindTexture;
			}

		if (p_Textures.size() >= OE_2D_MAX_TEXTURES) {
			OE_LOG_WARNING("Renderer2D: maximum of binded textured exceeded");
			p_BindTexture = 0;
			return 0;
		}

		p_Textures.push_back(tex);
		p_BindTexture = (unsigned short)p_Textures.size();

		return p_BindTexture;
	}

	Misc::ScissorStack* Renderer2D::getScissorStack()
	{
		return p_ScissorStack;
	}

	void Renderer2D::bindTexture(Texture* tex)
	{
		p_BindTexture = handleTexture(tex);
	}

	void Renderer2D::setPVMatrix(const Math::Mat4& matrix)
	{
		m_UPVMatrix->setData(matrix);
	}

	void Renderer2D::setTransform(const Math::Mat4& matrix)
	{
		p_Transform = matrix;
	}

	void Renderer2D::rect(const Math::Vec2f& position, const Math::Vec2f& size)
	{
		rect(position, size, Math::UV_Default);
	}

	void Renderer2D::rect(const Math::Vec2f& position, const Math::Vec2f& size, const Math::UV& uvs, bool flipX, bool flipY)
	{
		m_TempVertexs[0].x = position.x;
		m_TempVertexs[0].y = position.y;
		m_TempVertexs[1].x = position.x;
		m_TempVertexs[1].y = position.y + size.y;
		m_TempVertexs[2].x = position.x + size.x;
		m_TempVertexs[2].y = position.y + size.y;
		m_TempVertexs[3].x = position.x + size.x;
		m_TempVertexs[3].y = position.y;
		/*
		if (m_MatrixStack->countStack() > 0) { // Skip the multiplication to the identity
			Mat4& mat = m_MatrixStack->get();
			m_TempVertexs[0] = mat.multiply(m_TempVertexs[0]);
			m_TempVertexs[1] = mat.multiply(m_TempVertexs[1]);
			m_TempVertexs[2] = mat.multiply(m_TempVertexs[2]);
			m_TempVertexs[3] = mat.multiply(m_TempVertexs[3]);
		}
		*/
		if (flipX) {
			// TODO See WRONG
			m_TempUVs[0] = uvs._4;
			m_TempUVs[1] = uvs._3;
			m_TempUVs[2] = uvs._2;
			m_TempUVs[3] = uvs._1;
		}
		else if (flipY) {
			// TODO
			OE_LOG_WARNING("TODO FlipY!");
		}
		else if (flipX && flipY) {
			// TODO
			OE_LOG_WARNING("TODO FlipX && FlipY!");
		}
		else {
			m_TempUVs[0] = uvs._1;
			m_TempUVs[1] = uvs._2;
			m_TempUVs[2] = uvs._3;
			m_TempUVs[3] = uvs._4;
		}

		submitRect(m_TempVertexs, m_TempUVs);
	}

	void Renderer2D::begin()
	{
		p_BindTexture = 0;
		p_Textures.clear();
		bindColor(Math::Color(1, 1, 1, 1));
	}

	void Renderer2D::end()
	{
		FrameBuffer::Prepare();
		for (unsigned int i = 0; i < p_Textures.size(); i++)
			p_Textures[i]->bind(i);
	}

	void Renderer2D::RenderQuadScreen()
	{
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