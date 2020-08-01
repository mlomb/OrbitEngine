#include "OE/UI/Painter.hpp"

#include "OE/Misc/Log.hpp"
#include "OE/Graphics/API/VertexLayout.hpp"
#include "OE/Graphics/ShaderLoader.hpp"

namespace OrbitEngine { namespace UI {
	Painter::Painter()
		: m_pVertex(NULL), m_pIndex(NULL), m_VertexCount(0), m_IndexCount(0)
	{
		Graphics::VertexLayout* layout = new Graphics::VertexLayout();
		layout->push<Math::Vec2f>("POSITION");
		layout->push<Math::Color4f>("COLOR");
		layout->push<Math::Vec2f>("TEXCOORD"); // uvs
		layout->push<float>("TEXID"); // flags

		m_Mesh = Graphics::Mesh::Create(layout, true);
		m_Mesh->setTopology(Graphics::Topology::TRIANGLES);

		m_Shader = Graphics::ShaderLoader::UI();
		m_Shader->matchLayout(m_Mesh);

		m_Mesh->getVBO()->resize(4 * 10000 * sizeof(Vertex));
		m_Mesh->getIBO()->resize(6 * 10000 * sizeof(uint16_t));
		
		m_ConstantBuffer = Graphics::UniformsPack<ConstantBuffer>::Create();
	}

	Painter::~Painter()
	{
		OE_ASSERT(m_pVertex == NULL && m_pIndex == NULL);
		delete m_Mesh;
		delete m_Shader;
	}

	void Painter::drawRectangle(const Math::Vec2f& position, const Math::Vec2f& size, const Math::Color4f& color)
	{
		*m_pVertex = { position, color, Math::Vec2f(0, 0), 0 }; m_pVertex++;
		*m_pVertex = { position + Math::Vec2f(0, size.y), color, Math::Vec2f(0, 0), 0 }; m_pVertex++;
		*m_pVertex = { position + Math::Vec2f(size.x, size.y), color, Math::Vec2f(0, 0), 0 }; m_pVertex++;
		*m_pVertex = { position + Math::Vec2f(size.x, 0), color, Math::Vec2f(0, 0), 0 }; m_pVertex++;

		*m_pIndex = m_VertexCount + 2; m_pIndex++;
		*m_pIndex = m_VertexCount + 1; m_pIndex++;
		*m_pIndex = m_VertexCount + 0; m_pIndex++;
		*m_pIndex = m_VertexCount + 0; m_pIndex++;
		*m_pIndex = m_VertexCount + 3; m_pIndex++;
		*m_pIndex = m_VertexCount + 2; m_pIndex++;

		m_IndexCount += 6;
		m_VertexCount += 4;
	}

	void Painter::drawText(const std::string& text, const Math::Vec2f& position, Graphics::Font* font, const Graphics::TextSettings& textSettings)
	{
		// 
	}

	void Painter::setProjection(const Math::Mat4& proj)
	{
		m_ConstantBufferData.pr_matrix = proj;
	}

	void Painter::begin()
	{
		m_pVertex = (Vertex*)m_Mesh->getVBO()->mapPointer(Graphics::MappedPointerMode::WRITE);
		m_pIndex = (uint16_t*)m_Mesh->getIBO()->mapPointer(Graphics::MappedPointerMode::WRITE);

		m_VertexCount = 0;
		m_IndexCount = 0;
	}

	void Painter::flush()
	{
		end();
		begin();
	}

	void Painter::end()
	{
		m_Mesh->getVBO()->unmapPointer();
		m_Mesh->getIBO()->unmapPointer();
		m_pVertex = 0;
		m_pIndex = 0;

		Graphics::States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();
		states->setBlending(Graphics::BlendState::SRC_ALPHA);
		states->setDepthTest(Graphics::FunctionMode::DISABLED);
		states->setStencil(Graphics::FunctionMode::DISABLED);
		states->setCullMode(Graphics::CullMode::NONE);

		m_Shader->bind();
		m_ConstantBuffer->bind("PVMatrices", m_Shader);
		m_ConstantBuffer->setData(m_ConstantBufferData);

		m_Mesh->drawIndexed(m_IndexCount);
	}
} }
